#include "assembler/Emitter.hpp"
#include "assembler/SymbolTable.hpp"
#include <fstream>
#include <iostream>

using namespace assembler;

void BinaryWriter::writeBytes(const std::vector<uint8_t>& v) {
    buf.insert(buf.end(), v.begin(), v.end());
}

void BinaryWriter::writeString(const std::string& s) {
    uint8_t len = static_cast<uint8_t>(s.size());
    write(len);
    buf.insert(buf.end(), s.begin(), s.end());
}

void assembler::writeVMFile(
    const std::string& filename,
    const std::vector<uint8_t>& code,
    const SymbolTable& symtab
) {
    BinaryWriter writer;

    // --- Build Header ---
    Header hdr{};
    hdr.magic = 0x01004D56;   // "VM\1"
    hdr.version = 1;
    hdr.entryPoint = 0;        // will be patched later

    hdr.constPoolOffset = sizeof(Header);
    hdr.constPoolSize   = 0;

    hdr.codeOffset = sizeof(Header);
    hdr.codeSize   = code.size();

    hdr.globalsOffset = hdr.codeOffset + hdr.codeSize;
    hdr.globalsSize   = 0;

    hdr.classMetadataOffset = hdr.globalsOffset;

    writer.write(hdr);

    // --- Write code ---
    writer.writeBytes(code);

    // --- Write class metadata ---
    size_t classMetaStart = writer.data().size();

    const auto& classes = symtab.classes();
    writer.write(static_cast<uint32_t>(classes.size()));

    uint32_t mainOffset = 0;

    for (const auto& pair : classes) {
        const auto& ci = pair.second;
        writer.writeString(ci.name);

        // Superclass index
        int32_t superIndex = -1;
        if (!ci.super_name.empty()) {
            size_t idx = 0;
            for (const auto& otherPair : classes) {
                if (otherPair.second.name == ci.super_name) {
                    superIndex = static_cast<int32_t>(idx);
                    break;
                }
                ++idx;
            }
        }
        writer.write(superIndex);

        // Fields
        writer.write(static_cast<uint32_t>(ci.fields.size()));
        for (const auto& f : ci.fields) {
            writer.writeString(f.name);
            writer.write(f.pool_index);
        }

        // Methods
        // Methods
writer.write(static_cast<uint32_t>(ci.methods.size()));
for (const auto& mkey : ci.methods) {
    std::pair<bool, MethodInfo> result = symtab.get_method(mkey);
    if (!result.first) continue;
    const MethodInfo& mi = result.second;
    writer.writeString(mi.name);
    writer.write(mi.address);

    if (mi.is_entry) mainOffset = mi.address;
}

    }

    // Patch header
    size_t classMetaEnd = writer.data().size();
    uint32_t classMetaSize = classMetaEnd - classMetaStart;

    Header* hdrPtr = (Header*)writer.data().data();
    hdrPtr->classMetadataOffset = classMetaStart;
    hdrPtr->classMetadataSize   = classMetaSize;
    hdrPtr->entryPoint          = mainOffset;

    // --- Write to file ---
    std::ofstream out(filename, std::ios::binary);
    out.write((char*)writer.data().data(), writer.data().size());

    std::cout << "[Emitter] VM file written: " << filename
              << ", code size: " << code.size()
              << ", classes: " << classes.size()
              << ", main offset: " << mainOffset << std::endl;
}
