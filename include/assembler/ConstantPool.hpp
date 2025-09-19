// ============================================================================
// ConstantPool.hpp - JVM-style constant pool for our toy VM
// Updated for simplified OOP references
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
    INT       = 1,
    FLOAT     = 2,
    STRING    = 3,   // UTF8 literal
    CLASSREF  = 4,   // -> STRING index
    FIELDREF  = 8,   // -> class_index, name_or_desc_index
    METHODREF = 9    // -> class_index, name_or_desc_index
};

// Constant pool entry
struct ConstEntry {
    ConstTag tag;
    int index;            // pool index (starts at 1)
    std::string str;      // literal or name stored as string
    int ref1 = 0;         // class index
    int ref2 = 0;         // name or descriptor index
};

class ConstantPool {
public:
    // Add primitive literals
    int add_int(int32_t v);
    int add_float(float f);
    int add_string(const std::string &s);

    // Add OOP references
    int add_class(const std::string &cls);
    int add_field(const std::string &cls, const std::string &field, const std::string &desc = "");
    int add_method(const std::string &cls, const std::string &meth, const std::string &desc = "");

    // Generic add
    int add_entry(ConstTag tag, const std::string &val, int ref1 = 0, int ref2 = 0);

    // Lookup
    int find(ConstTag tag, const std::string &val, int ref1 = 0, int ref2 = 0) const;

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
    std::unordered_map<std::string, int> lookup_; // key = tag|val|ref1|ref2
    int next_index_ = 1; // pool indices start at 1
};

} // namespace assembler

#endif // ASSEMBLER_ConstantPool_hpp
