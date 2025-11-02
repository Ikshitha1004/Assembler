// ============================================================================
// SymbolTable.hpp - Developed by Sahiti
// ============================================================================
#ifndef ASSEMBLER_SYMBOLTABLE_HPP
#define ASSEMBLER_SYMBOLTABLE_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "assembler/Instruction.hpp"

enum class Section { NONE, DATA, TEXT };

// ============================================================================
// Basic symbol info
// ============================================================================

struct LabelInfo {
    uint32_t address;     // absolute byte address = base + LC at definition time
    int line;             
    int col;
    Section section = Section::NONE; // which section label belongs to
};

struct ConstantInfo {
    std::string name;
    int32_t value; // simple int constants
};

struct FieldInfo {
    std::string owner_class;
    std::string name;
    std::string descriptor;
    uint32_t index;
};

struct MethodInfo {
    std::string name;      // "main"
    std::string signature; // e.g., "(I)V" or "([Ljava/lang/String;)V"
    uint32_t address;      // absolute byte address where method code starts (base+offset)
    uint32_t size;         // size of the method in bytes
    uint32_t stack_limit;  // from .limit stack
    uint32_t locals_limit; // from .limit locals
    uint32_t index;
    bool is_defined;  // indicates bytecode has been parsed


    MethodInfo()
        : address(0), size(0), stack_limit(0), locals_limit(0), index(UINT32_MAX),is_defined(false) {}
};

// ============================================================================
// Class metadata
// ============================================================================

struct ClassMetadata {
    std::string name;
    std::string super_name;
    std::vector<FieldInfo> fields;
    std::vector<MethodInfo> methods; // nested MethodInfo for easier linker updates
    uint32_t index;

    ClassMetadata() : index(UINT32_MAX) {}
};

// ============================================================================
// Pending references for control flow
// ============================================================================

struct PendingRef {
    std::size_t instr_index;  // which instruction in the IR
    std::size_t operand_index; // which operand of that instruction
    std::string label;        // label text
    int line;
    int col;
    uint32_t from_code_offset;
    Section section;
    bool resolved = false;
    std::string target_file;
    int is_method_ref = 0; //0 for label,1 for method,2 for class
};

struct RelocationEntry {
   
    uint32_t offset;          // where to patch
    std::string symbol_name;  // what label/variable
    Section section;          // which section
     int is_method_ref = 0; // NEW
};

// ============================================================================
// SymbolTable
// ============================================================================

class SymbolTable {
public:
    explicit SymbolTable(uint32_t base_addr = 0)
        : base_address_(base_addr),
          lc_bytes_(0),
          current_section_(Section::NONE),
          current_class_meta_(nullptr),
          current_class_(""),
          current_method_key_("") {}

    // ------------------------------------------------------------------------
    // Base + LC management
    // ------------------------------------------------------------------------
    void set_base(uint32_t base) { base_address_ = base; }
    uint32_t base() const { return base_address_; }
    std::string get_current_class() const { return current_class_; }
    void reset_lc() { lc_bytes_ = 0; }
    uint32_t lc() const { return lc_bytes_; }
    void advance_lc(uint32_t bytes) { lc_bytes_ += bytes; }

    // ------------------------------------------------------------------------
    // Labels
    // ------------------------------------------------------------------------
    bool define_label(const std::string& name, int line, int col);
    std::pair<bool, LabelInfo> get_label(const std::string& name) const;

    // ------------------------------------------------------------------------
    // Pending control-flow references
    // ------------------------------------------------------------------------
    void add_reference(std::size_t instr_index, std::size_t operand_index,
                       const std::string& label, int line, int col, int is_method=0);
    std::vector<PendingRef>& pending_refs() { return pending_refs_; }
    const std::vector<PendingRef>& pending_refs() const { return pending_refs_; }
    std::vector<RelocationEntry> generate_relocation_table() const;

    // ------------------------------------------------------------------------
    // Constants (.const)
    // ------------------------------------------------------------------------
    bool define_constant(const std::string& name, int32_t value); // false if duplicate
    std::pair<bool, ConstantInfo> get_constant(const std::string& name) const;

    // ------------------------------------------------------------------------
    // Classes / Class metadata
    // ------------------------------------------------------------------------
    bool add_field(const std::string& owner_class, const std::string& field_name,
                   const std::string& descriptor, uint32_t index = UINT32_MAX);
    bool begin_class_metadata(const std::string& class_name, const std::string& super_name);
    bool set_class_super(const std::string& super_name);
    void set_current_class(const std::string& class_name) { current_class_ = class_name; }
    bool add_field_metadata(const FieldInfo& field);
    bool add_method_metadata(const MethodInfo& method);
    bool end_class_metadata();
    std::pair<bool, ClassMetadata> get_class_metadata(const std::string& class_name) const;

    // ------------------------------------------------------------------------
    // Methods
    // ------------------------------------------------------------------------
    bool begin_method(const std::string& method_name, const std::string& signature);
    bool set_method_stack_limit(uint32_t limit);
    bool set_method_locals_limit(uint32_t limit);
    bool set_method_address(uint32_t address);
    bool end_method();
    bool define_method(const std::string& class_name, const std::string& method_name,
                       const std::string& signature, uint32_t address,
                       uint32_t stack_limit, uint32_t locals_limit,bool is_defined);
    std::pair<bool, MethodInfo> get_method(const std::string& method_key) const;
    std::pair<bool, FieldInfo> get_field(const std::string& field_key) const;

    // ------------------------------------------------------------------------
    // Accessors
    // ------------------------------------------------------------------------
    const std::unordered_map<std::string, LabelInfo>& labels() const { return labels_; }
    const std::unordered_map<std::string, ConstantInfo>& constants() const { return constants_; }
    const std::unordered_map<std::string, FieldInfo>& fields() const { return fields_; }
    const std::unordered_map<std::string, MethodInfo>& methods() const { return methods_; }
    const std::unordered_map<std::string, ClassMetadata>& class_metadata() const { return class_metadata_; }
    static std::string make_field_key(const std::string& owner, const std::string& name);
    // static std::string make_method_key(const std::string& owner,
    //                                    const std::string& name,
    //                                    const std::string& sig);

    const std::string& current_class() const { return current_class_; }
    const std::string& current_method_key() const { return current_method_key_; }

    // ------------------------------------------------------------------------
    // Sections (.data / .text)
    // ------------------------------------------------------------------------
    void begin_data() { current_section_ = Section::DATA; }
    void begin_text() { current_section_ = Section::TEXT; }
    Section current_section() const { return current_section_; }

    // Data symbol management
    bool define_data_symbol(const std::string& name, const std::vector<Value>& values);
    std::pair<bool, std::vector<Value>> get_data_symbol(const std::string& name) const;
    const std::unordered_map<std::string, ClassMetadata>& classes() const {
    return class_metadata_;
}

private:
    uint32_t base_address_;
    uint32_t lc_bytes_;

    Section current_section_;
    ClassMetadata* current_class_meta_ = nullptr; // active class metadata
    std::string current_class_;
    std::string current_method_key_;

    std::unordered_map<std::string, LabelInfo> labels_;
    std::vector<PendingRef> pending_refs_;
    std::unordered_map<std::string, ConstantInfo> constants_;

    std::unordered_map<std::string, FieldInfo> fields_;
    std::unordered_map<std::string, MethodInfo> methods_;
    std::unordered_map<std::string, ClassMetadata> class_metadata_;

    // Data symbols (for .data section)
    std::unordered_map<std::string, std::vector<Value>> data_symbols_;
};

#endif // ASSEMBLER_SYMBOLTABLE_HPP