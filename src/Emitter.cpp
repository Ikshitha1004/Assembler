#include "assembler/Emitter.hpp"
#include <fstream>
#include <cstring>

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
    const std::vector<ClassMeta>& classes,
    uint32_t entryPoint
) {
    BinaryWriter writer;

    // --- Build Header ---
    Header hdr{};
    hdr.magic = 0x01004D56;   // "VM\1"
    hdr.version = 1;
    hdr.entryPoint = entryPoint;

    hdr.constPoolOffset = sizeof(Header);
    hdr.constPoolSize   = 0;

    hdr.codeOffset = sizeof(Header);
    hdr.codeSize   = code.size();

    hdr.globalsOffset = hdr.codeOffset + hdr.codeSize;
    hdr.globalsSize   = 0;

    hdr.classMetadataOffset = hdr.globalsOffset;

    // Write header placeholder
    writer.write(hdr);

    // --- Write code ---
    writer.writeBytes(code);

    // --- Write class metadata ---
    size_t classMetaStart = writer.data().size();

    writer.write((uint32_t)classes.size());
    for (auto& cls : classes) {
        writer.writeString(cls.name);
        writer.write((int32_t)cls.superclass);
        writer.write((uint32_t)0); // field count (not yet used)
        writer.write((uint32_t)cls.methods.size());

        for (auto& m : cls.methods) {
            writer.writeString(m.name);
            writer.write(m.codeOffset);
        }
    }

    size_t classMetaEnd = writer.data().size();
    uint32_t classMetaSize = classMetaEnd - classMetaStart;

    // --- Patch header ---
    Header* hdrPtr = (Header*)writer.data().data();
    hdrPtr->classMetadataOffset = classMetaStart;
    hdrPtr->classMetadataSize   = classMetaSize;

    // --- Write file ---
    std::ofstream out(filename, std::ios::binary);
    out.write((char*)writer.data().data(), writer.data().size());
}
