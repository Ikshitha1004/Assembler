// ============================================================================
// SymbolTable.hpp -Developed by Sahiti
// ============================================================================

#ifndef ASSEMBLER_SymbolTable_hpp
#define ASSEMBLER_SymbolTable_hpp

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

enum class Section { NONE, DATA, TEXT };

struct LabelInfo {
    uint32_t address;     // absolute byte address = base + LC at definition time
    int line;             
    int col;
};

struct ConstantInfo {
    std::string name;
    int32_t     value;    // simple int constants 
};

struct FieldInfo {
    std::string owner_class; 
    std::string name;        
    std::string descriptor; 
    uint32_t    pool_index; 
};

struct MethodInfo {
    std::string name;         // "main"
    std::string signature;    // e.g., "(I)V" or "([Ljava/lang/String;)V"
    uint32_t    address;      // absolute byte address where method code starts (base+offset)
    uint32_t    size;         // size of the method in bytes
    uint32_t    stack_limit;  // from .limit stack
    uint32_t    locals_limit; // from .limit locals
     uint32_t    pool_index; 


    MethodInfo()
        : address(0), stack_limit(0), locals_limit(0) {}
};

struct ClassInfo {
    std::string name;          // "Example"
    std::string super_name;    // "java/lang/Object"
    std::vector<FieldInfo>  fields;
    std::vector<std::string> methods; // store method names (or qualified keys) available in this class
    uint32_t    pool_index;    // constant-pool index if you later add a CP (UINT32_MAX => unknown)

    ClassInfo() : pool_index(UINT32_MAX) {}
};

struct PendingRef {
    // what to patch after pass 1
    std::size_t instr_index;    // which instruction in the IR
    std::size_t operand_index;  // which operand of that instruction
    std::string label;          // label text
    int line;
    int col;

    // for debug: where in code the ref appears (byte LC when encountered)
    uint32_t from_code_offset;
};

// ---------- SymbolTable ----------

class SymbolTable {
public:
    explicit SymbolTable(uint32_t base_addr = 0)
        : base_address_(base_addr), lc_bytes_(0),
          current_class_(""), current_method_key_("") {}

    // ----- Base + LC management -----
    void set_base(uint32_t base) { base_address_ = base; }
    uint32_t base() const { return base_address_; }
    std::string get_current_class() const {
        return current_class_;
    }
    void reset_lc() { lc_bytes_ = 0; }
    uint32_t lc() const { return lc_bytes_; }
    void advance_lc(uint32_t bytes) { lc_bytes_ += bytes; }

    // ----- Labels -----
    // Define label at current absolute addr = base + LC.
    // Returns false if duplicate; on success fills out LabelInfo.
    bool define_label(const std::string& name, int line, int col);

    // Lookup: returns {found, info}
    std::pair<bool, LabelInfo> get_label(const std::string& name) const;

    // ----- Pending control-flow references -----
    void add_label_reference(std::size_t instr_index,
                             std::size_t operand_index,
                             const std::string& label,
                             int line, int col);

    const std::vector<PendingRef>& pending_refs() const { return pending_refs_; }

    // ----- Constants (.const) -----
    bool define_constant(const std::string& name, int32_t value); // false if duplicate
    std::pair<bool, ConstantInfo> get_constant(const std::string& name) const;

    // ----- Classes (.class / .super) -----
    // Begin a class scope (must end with end_class)
    bool begin_class(const std::string& class_name); // false if already declared
    // Set super; returns false if no active class
    bool set_super(const std::string& super_name);
    // Add a field to current or specific class
    bool add_field(const std::string& owner_class,
                   const std::string& field_name,
                   const std::string& descriptor,
                   uint32_t pool_index = UINT32_MAX);
    // End active class
    bool end_class();

    // Lookup class
    std::pair<bool, ClassInfo> get_class(const std::string& class_name) const;

    // ----- Methods (.method / .limit / .entry / .end) -----
    // Begin method scope under the current class (or standalone if class empty)
    // method_key convention: "ClassName.methodName:signature" if class present; else "methodName:signature"
    bool begin_method(const std::string& method_name,
                      const std::string& signature);

    // Set limits/entry on the active method
    bool set_method_stack_limit(uint32_t limit);
    bool set_method_locals_limit(uint32_t limit);

    // Set starting address of active method (usually current base+LC when first instruction emitted)
    bool set_method_address(uint32_t address);

    // End active method scope
    bool end_method();

    // Direct define (for non-scoped usage)
    bool define_method(const std::string& class_name,
                       const std::string& method_name,
                       const std::string& signature,
                       uint32_t address,
                       uint32_t stack_limit,
                       uint32_t locals_limit);

    // Lookup by key (same format as produced by make_method_key)
    std::pair<bool, MethodInfo> get_method(const std::string& method_key) const;
    std::pair<bool, FieldInfo> get_field(const std::string& field_key) const;

    // ----- Diagnostics / Accessors -----
    const std::unordered_map<std::string, LabelInfo>&   labels()   const { return labels_; }
    const std::unordered_map<std::string, ConstantInfo>& constants() const { return constants_; }
    const std::unordered_map<std::string, FieldInfo>&    fields()   const { return fields_; }
    const std::unordered_map<std::string, MethodInfo>&   methods()  const { return methods_; }
    const std::unordered_map<std::string, ClassInfo>&    classes()  const { return classes_; }

    // helpers to build stable keys
    static std::string make_field_key(const std::string& owner,
                                      const std::string& name);
    static std::string make_method_key(const std::string& owner,
                                       const std::string& name,
                                       const std::string& sig);

    // Current scopes (empty string if none)
    const std::string& current_class() const { return current_class_; }
    const std::string& current_method_key() const { return current_method_key_; }

        // ----- Sections (.data / .text) -----
        void begin_data() { current_section_ = Section::DATA; }
        void begin_text() { current_section_ = Section::TEXT; }
        Section current_section() const { return current_section_; }

        // data symbol management
        bool define_data_symbol(const std::string& name, const std::vector<int32_t>& values);
        std::pair<bool, std::vector<int32_t>> get_data_symbol(const std::string& name) const;


private:
    uint32_t base_address_;
    uint32_t lc_bytes_;

    std::unordered_map<std::string, LabelInfo> labels_;
    std::vector<PendingRef> pending_refs_;

    std::unordered_map<std::string, ConstantInfo> constants_;

    // keys:
    //  - field key: "OwnerClass.fieldName"
    //  - method key: "OwnerClass.methodName:signature" or "methodName:signature" if no class
    std::unordered_map<std::string, FieldInfo>  fields_;
    std::unordered_map<std::string, MethodInfo> methods_;
    std::unordered_map<std::string, ClassInfo>  classes_;

    // active scopes
    std::string current_class_;
    std::string current_method_key_;

    // active section tracking
Section current_section_ = Section::NONE;

// data symbols (for .data section)
// name -> vector of values (since a label may refer to an array of constants)
std::unordered_map<std::string, std::vector<int32_t>> data_symbols_;

};

#endif // ARM_ALL_ASSEMBLER_SYMBOLTABLE_HPP
