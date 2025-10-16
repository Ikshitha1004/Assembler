// Disassembler for .vm (executable) and .vmobj (object) files
// Usage: vm_disasm <file.vm|file.vmobj>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <cstring>

#include "assembler/Instruction.hpp" 
#include "assembler/Utils.hpp"

using namespace std;

// Helpers to read little-endian
static uint8_t read_u8(const vector<uint8_t>& buf, size_t off) {
    return buf.at(off);
}
static uint16_t read_u16(const vector<uint8_t>& buf, size_t off) {
    uint16_t lo = buf.at(off);
    uint16_t hi = buf.at(off+1);
    return (hi << 8) | lo;
}
static uint32_t read_u32(const vector<uint8_t>& buf, size_t off) {
    return (uint32_t)buf.at(off) 
         | ((uint32_t)buf.at(off+1) << 8)
         | ((uint32_t)buf.at(off+2) << 16)
         | ((uint32_t)buf.at(off+3) << 24);
}
static int32_t read_i32(const vector<uint8_t>& buf, size_t off) {
    return static_cast<int32_t>(read_u32(buf, off));
}

// Determine operand byte-size for an opcode (based on your assembler/VM)
// returns number of bytes of operand (0, 2, or 4)




// For .vmobj format we've been using these constants:
static constexpr uint32_t OBJ_MAGIC = 0x004F4D56; // 'VMO\0' little-endian
static constexpr uint32_t OBJ_VERSION = 1;

// Read entire file into vector
static vector<uint8_t> slurp_file(const string &path) {
    ifstream in(path, ios::binary);
    if (!in) throw runtime_error("Cannot open file: " + path);
    in.seekg(0, ios::end);
    size_t sz = (size_t)in.tellg();
    in.seekg(0);
    vector<uint8_t> data(sz);
    if (sz) in.read(reinterpret_cast<char*>(data.data()), sz);
    return data;
}

// Simple container for relocation entries read from .vmobj
struct RelocEntry {
    uint32_t offset;
    string symbol;
    uint32_t section;
};

// Parse .vmobj and extract code bytes, labels, methods, relocations
// Returns true on success and fills code, constpool (if any), labels, methods, relocs
static bool parse_vmobj(const vector<uint8_t>& data,
                        vector<uint8_t>& code_out,
                        unordered_map<string,uint32_t>& label_addr_out,
                        unordered_map<string,uint32_t>& method_addr_out,
                        vector<RelocEntry>& relocs_out)
{
    if (data.size() < 28) return false;
    uint32_t magic = read_u32(data, 0);
    if (magic != OBJ_MAGIC) return false;
    uint32_t version = read_u32(data, 4);
    if (version != OBJ_VERSION) {
        cerr << "Unsupported VMO version: " << version << "\n";
        return false;
    }
    uint32_t codeSize = read_u32(data, 8);
    uint32_t symtabOffset = read_u32(data, 12);
    uint32_t symtabSize = read_u32(data, 16);
    uint32_t relocOffset = read_u32(data, 20);
    uint32_t relocCount = read_u32(data, 24);

    // Code starts at header end (offset 28) in writer logic
    size_t codeStart = 28;
    if (codeStart + codeSize > data.size()) {
        cerr << "VMO: code out of range\n";
        return false;
    }
    code_out.assign(data.begin()+codeStart, data.begin()+codeStart+codeSize);

    // Parse symtab
    size_t p = symtabOffset;
    size_t symEnd = symtabOffset + symtabSize;
    if (symtabOffset == 0 || symtabOffset + 4 > data.size()) {
        // no symtab
    } else {
        // labels
        if (p + 4 > symEnd) return false;
        uint32_t labelCount = read_u32(data, p); p += 4;
        for (uint32_t i = 0; i < labelCount; ++i) {
            if (p + 4 > data.size()) return false;
            uint32_t namelen = read_u32(data, p); p += 4;
            if (p + namelen + 16 > symEnd) return false; // at least addr, line, col, section (4x4)
            string name;
            if (namelen) {
                name.assign(reinterpret_cast<const char*>(&data[p]), namelen);
                p += namelen;
            }
            uint32_t addr = read_u32(data, p); p += 4;
            uint32_t line = read_u32(data, p); p += 4;
            uint32_t col  = read_u32(data, p); p += 4;
            uint32_t section = 0;
            if (p + 4 <= symEnd) {
                section = read_u32(data, p); p += 4;
            }
            label_addr_out[name] = addr;
            (void)line; (void)col; (void)section;
        }

        // methods
        if (p + 4 > symEnd) return false;
        uint32_t methodCount = read_u32(data, p); p += 4;
        for (uint32_t i = 0; i < methodCount; ++i) {
            if (p + 4 > symEnd) return false;
            uint32_t keylen = read_u32(data, p); p += 4;
            string key;
            if (keylen) { key.assign(reinterpret_cast<const char*>(&data[p]), keylen); p += keylen; }

            uint32_t namelen = read_u32(data, p); p += 4;
            string shortname;
            if (namelen) { shortname.assign(reinterpret_cast<const char*>(&data[p]), namelen); p += namelen; }

            uint32_t siglen = read_u32(data, p); p += 4;
            string sig;
            if (siglen) { sig.assign(reinterpret_cast<const char*>(&data[p]), siglen); p += siglen; }

            uint32_t addr = 0;
            if (p + 4 > symEnd) return false;
            addr = read_u32(data, p); p += 4;

            // skip size, stack_limit, locals_limit (3 * u32)
            if (p + 12 > symEnd) return false;
            p += 12;

            method_addr_out[key] = addr;
            (void)shortname; (void)sig;
        }
    }

    // Parse relocation table
    if (relocOffset != 0) {
        size_t p2 = relocOffset;
        if (p2 + 4 > data.size()) return false;
        uint32_t rc = read_u32(data, p2); p2 += 4;
        for (uint32_t i = 0; i < rc; ++i) {
            if (p2 + 4 > data.size()) return false;
            uint32_t off = read_u32(data, p2); p2 += 4;
            if (p2 + 4 > data.size()) return false;
            uint32_t namelen = read_u32(data, p2); p2 += 4;
            string sym;
            if (namelen) {
                if (p2 + namelen > data.size()) return false;
                sym.assign(reinterpret_cast<const char*>(&data[p2]), namelen);
                p2 += namelen;
            }
            uint32_t section = 0;
            if (p2 + 4 > data.size()) return false;
            section = read_u32(data, p2); p2 += 4;

            RelocEntry re { off, sym, section };
            relocs_out.push_back(re);
        }
    }

    return true;
}

// Parse final .vm (executable) format code offset/size and constants (simple)
static bool parse_vm_exec(const vector<uint8_t>& data,
                          size_t &codeOffset, size_t &codeSize,
                          size_t &constPoolOffset, size_t &constPoolSize)
{
    if (data.size() < 44) return false;
    // VM header format used in your VM::loadFromBinary (magic(4) + version + entry + constPoolOffset + constPoolSize + codeOffset + codeSize + globalsOffset + globalsSize + classMetaOffset + classMetaSize)
    // The code in VM::loadFromBinary reads: magic(4) then next 4 as version (32-bit) etc.
    // Let's parse according to that layout:
    uint32_t magic = read_u32(data, 0);
    const uint32_t EXPECTED_MAGIC = 0x01004D56; // you used hdr.magic = 0x01004D56
    if (magic != EXPECTED_MAGIC) return false;
    uint32_t version = read_u32(data, 4);
    uint32_t entry = read_u32(data, 8);
    constPoolOffset = read_u32(data, 12);
    constPoolSize   = read_u32(data, 16);
    codeOffset = read_u32(data, 20);
    codeSize   = read_u32(data, 24);
    // we don't need the rest for disasm
    if (codeOffset + codeSize > data.size()) return false;
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: vm_disasm <file.vm | file.vmobj>\n";
        return 1;
    }
    string path = argv[1];
    vector<uint8_t> filedata;
    try {
        filedata = slurp_file(path);
    } catch (const exception &e) {
        cerr << "Error reading file: " << e.what() << "\n";
        return 1;
    }

    // Try .vmobj first
    vector<uint8_t> code;
    unordered_map<string,uint32_t> labels;
    unordered_map<string,uint32_t> methods;
    vector<RelocEntry> relocs;

    bool is_obj = false;
    if (filedata.size() >= 4 && read_u32(filedata, 0) == OBJ_MAGIC) {
        is_obj = true;
        bool ok = parse_vmobj(filedata, code, labels, methods, relocs);
        if (!ok) {
            cerr << "Failed to parse .vmobj file\n";
            return 1;
        }
        cout << "[VMOBJ] code bytes: " << code.size() << ", labels: " << labels.size() << ", methods: " << methods.size() << ", relocs: " << relocs.size() << "\n";
    } else {
        // Try executable .vm
        size_t codeOffset=0, codeSize=0, constPoolOffset=0, constPoolSize=0;
        bool ok = parse_vm_exec(filedata, codeOffset, codeSize, constPoolOffset, constPoolSize);
        if (!ok) {
            cerr << "Unknown file format or unsupported magic\n";
            return 1;
        }
        code.assign(filedata.begin()+codeOffset, filedata.begin()+codeOffset+codeSize);
        cout << "[VM_EXEC] codeOffset=" << codeOffset << " codeSize=" << codeSize << " constPoolOffset=" << constPoolOffset << " constPoolSize=" << constPoolSize << "\n";
        // .vm executable normally contains class metadata which we don't need for disassembly here
    }

    // Build reloc map: offset -> (symbol, section)
    unordered_map<uint32_t, RelocEntry> reloc_map;
    for (const auto &r : relocs) reloc_map[r.offset] = r;

    // Build reverse map for labels & methods if obj: name -> addr already available
    // Disassemble: iterate through code bytes
        cout << "\n=== DISASSEMBLY ===\n";
    size_t ip = 0;
    cout << std::dec; // default to decimal for readability

    while (ip < code.size()) {
    uint8_t opc = read_u8(code, ip);
    OpCode oc = static_cast<OpCode>(opc);
    size_t instrSize = instruction_size(oc);

    if (ip + instrSize > code.size()) {
        cout << hex << setw(4) << setfill('0') << ip << ": ";
        cout << opcode_to_string(oc) << " [truncated]\n";
        break;
    }

    // --- Print raw bytes ---
    cout << hex << setw(4) << setfill('0') << ip << ": ";
    for (size_t b = 0; b < instrSize; ++b)
        cout << setw(2) << setfill('0') << (int)code[ip + b] << " ";

    // --- Print mnemonic ---
    string mnem = opcode_to_string(oc);
    cout << "   " << left << setw(12) << mnem << right;

    // --- Decode operands ---
    if (instrSize == 3) {
        // 16-bit operand (jumps)
        uint16_t val = read_u16(code, ip + 1);
        cout << " " << dec << val;
        auto it = reloc_map.find(ip + 1);
        if (it != reloc_map.end()) cout << "    ; RELOC -> " << it->second.symbol;
    }
    else if (instrSize == 5) {
        // 32-bit operand (PUSH, CALL, etc.)
        int32_t ival = read_i32(code, ip + 1);
        cout << " " << dec << ival;
        auto it = reloc_map.find(ip + 1);
        if (it != reloc_map.end()) cout << "    ; RELOC -> " << it->second.symbol;
    }
    else if (instrSize == 2) {
        // 8-bit operand (SYS_CALL, NEWARRAY, etc.)
        uint8_t subcode = read_u8(code, ip + 1);

        if (oc == OpCode::SYS_CALL) {
            cout << " " << syscall_to_mnemonic(subcode);
        } else {
            cout << " " << (int)subcode;
        }
    }

    cout << "\n";
    ip += instrSize;
}



    // Also dump relocation table (if present)
    if (!relocs.empty()) {
        cout << "\n=== RELOCATION TABLE ===\n";
        for (const auto &r : relocs) {
            cout << "offset=" << dec << r.offset << " symbol=\"" << r.symbol << "\" section=" << r.section << "\n";
        }
    }

    // And dump labels/methods (if present)
    if (!labels.empty()) {
        cout << "\n=== LABELS ===\n";
        for (const auto &kv : labels) {
            cout << kv.first << " -> " << kv.second << "\n";
        }
    }
    if (!methods.empty()) {
        cout << "\n=== METHODS ===\n";
        for (const auto &kv : methods) {
            cout << kv.first << " -> " << kv.second << "\n";
        }
    }

    return 0;
}
