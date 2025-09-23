// ============================================================================
// ConstantPool.hpp - Developed by Sahiti(CS22B056)
// ============================================================================

#ifndef ASSEMBLER_ConstantPool_hpp
#define ASSEMBLER_ConstantPool_hpp

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <iostream>

namespace assembler {

// Tags for our VM constant pool
enum class ConstTag : uint8_t {
    INT    = 1,
    FLOAT  = 2,
    STRING = 3   // UTF8 literal
};

// Constant pool entry
struct ConstEntry {
    ConstTag tag;
    int index;            // pool index (starts at 1)
    std::string str;      // literal stored as string
};

class ConstantPool {
public:
    // Add primitive literals
    int add_int(int32_t v);
    int add_float(float f);
    int add_string(const std::string &s);

    // Access entries
    const std::vector<ConstEntry>& entries() const { return pool_; }

    // Serialize constant pool into binary
    void emit(std::vector<uint8_t> &buf) const;

    // Calculate size in bytes
    uint32_t size_bytes() const;

    // Debug print
    void print() const;

private:
    std::vector<ConstEntry> pool_;
    std::unordered_map<std::string, int> lookup_; // deduplication
    int next_index_ = 1; // pool indices start at 1
};

} // namespace assembler

#endif // ASSEMBLER_ConstantPool_hpp
