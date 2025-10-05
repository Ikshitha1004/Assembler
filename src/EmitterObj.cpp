#include "assembler/EmitterObj.hpp"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace assembler;

// Simple obj-file header:
// magic(4) 'VMO\0'  | version(u32) | codeSize(u32) | symtabOffset(u32) | symtabSize(u32) | relocOffset(u32) | relocCount(u32)
static constexpr uint32_t OBJ_MAGIC = 0x004F4D56; // 'VMO\0' little-endian (V M O 0)
static constexpr uint32_t OBJ_VERSION = 1;

void write_u32(std::ofstream &out, uint32_t v) {
    out.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

void write_u16(std::ofstream &out, uint16_t v) {
    out.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

void write_string_with_len(std::ofstream &out, const std::string &s) {
    uint32_t len = static_cast<uint32_t>(s.size());
    write_u32(out, len);
    if (len) out.write(s.data(), len);
}

void assembler::writeObjectFile(
    const std::string& filename,
    const std::vector<uint8_t>& code,
    const SymbolTable& symtab
) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open object file for write: " + filename);

    // header placeholder
    write_u32(out, OBJ_MAGIC);
    write_u32(out, OBJ_VERSION);
    write_u32(out, static_cast<uint32_t>(code.size())); // codeSize
    write_u32(out, 0); // symtabOffset (patch)
    write_u32(out, 0); // symtabSize (patch)
    write_u32(out, 0); // relocOffset (patch)
    write_u32(out, 0); // relocCount (patch)

    // --- write code ---
    size_t codeStart = out.tellp();
    if (!code.empty()) out.write(reinterpret_cast<const char*>(code.data()), code.size());
    size_t codeEnd = out.tellp();

    // --- write symbol table as simple records ---
    size_t symtabOffset = out.tellp();

    // labels
    const auto& labels = symtab.labels();
    uint32_t labelCount = static_cast<uint32_t>(labels.size());
    write_u32(out, labelCount);
    for (const auto& kv : labels) {
        const std::string& name = kv.first;
        const LabelInfo& li = kv.second;
        write_string_with_len(out, name);
        write_u32(out, li.address);
        write_u32(out, static_cast<uint32_t>(li.line));
        write_u32(out, static_cast<uint32_t>(li.col));
        uint32_t sectionByte = static_cast<uint32_t>(li.address == 0 ? 0 : static_cast<uint32_t>(li.address)); // not used; we store section below
        // store section as uint32 (enum)
        write_u32(out, static_cast<uint32_t>(li.col)); // placeholder (not ideal) -> we will retrieve section differently
        // Better: if you added section inside LabelInfo, write it; else skip.
    }

    // methods
    const auto& methods = symtab.methods();
    uint32_t methodCount = static_cast<uint32_t>(methods.size());
    write_u32(out, methodCount);
    for (const auto& kv : methods) {
        const std::string& key = kv.first;
        const MethodInfo& mi = kv.second;
        write_string_with_len(out, key);         // method key (owner.method)
        write_string_with_len(out, mi.name);     // short name
        write_string_with_len(out, mi.signature);
        write_u32(out, mi.address);
        write_u32(out, mi.size);
        write_u32(out, mi.stack_limit);
        write_u32(out, mi.locals_limit);
    }

    size_t symtabEnd = out.tellp();
    uint32_t symtabSize = static_cast<uint32_t>(symtabEnd - symtabOffset);

    // --- write relocation table ---
    size_t relocOffset = out.tellp();
    auto relos = symtab.generate_relocation_table();
    uint32_t relocCount = static_cast<uint32_t>(relos.size());
    write_u32(out, relocCount);
    for (const auto& r : relos) {
        write_u32(out, r.offset); // offset in code
        write_string_with_len(out, r.symbol_name);
        write_u32(out, static_cast<uint32_t>(r.section));
    }
    size_t relocEnd = out.tellp();

    // patch header: symtabOffset, symtabSize, relocOffset, relocCount
    out.seekp(8); // after magic(4) + version(4)
    write_u32(out, static_cast<uint32_t>(code.size())); // codeSize (re-write)
    write_u32(out, static_cast<uint32_t>(symtabOffset));
    write_u32(out, symtabSize);
    write_u32(out, static_cast<uint32_t>(relocOffset));
    write_u32(out, relocCount);

    out.close();
    std::cout << "[EmitterObj] Wrote .vmobj: " << filename << " code=" << code.size()
              << " labels=" << labels.size() << " methods=" << methods.size()
              << " relocs=" << relocCount << "\n";
}
