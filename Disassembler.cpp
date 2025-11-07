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
    uint32_t is_method_ref;
    uint32_t section;
};
// Updated to match Linker::writeFinalVM() classMeta layout exactly
// static bool parse_and_dump_class_meta(const vector<uint8_t>& data, size_t offset, size_t size) {
//     size_t p = offset;
//     const size_t end = offset + size;

//     if (offset + 4 > data.size() || offset + 4 > end) return false;
//     uint32_t classCount = read_u32(data, p); p += 4;

//     std::cout << "\n=== CLASS METADATA ===\n";
//     std::cout << "classCount = " << classCount << "\n";

//     for (uint32_t i = 0; i < classCount; ++i) {
//         // read u8 nameLen
//         if (p + 1 > data.size() || p + 1 > end) return false;
//         uint8_t nameLen = read_u8(data, p); p += 1;

//         if (p + nameLen > data.size() || p + nameLen > end) return false;
//         string className;
//         if (nameLen) {
//             className.assign(reinterpret_cast<const char*>(&data[p]), nameLen);
//             p += nameLen;
//         }

//         if (p + 4 > data.size() || p + 4 > end) return false;
//         uint32_t superIdx_u = read_u32(data, p); p += 4;
//         int32_t superIdx = static_cast<int32_t>(superIdx_u); // -1 means none

//         // field count
//         if (p + 4 > data.size() || p + 4 > end) return false;
//         uint32_t fieldsCount = read_u32(data, p); p += 4;

//         // read fields
//         vector<pair<string,uint32_t>> fields;
//         for (uint32_t f = 0; f < fieldsCount; ++f) {
//             if (p + 1 > data.size() || p + 1 > end) return false;
//             uint8_t fnameLen = read_u8(data, p); p += 1;

//             if (p + fnameLen > data.size() || p + fnameLen > end) return false;
//             string fname;
//             if (fnameLen) {
//                 fname.assign(reinterpret_cast<const char*>(&data[p]), fnameLen);
//                 p += fnameLen;
//             }

//             if (p + 4 > data.size() || p + 4 > end) return false;
//             uint32_t ftype = read_u8(data, p); p += 1;

//             fields.emplace_back(fname, ftype);
//         }

//         // method count
//         if (p + 4 > data.size() || p + 4 > end) return false;
//         uint32_t methodsCount = read_u32(data, p); p += 4;

//         vector<pair<string,uint32_t>> methods;
//         for (uint32_t m = 0; m < methodsCount; ++m) {
//             if (p + 1 > data.size() || p + 1 > end) return false;
//             uint8_t mNameLen = read_u8(data, p); p += 1;

//             if (p + mNameLen + 4 > data.size() || p + mNameLen + 4 > end) return false;
//             string mName;
//             if (mNameLen) {
//                 mName.assign(reinterpret_cast<const char*>(&data[p]), mNameLen);
//                 p += mNameLen;
//             }

//             uint32_t absOffset = read_u32(data, p); p += 4;
//             methods.emplace_back(mName, absOffset);
//         }

//         // Print class info
//         std::cout << "\nClass[" << i << "]: \"" << className << "\"\n";
//         std::cout << "  superIndex = " << superIdx << "\n";
//         std::cout << "  fieldsCount = " << fieldsCount << "\n";
//         std::cout << "  methodsCount = " << methodsCount << "\n";

//         if (!fields.empty()) {
//             std::cout << "  Fields:\n";
//             for (size_t fi = 0; fi < fields.size(); ++fi) {
//                 std::cout << "    [" << fi << "] name=\"" << fields[fi].first
//                      << "\" typeCode=" << fields[fi].second << "\n";
//             }
//         }

//         if (!methods.empty()) {
//             std::cout << "  Methods:\n";
//             for (size_t mi = 0; mi < methods.size(); ++mi) {
//                 std::cout << "    [" << mi << "] name=\"" << methods[mi].first
//                      << "\" -> offset=" << methods[mi].second << "\n";
//             }
//         }
//     }

//     // report any extra bytes (helpful for debugging)
//     if (p != end) {
//         std::cout << "(note: class meta parsed, " << (end - p) << " extra bytes remain)\n";
//     }
//     return true;
// }
static bool parse_and_dump_class_meta_obj(const std::vector<uint8_t>& data, size_t offset, size_t size) {
    size_t p = offset;
    const size_t end = offset + size;

    auto fail = [&](const char *msg) -> bool {
        std::cerr << "[class_meta_parse] " << msg << " (p=" << p << ", end=" << end << ")\n";
        return false;
    };

    if (p + 4 > data.size()) return fail("Missing classCount");
    uint32_t classCount = read_u32(data, p); p += 4;

    std::cout << "\n=== CLASS METADATA ===\n";
    std::cout << "classCount = " << classCount << "\n";

    for (uint32_t i = 0; i < classCount; ++i) {
        // class name
        uint32_t classNameLen = read_u32(data, p); p += 4;
        std::string className(reinterpret_cast<const char*>(&data[p]), classNameLen);
        p += classNameLen;

        // super name
        uint32_t superNameLen = read_u32(data, p); p += 4;
        std::string superName(reinterpret_cast<const char*>(&data[p]), superNameLen);
        p += superNameLen;

        std::cout << "\nClass[" << i << "] name=\"" << className
                  << "\" super=\"" << superName << "\"\n";

        // --- Methods ---
        uint32_t methodCount = read_u32(data, p); p += 4;
        std::cout << "  Methods: " << methodCount << "\n";
        for (uint32_t m = 0; m < methodCount; ++m) {
            uint32_t mlen = read_u32(data, p); p += 4;
            std::string mName(reinterpret_cast<const char*>(&data[p]), mlen);
            p += mlen;
            uint32_t addr = read_u32(data, p); p += 4;

            std::cout << "    [" << m << "] " << mName << " @ " << addr << "\n";
        }

        // --- Fields ---
        uint32_t fieldCount = read_u32(data, p); p += 4;
        std::cout << "  Fields: " << fieldCount << "\n";
        for (uint32_t f = 0; f < fieldCount; ++f) {
            uint32_t fnameLen = read_u32(data, p); p += 4;
            std::string fname(reinterpret_cast<const char*>(&data[p]), fnameLen);
            p += fnameLen;

            uint32_t ownerLen = read_u32(data, p); p += 4;
            std::string owner(reinterpret_cast<const char*>(&data[p]), ownerLen);
            p += ownerLen;

            uint32_t descLen = read_u32(data, p); p += 4;
            std::string desc(reinterpret_cast<const char*>(&data[p]), descLen);
            p += descLen;

            uint32_t index = read_u32(data, p); p += 4;

            std::cout << "    [" << f << "] name=\"" << fname
                      << "\" owner=\"" << owner
                      << "\" desc=\"" << desc
                      << "\" idx=" << index << "\n";
        }
    }

    if (p != end) {
        std::cout << "(note: parsed metadata, " << (end - p)
                  << " trailing bytes remain)\n";
    }

    return true;
}

// ------------------------------------------------------------------------------------------
// UPDATED parse_vmobj()
// ------------------------------------------------------------------------------------------
static bool parse_vmobj(const std::vector<uint8_t>& data,
                        std::vector<uint8_t>& code_out,
                        std::unordered_map<std::string,uint32_t>& label_addr_out,
                        std::unordered_map<std::string,uint32_t>& method_addr_out,
                        std::unordered_map<std::string,uint32_t>& field_addr_out,
                        std::vector<RelocEntry>& relocs_out,
                        size_t &classMetaOffset,
                        size_t &classMetaSize)

{
    auto read_u32 = [](const std::vector<uint8_t>& d, size_t pos) -> uint32_t {
        return static_cast<uint32_t>(d[pos]) |
               (static_cast<uint32_t>(d[pos+1]) << 8) |
               (static_cast<uint32_t>(d[pos+2]) << 16) |
               (static_cast<uint32_t>(d[pos+3]) << 24);
    };

    auto read_u8 = [](const std::vector<uint8_t>& d, size_t pos) -> uint8_t {
        return d[pos];
    };

    auto fail = [](const std::string& msg) -> bool {
        std::cerr << "VMO parse failed: " << msg << "\n";
        return false;
    };

    if (data.size() < 28)
        return fail("file too small (<28 bytes)");

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

    std::cerr << "[VMO HEADER] magic=" << magic
              << " version=" << version
              << " codeSize=" << codeSize
              << " symtabOffset=" << symtabOffset
              << " symtabSize=" << symtabSize
              << " relocOffset=" << relocOffset
              << " relocCount=" << relocCount << "\n";

    if (magic != OBJ_MAGIC)
        return fail("bad magic (not OBJ_MAGIC)");
    if (version != OBJ_VERSION)
        return fail("unsupported OBJ_VERSION");

    size_t codeStart = 28;
    if (codeStart + codeSize > data.size())
        return fail("code section out of range");

    code_out.assign(data.begin()+codeStart, data.begin()+codeStart+codeSize);

    if (symtabOffset == 0 || symtabOffset + 4 > data.size()) {
        std::cerr << "[VMO] No symbol table found.\n";
        return true;
    }

    size_t p = symtabOffset;
    size_t symEnd = std::min<size_t>(symtabOffset + symtabSize, data.size());

    // --- LABELS ---
    if (p + 4 > symEnd) return fail("label count out of range");
    uint32_t labelCount = read_u32(data, p); p += 4;
    std::cerr << "[VMO] Label count: " << labelCount << "\n";

    for (uint32_t i = 0; i < labelCount; ++i) {
        if (p + 4 > symEnd) return fail("label name length out of range");
        uint32_t namelen = read_u32(data, p); p += 4;
        std::string name;
        if (namelen) {
            if (p + namelen > symEnd) return fail("label name exceeds bounds");
            name.assign(reinterpret_cast<const char*>(&data[p]), namelen);
            p += namelen;
        }
        if (p + 4 > symEnd) return fail("label address out of range");
        uint32_t addr = read_u32(data, p); p += 4;
        if (p + 12 > symEnd) return fail("label metadata out of range");
        p += 12;
        label_addr_out[name] = addr;
    }

    // --- METHODS ---
    if (p + 4 > symEnd) return fail("method count out of range");
    uint32_t methodCount = read_u32(data, p); p += 4;
    std::cerr << "[VMO] Method count: " << methodCount << "\n";

    for (uint32_t i = 0; i < methodCount; ++i) {
        if (p + 4 > symEnd) return fail("method keylen out of range");
        uint32_t keylen = read_u32(data, p); p += 4;
        std::string fullName;
        if (keylen) {
            if (p + keylen > symEnd) return fail("method name exceeds bounds");
            fullName.assign(reinterpret_cast<const char*>(&data[p]), keylen);
            p += keylen;
        }

        if (p + 4 > symEnd) return fail("shortlen out of range");
        uint32_t shortlen = read_u32(data, p); p += 4;
        if (p + shortlen > symEnd) return fail("short name exceeds bounds");
        p += shortlen;

        if (p + 4 > symEnd) return fail("siglen out of range");
        uint32_t siglen = read_u32(data, p); p += 4;
        if (p + siglen > symEnd) return fail("signature exceeds bounds");
        p += siglen;

        if (p + 4 > symEnd) return fail("method address out of range");
        uint32_t addr = read_u32(data, p); p += 4;

        if (p + 12 > symEnd) return fail("method metadata out of range");
        p += 12;

        method_addr_out[fullName] = addr;
    }
// --- CLASSES ---
if (p + 4 > symEnd)
    return fail("class count out of range");

uint32_t classCount = read_u32(data, p);
p += 4;

std::cerr << "[VMO] Class count: " << classCount << "\n";

// The class metadata block starts just before classCount
classMetaOffset = p - 4;  // rewind 4 bytes to include the classCount field
classMetaSize   = symEnd - classMetaOffset;

std::cerr << "[VMO] Delegating to parse_and_dump_class_meta() "
          << "(offset=" << classMetaOffset
          << ", size=" << classMetaSize << ")\n";

// Use the standard metadata parser
if (!parse_and_dump_class_meta_obj(data, classMetaOffset, classMetaSize))
    return fail("class meta parse failed");

// After class meta parsing, advance p to the end of the section
p = symEnd;


       // --- RELOCATIONS ---
    if (relocOffset != 0 && relocOffset + 4 <= data.size()) {
        size_t p2 = relocOffset;
        uint32_t rc = read_u32(data, p2); p2 += 4;
        std::cerr << "[VMO] Relocation count: " << rc << "\n";

        for (uint32_t i = 0; i < rc; ++i) {
            if (p2 + 4 > data.size()) return fail("reloc offset out of range");
            uint32_t off = read_u32(data, p2); p2 += 4;

            if (p2 + 4 > data.size()) return fail("reloc name len out of range");
            uint32_t namelen = read_u32(data, p2); p2 += 4;
            std::string sym;
            if (namelen) {
                if (p2 + namelen > data.size()) return fail("reloc name exceeds bounds");
                sym.assign(reinterpret_cast<const char*>(&data[p2]), namelen);
                p2 += namelen;
            }

            // new field: is_method_ref (optional)
            uint32_t isMethodRef = 0;
            if (p2 + 4 <= data.size()) {
                isMethodRef = read_u32(data, p2);
                p2 += 4;
            } else {
                std::cerr << "[VMO WARN] relocation missing is_method_ref, assuming 0\n";
            }

            if (p2 + 4 > data.size()) return fail("reloc section out of range");
            uint32_t sect = read_u32(data, p2); p2 += 4;

            relocs_out.push_back({off, sym, isMethodRef, sect});
        }
    }


    std::cerr << "[VMO] Parsed successfully.\n";
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
    size_t classMetaOffset = 0, classMetaSize = 0;
    bool ok = parse_vmobj(filedata, code, labels, methods, fields, relocs,
                          classMetaOffset, classMetaSize);
    if (!ok) {
        cerr << "Failed to parse .vmobj file\n";
        return 1;
    }

    std::cout << "[VMOBJ] code bytes: " << code.size()
         << ", labels: " << labels.size()
         << ", methods: " << methods.size()
         << ", fields: " << fields.size()
         << ", relocs: " << relocs.size()
         << ", classMetaOffset=" << classMetaOffset
         << ", classMetaSize=" << classMetaSize << "\n";


    if (!fields.empty()) {
        std::cout << "\n=== FIELDS (object) ===\n";
        for (const auto &kv : fields)
            std::cout << kv.first << " -> index=" << kv.second << "\n";
    }
    if (!methods.empty()) {
        std::cout << "\n=== METHODS (object) ===\n";
        for (const auto &kv : methods)
            std::cout << kv.first << " -> addr=" << kv.second << "\n";
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

        std::cout << "[VM_EXEC] codeOffset=" << codeOffset
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
    std::cout << "\n=== DISASSEMBLY ===\n";
    size_t ip = 0;
    std::cout << std::dec; // default to decimal for readability

    while (ip < code.size()) {
        uint8_t opc = read_u8(code, ip);
        OpCode oc = static_cast<OpCode>(opc);
        size_t instrSize = instruction_size(oc);

        if (ip + instrSize > code.size()) {
            std::cout << hex << setw(4) << setfill('0') << ip << ": ";
            std::cout << opcode_to_string(oc) << " [truncated]\n";
            break;
        }

        // --- Print raw bytes ---
        std::cout << hex << setw(4) << setfill('0') << ip << ": ";
        for (size_t b = 0; b < instrSize; ++b)
            std::cout << setw(2) << setfill('0') << (int)code[ip + b] << " ";

        // --- Print mnemonic ---
        string mnem = opcode_to_string(oc);
        std::cout << "   " << left << setw(12) << mnem << right;

        // --- Decode operands ---
        if (instrSize == 3) {
            // 16-bit operand (jumps)
            uint16_t val = read_u16(code, ip + 1);
            std::cout << " " << dec << val;
            auto it = reloc_map.find(ip + 1);
            if (it != reloc_map.end()) std::cout << "    ; RELOC -> " << it->second.symbol;
        }
        else if (instrSize == 5) {
            // 32-bit operand (PUSH, FPUSH, CALL, etc.)
            uint32_t raw = read_u32(code, ip + 1);
            if (is_float_opcode(oc)) {
                float f;
                std::memcpy(&f, &raw, sizeof(f));
                std::cout << " " << std::fixed << std::setprecision(6) << f;
            } else {
                int32_t ival = static_cast<int32_t>(raw);
                std::cout << " " << dec << ival;
            }
            auto it = reloc_map.find(ip + 1);
            if (it != reloc_map.end()) std::cout << "    ; RELOC -> " << it->second.symbol;
        }
        else if (instrSize == 2) {
            // 8-bit operand (SYS_CALL, NEWARRAY, etc.)
            uint8_t subcode = read_u8(code, ip + 1);
            if (oc == OpCode::SYS_CALL) {
                std::cout << " " << syscall_to_mnemonic(subcode);
            } else {
                std::cout << " " << (int)subcode;
            }
        }

        std::cout << "\n";
        ip += instrSize;
    }

    // Also dump relocation table (if present)
    if (!relocs.empty()) {
        std::cout << "\n=== RELOCATION TABLE ===\n";
        for (const auto &r : relocs) {
            std::cout << "offset=" << dec << r.offset << " symbol=\"" << r.symbol << "\" section=" << r.section << "\n";
        }
    }

    // And dump labels/methods/fields (if present)
    if (!labels.empty()) {
        std::cout << "\n=== LABELS ===\n";
        for (const auto &kv : labels) {
            std::cout << kv.first << " -> " << kv.second << "\n";
        }
    }
    if (!methods.empty()) {
        std::cout << "\n=== METHODS ===\n";
        for (const auto &kv : methods) {
            std::cout << kv.first << " -> " << kv.second << "\n";
        }
    }
    if (!fields.empty()) {
        std::cout << "\n=== FIELDS ===\n";
        for (const auto &kv : fields) {
            std::cout << kv.first << " -> " << kv.second << "\n";
        }
    }

    return 0;
}
