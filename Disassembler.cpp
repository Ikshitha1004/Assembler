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

static bool is_float_opcode(OpCode oc) {
    switch (oc) {
        case OpCode::FPUSH:
        case OpCode::FPOP:
        case OpCode::FADD:
        case OpCode::FSUB:
        case OpCode::FMUL:
        case OpCode::FDIV:
        case OpCode::FCMP_EQ:
        case OpCode::FCMP_LT:
        case OpCode::FCMP_GT:
        case OpCode::FCMP_GEQ:
        case OpCode::FCMP_NEQ:
        case OpCode::FCMP_LEQ:
            return true;
        default:
            return false;
    }
}





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
// ------------------------------------------------------------------------------------------
// UPDATED parse_vmobj()
// ------------------------------------------------------------------------------------------
static bool parse_vmobj(const vector<uint8_t>& data,
                        vector<uint8_t>& code_out,
                        unordered_map<string,uint32_t>& label_addr_out,
                        unordered_map<string,uint32_t>& method_addr_out,
                        unordered_map<string,uint32_t>& field_addr_out,
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

    uint32_t codeSize     = read_u32(data, 8);
    uint32_t symtabOffset = read_u32(data, 12);
    uint32_t symtabSize   = read_u32(data, 16);
    uint32_t relocOffset  = read_u32(data, 20);
    uint32_t relocCount   = read_u32(data, 24);

    size_t codeStart = 28;
    if (codeStart + codeSize > data.size()) {
        cerr << "VMO: code out of range\n";
        return false;
    }

    code_out.assign(data.begin()+codeStart, data.begin()+codeStart+codeSize);

    // Parse symbol table
    if (symtabOffset == 0 || symtabOffset + 4 > data.size()) {
        // no symbol table present
        return true;
    }

    size_t p = symtabOffset;
    size_t symEnd = symtabOffset + symtabSize;
    if (symEnd > data.size()) symEnd = data.size();

    // --- LABELS ---
    if (p + 4 > symEnd) return false;
    uint32_t labelCount = read_u32(data, p); p += 4;
    for (uint32_t i = 0; i < labelCount; ++i) {
        if (p + 4 > symEnd) return false;
        uint32_t namelen = read_u32(data, p); p += 4;
        string name;
        if (namelen) {
            if (p + namelen > symEnd) return false;
            name.assign(reinterpret_cast<const char*>(&data[p]), namelen);
            p += namelen;
        }
        if (p + 4 > symEnd) return false;
        uint32_t addr = read_u32(data, p); p += 4;
        // skip line, col, section (3 * u32)
        if (p + 12 > symEnd) return false;
        (void)read_u32(data, p); p += 4;
        (void)read_u32(data, p); p += 4;
        (void)read_u32(data, p); p += 4;
        label_addr_out[name] = addr;
    }

    // --- METHODS ---
    if (p + 4 > symEnd) return false;
    uint32_t methodCount = read_u32(data, p); p += 4;
    for (uint32_t i = 0; i < methodCount; ++i) {
        if (p + 4 > symEnd) return false;
        uint32_t keylen = read_u32(data, p); p += 4;
        string fullName;
        if (keylen) {
            if (p + keylen > symEnd) return false;
            fullName.assign(reinterpret_cast<const char*>(&data[p]), keylen);
            p += keylen;
        }

        if (p + 4 > symEnd) return false;
        uint32_t shortlen = read_u32(data, p); p += 4;
        if (shortlen) {
            if (p + shortlen > symEnd) return false;
            // skip short name
            p += shortlen;
        }

        if (p + 4 > symEnd) return false;
        uint32_t siglen = read_u32(data, p); p += 4;
        if (siglen) {
            if (p + siglen > symEnd) return false;
            // skip signature
            p += siglen;
        }

        if (p + 4 > symEnd) return false;
        uint32_t addr = read_u32(data, p); p += 4;

        // skip size, stack_limit, locals_limit (3 * u32)
        if (p + 12 > symEnd) return false;
        p += 12;

        method_addr_out[fullName] = addr;
    }

    // --- CLASSES ---
    if (p + 4 > symEnd) return false;
    uint32_t classCount = read_u32(data, p); p += 4;
    for (uint32_t ci = 0; ci < classCount; ++ci) {
        // class name (u32 len + bytes)
        if (p + 4 > symEnd) return false;
        uint32_t nameLen = read_u32(data, p); p += 4;
        string clsName;
        if (nameLen) {
            if (p + nameLen > symEnd) return false;
            clsName.assign(reinterpret_cast<const char*>(&data[p]), nameLen);
            p += nameLen;
        }

        // super name
        if (p + 4 > symEnd) return false;
        uint32_t superLen = read_u32(data, p); p += 4;
        string superName;
        if (superLen) {
            if (p + superLen > symEnd) return false;
            superName.assign(reinterpret_cast<const char*>(&data[p]), superLen);
            p += superLen;
        }

        // methods in class
        if (p + 4 > symEnd) return false;
        uint32_t methodInClassCount = read_u32(data, p); p += 4;
        for (uint32_t j = 0; j < methodInClassCount; ++j) {
            if (p + 4 > symEnd) return false;
            uint32_t mlen = read_u32(data, p); p += 4;
            string mname;
            if (mlen) {
                if (p + mlen > symEnd) return false;
                mname.assign(reinterpret_cast<const char*>(&data[p]), mlen);
                p += mlen;
            }
            if (p + 4 > symEnd) return false;
            uint32_t addr = read_u32(data, p); p += 4;
            // store under "Class::method" to avoid collisions
            method_addr_out[clsName + "::" + mname] = addr;
        }

        // fields in class
        if (p + 4 > symEnd) return false;
        uint32_t fieldCount = read_u32(data, p); p += 4;
        for (uint32_t k = 0; k < fieldCount; ++k) {
            if (p + 4 > symEnd) return false;
            uint32_t nlen = read_u32(data, p); p += 4;
            string fname;
            if (nlen) {
                if (p + nlen > symEnd) return false;
                fname.assign(reinterpret_cast<const char*>(&data[p]), nlen);
                p += nlen;
            }
            if (p + 4 > symEnd) return false;
            uint32_t ownerLen = read_u32(data, p); p += 4;
            string owner;
            if (ownerLen) {
                if (p + ownerLen > symEnd) return false;
                owner.assign(reinterpret_cast<const char*>(&data[p]), ownerLen);
                p += ownerLen;
            }
            if (p + 4 > symEnd) return false;
            uint32_t descLen = read_u32(data, p); p += 4;
            string desc;
            if (descLen) {
                if (p + descLen > symEnd) return false;
                desc.assign(reinterpret_cast<const char*>(&data[p]), descLen);
                p += descLen;
            }
            if (p + 4 > symEnd) return false;
            uint8_t fIndex = read_u8(data, p); p += 1;

            // store field as "Owner.field" -> index (and we could also store descriptor if needed)
            field_addr_out[owner + "." + fname] = fIndex;
            (void)desc;
        }
    }

    // --- RELOCATIONS ---
    if (relocOffset != 0 && relocOffset + 4 <= data.size()) {
        size_t p2 = relocOffset;
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
            if (p2 + 4 > data.size()) return false;
            uint32_t sect = read_u32(data, p2); p2 += 4;
            relocs_out.push_back({off, sym, sect});
        }
    }

    return true;
}


// parse_vm_exec stays the same — kept here for completeness
static bool parse_vm_exec(const vector<uint8_t>& data,
                          size_t &codeOffset, size_t &codeSize,
                          size_t &constPoolOffset, size_t &constPoolSize,
                          uint32_t &entryPoint,
                          size_t &classMetaOffset, size_t &classMetaSize)
{
    if (data.size() < 44) return false;
    uint32_t magic = read_u32(data, 0);
    const uint32_t EXPECTED_MAGIC = 0x01004D56; // bytes {0x56,0x4D,0x00,0x01}
    if (magic != EXPECTED_MAGIC) return false;
    uint32_t version = read_u32(data, 4);
    (void)version;
    entryPoint = read_u32(data, 8);
    constPoolOffset = read_u32(data, 12);
    constPoolSize   = read_u32(data, 16);
    codeOffset = read_u32(data, 20);
    codeSize   = read_u32(data, 24);
    // globals (we don't use)
    classMetaOffset = read_u32(data, 36);
    classMetaSize   = read_u32(data, 40);

    if (codeOffset + codeSize > data.size()) return false;
    if (classMetaOffset + classMetaSize > data.size()) return false;
    return true;
}
// Updated to match Linker::writeFinalVM() classMeta layout exactly
static bool parse_and_dump_class_meta(const vector<uint8_t>& data, size_t offset, size_t size) {
    size_t p = offset;
    const size_t end = offset + size;

    if (offset + 4 > data.size() || offset + 4 > end) return false;
    uint32_t classCount = read_u32(data, p); p += 4;

    cout << "\n=== CLASS METADATA ===\n";
    cout << "classCount = " << classCount << "\n";

    for (uint32_t i = 0; i < classCount; ++i) {
        // read u8 nameLen
        if (p + 1 > data.size() || p + 1 > end) return false;
        uint8_t nameLen = read_u8(data, p); p += 1;

        if (p + nameLen > data.size() || p + nameLen > end) return false;
        string className;
        if (nameLen) {
            className.assign(reinterpret_cast<const char*>(&data[p]), nameLen);
            p += nameLen;
        }

        if (p + 4 > data.size() || p + 4 > end) return false;
        uint32_t superIdx_u = read_u32(data, p); p += 4;
        int32_t superIdx = static_cast<int32_t>(superIdx_u); // -1 means none

        // field count
        if (p + 4 > data.size() || p + 4 > end) return false;
        uint32_t fieldsCount = read_u32(data, p); p += 4;

        // read fields
        vector<pair<string,uint32_t>> fields;
        for (uint32_t f = 0; f < fieldsCount; ++f) {
            if (p + 1 > data.size() || p + 1 > end) return false;
            uint8_t fnameLen = read_u8(data, p); p += 1;

            if (p + fnameLen > data.size() || p + fnameLen > end) return false;
            string fname;
            if (fnameLen) {
                fname.assign(reinterpret_cast<const char*>(&data[p]), fnameLen);
                p += fnameLen;
            }

            if (p + 4 > data.size() || p + 4 > end) return false;
            uint32_t ftype = read_u8(data, p); p += 1;

            fields.emplace_back(fname, ftype);
        }

        // method count
        if (p + 4 > data.size() || p + 4 > end) return false;
        uint32_t methodsCount = read_u32(data, p); p += 4;

        vector<pair<string,uint32_t>> methods;
        for (uint32_t m = 0; m < methodsCount; ++m) {
            if (p + 1 > data.size() || p + 1 > end) return false;
            uint8_t mNameLen = read_u8(data, p); p += 1;

            if (p + mNameLen + 4 > data.size() || p + mNameLen + 4 > end) return false;
            string mName;
            if (mNameLen) {
                mName.assign(reinterpret_cast<const char*>(&data[p]), mNameLen);
                p += mNameLen;
            }

            uint32_t absOffset = read_u32(data, p); p += 4;
            methods.emplace_back(mName, absOffset);
        }

        // Print class info
        cout << "\nClass[" << i << "]: \"" << className << "\"\n";
        cout << "  superIndex = " << superIdx << "\n";
        cout << "  fieldsCount = " << fieldsCount << "\n";
        cout << "  methodsCount = " << methodsCount << "\n";

        if (!fields.empty()) {
            cout << "  Fields:\n";
            for (size_t fi = 0; fi < fields.size(); ++fi) {
                cout << "    [" << fi << "] name=\"" << fields[fi].first
                     << "\" typeCode=" << fields[fi].second << "\n";
            }
        }

        if (!methods.empty()) {
            cout << "  Methods:\n";
            for (size_t mi = 0; mi < methods.size(); ++mi) {
                cout << "    [" << mi << "] name=\"" << methods[mi].first
                     << "\" -> offset=" << methods[mi].second << "\n";
            }
        }
    }

    // report any extra bytes (helpful for debugging)
    if (p != end) {
        cout << "(note: class meta parsed, " << (end - p) << " extra bytes remain)\n";
    }
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
    unordered_map<string,uint32_t> fields;
    vector<RelocEntry> relocs;

    bool is_obj = false;
    if (filedata.size() >= 4 && read_u32(filedata, 0) == OBJ_MAGIC) {
        is_obj = true;
        bool ok = parse_vmobj(filedata, code, labels, methods, fields, relocs);
        if (!ok) {
            cerr << "Failed to parse .vmobj file\n";
            return 1;
        }
        cout << "[VMOBJ] code bytes: " << code.size()
             << ", labels: " << labels.size()
             << ", methods: " << methods.size()
             << ", fields: " << fields.size()
             << ", relocs: " << relocs.size() << "\n";

        // If object had classes/fields, print a brief listing
        if (!fields.empty()) {
            cout << "\n=== FIELDS (object) ===\n";
            for (const auto &kv : fields) {
                cout << kv.first << " -> index=" << kv.second << "\n";
            }
        }
        if (!methods.empty()) {
            cout << "\n=== METHODS (object) ===\n";
            for (const auto &kv : methods) {
                cout << kv.first << " -> addr=" << kv.second << "\n";
            }
        }
    } else {
        // Try executable .vm
        size_t codeOffset=0, codeSize=0, constPoolOffset=0, constPoolSize=0;
        size_t classMetaOffset=0, classMetaSize=0;
        uint32_t entryPoint=0;
        bool ok = parse_vm_exec(filedata, codeOffset, codeSize, constPoolOffset, constPoolSize, entryPoint,
                                classMetaOffset, classMetaSize);
        if (!ok) { cerr << "Unknown file format or unsupported magic\n"; return 1; }
        code.assign(filedata.begin()+codeOffset, filedata.begin()+codeOffset+codeSize);

        cout << "[VM_EXEC] codeOffset=" << codeOffset
            << " codeSize=" << codeSize
            << " constPoolOffset=" << constPoolOffset
            << " constPoolSize=" << constPoolSize
            << " entryPoint=0x" << hex << entryPoint << dec
            << " classMetaOffset=" << classMetaOffset
            << " classMetaSize=" << classMetaSize
            << "\n";

        // parse class metadata if present
        if (classMetaSize > 0) {
            bool okmeta = parse_and_dump_class_meta(filedata, classMetaOffset, classMetaSize);
            if (!okmeta) {
                cerr << "Failed to parse class metadata section\n";
            }
        }
    }

    // Build reloc map: offset -> (symbol, section)
    unordered_map<uint32_t, RelocEntry> reloc_map;
    for (const auto &r : relocs) reloc_map[r.offset] = r;

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
            // 32-bit operand (PUSH, FPUSH, CALL, etc.)
            uint32_t raw = read_u32(code, ip + 1);
            if (is_float_opcode(oc)) {
                float f;
                std::memcpy(&f, &raw, sizeof(f));
                cout << " " << std::fixed << std::setprecision(6) << f;
            } else {
                int32_t ival = static_cast<int32_t>(raw);
                cout << " " << dec << ival;
            }
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

    // And dump labels/methods/fields (if present)
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
    if (!fields.empty()) {
        cout << "\n=== FIELDS ===\n";
        for (const auto &kv : fields) {
            cout << kv.first << " -> " << kv.second << "\n";
        }
    }

    return 0;
}
