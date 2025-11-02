#include "assembler/EmitterObj.hpp"
#include <fstream>
#include <iostream>
#include <cstring>

namespace assembler {
    static constexpr uint32_t OBJ_MAGIC = 0x004F4D56; // 'VMO\0' little-endian
static constexpr uint32_t OBJ_VERSION = 1;

void write_u32(std::ofstream &out, uint32_t v) {
    out.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

void write_string_with_len(std::ofstream &out, const std::string &s) {
    uint32_t len = static_cast<uint32_t>(s.size());
    write_u32(out, len);
    if (len) out.write(s.data(), len);
}

void writeObjectFile(
    const std::string& filename,
    const std::vector<uint8_t>& code,
    const SymbolTable& symtab,
    const std::vector<RelocationEntry>& relos
) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open object file: " + filename);

   
    // Header placeholder
    write_u32(out, OBJ_MAGIC);
    write_u32(out, OBJ_VERSION);
    write_u32(out, static_cast<uint32_t>(code.size())); // code size
    write_u32(out, 0); // symtabOffset
    write_u32(out, 0); // symtabSize
    write_u32(out, 0); // relocOffset
    write_u32(out, 0); // relocCount

    // Write code
    if (!code.empty()) out.write(reinterpret_cast<const char*>(code.data()), code.size());

    // Symbol table offset
    size_t symtabOffset = out.tellp();

    // Labels
    const auto& labels = symtab.labels();
    write_u32(out, static_cast<uint32_t>(labels.size()));
    for (const auto& kv : labels) {
        write_string_with_len(out, kv.first);
        write_u32(out, kv.second.address);
        write_u32(out, kv.second.line);
        write_u32(out, kv.second.col);
        write_u32(out, static_cast<uint8_t>(kv.second.section));
    }

    // Methods
    const auto& methods = symtab.methods();
    write_u32(out, static_cast<uint32_t>(methods.size()));
    for (const auto& kv : methods) {
        const MethodInfo &mi = kv.second;
        write_string_with_len(out, kv.first);   // full name
        write_string_with_len(out, mi.name);    // short name
        write_string_with_len(out, mi.signature);
        write_u32(out, mi.address);
        write_u32(out, mi.size);
        write_u32(out, mi.stack_limit);
        write_u32(out, mi.locals_limit);
    }

    // Classes
    const auto& classes = symtab.classes();
    write_u32(out, static_cast<uint32_t>(classes.size()));
    for (const auto& clsPair : classes) {
        const ClassMetadata &cls = clsPair.second;
        write_string_with_len(out, cls.name);
        write_string_with_len(out, cls.super_name);

        write_u32(out, static_cast<uint32_t>(cls.methods.size()));
        for (const MethodInfo &mi : cls.methods) {
            write_string_with_len(out, mi.name);
            write_u32(out, mi.address);
        }

        write_u32(out, static_cast<uint32_t>(cls.fields.size()));
        for (const FieldInfo &f : cls.fields) {
            write_string_with_len(out, f.name);
            write_string_with_len(out, f.owner_class);
            write_string_with_len(out, f.descriptor);
            write_u32(out, f.index);
        }
    }

    // Relocations
    size_t relocOffset = out.tellp();
    write_u32(out, static_cast<uint32_t>(relos.size()));
    for (const auto& r : relos) {
        write_u32(out, r.offset);
        write_string_with_len(out, r.symbol_name);
        write_u32(out, r.is_method_ref); // NEW
        write_u32(out, static_cast<uint32_t>(r.section));
    }

    // Patch header
    size_t symtabEnd = out.tellp();
    uint32_t symtabSize = static_cast<uint32_t>(symtabEnd - symtabOffset);
    uint32_t relocCount = static_cast<uint32_t>(relos.size());

    out.seekp(8);
    write_u32(out, static_cast<uint32_t>(code.size()));
    write_u32(out, static_cast<uint32_t>(symtabOffset));
    write_u32(out, symtabSize);
    write_u32(out, static_cast<uint32_t>(relocOffset));
    write_u32(out, relocCount);

    out.close();
}

} // namespace assembler