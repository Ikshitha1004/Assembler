#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace assembler {

struct Header {
    uint32_t magic;
    uint32_t version;
    uint32_t entryPoint;
    uint32_t constPoolOffset;
    uint32_t constPoolSize;
    uint32_t codeOffset;
    uint32_t codeSize;
    uint32_t globalsOffset;
    uint32_t globalsSize;
    uint32_t classMetadataOffset;
    uint32_t classMetadataSize;
};

struct MethodMeta {
    std::string name;
    uint32_t codeOffset;
};

struct ClassMeta {
    std::string name;
    int32_t superclass; // -1 if none
    std::vector<MethodMeta> methods;
};

class BinaryWriter {
    std::vector<uint8_t> buf;
public:
    template <typename T>
    void write(const T& val) {
        T tmp = val;
        uint8_t* p = (uint8_t*)&tmp;
        buf.insert(buf.end(), p, p + sizeof(T));
    }
    void writeBytes(const std::vector<uint8_t>& v);
    void writeString(const std::string& s);
    const std::vector<uint8_t>& data() const { return buf; }
};

void writeVMFile(
    const std::string& filename,
    const std::vector<uint8_t>& code,
    const std::vector<ClassMeta>& classes,
    uint32_t entryPoint = 0
);

} // namespace assembler
