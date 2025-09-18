// ============================================================================
//Developed by Sahiti
// ============================================================================
#ifndef ASSEMBLER_ConstantPool_hpp
#define ASSEMBLER_ConstantPool_hpp

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace assembler {

enum class ConstTag : uint8_t {
    INT = 1,
    FLOAT = 2,
    STRING = 3,
    CLASSREF = 4,
    METHODREF = 5,
    FIELDREF = 6
};

struct ConstEntry {
    ConstTag tag;
    std::string value;   // store as string (will parse/encode later)
    int index;           // assigned pool index
};

class ConstantPool {
public:
    // Add literal; return index if already exists
    int add_int(int32_t v);
    int add_float(float f);
    int add_string(const std::string &s);
    int add_class(const std::string &cls);
    int add_method(const std::string &meth);
    int add_field(const std::string &field);

    // General add (auto-detect by opcode usage or token type)
    int add_entry(ConstTag tag, const std::string &val);

    // Lookup
    int find(const std::string &val, ConstTag tag) const;

    // Get all entries
    const std::vector<ConstEntry>& entries() const { return pool_; }

    // Serialize into binary (vector<uint8_t>) at given offset
    void emit(std::vector<uint8_t> &buf) const;

    uint32_t size_bytes() const;

private:
    std::vector<ConstEntry> pool_;
    std::unordered_map<std::string, int> lookup_; // key = tag|value
    int next_index_ = 1; // pool indices start at 1 (like JVM)
};

} // namespace assembler

#endif // ASSEMBLER_ConstantPool_hpp
