// ============================================================================
// SymbolTable.cpp - Developed by Sahiti
// ============================================================================

#include "assembler/SymbolTable.hpp"
#include <limits>
#include <iostream>

// ----- Labels -----

bool SymbolTable::define_label(const std::string& name, int line, int col) {
    if (labels_.find(name) != labels_.end()) return false;
    LabelInfo li;
    li.address = base_address_ + lc_bytes_;
    li.line = line;
    li.col = col;
    li.section = current_section_;
    labels_[name] = li;
    return true;
}

std::pair<bool, LabelInfo> SymbolTable::get_label(const std::string& name) const {
    auto it = labels_.find(name);
    if (it == labels_.end()) return {false, LabelInfo{}};
    return {true, it->second};
}

// ----- Pending references -----

void SymbolTable::add_reference(std::size_t instr_index,
                                std::size_t operand_index,
                                const std::string& label,
                                int line, int col,
                                bool is_method) {
    PendingRef pr;
    pr.instr_index = instr_index;
    pr.operand_index = operand_index;
    pr.label = label;
    pr.line = line;
    pr.col = col;
    pr.from_code_offset = lc_bytes_;
    pr.section = current_section_;
    pr.is_method_ref = is_method;
    pending_refs_.push_back(pr);
}

// ----- Constants -----

bool SymbolTable::define_constant(const std::string& name, int32_t value) {
    if (constants_.find(name) != constants_.end()) return false;
    constants_[name] = ConstantInfo{name, value};
    return true;
}

std::pair<bool, ConstantInfo> SymbolTable::get_constant(const std::string& name) const {
    auto it = constants_.find(name);
    if (it == constants_.end()) return {false, ConstantInfo{}};
    return {true, it->second};
}

// ----- Class metadata -----

bool SymbolTable::begin_class_metadata(const std::string& class_name) {
    if (class_metadata_.find(class_name) != class_metadata_.end()) return false;
    ClassMetadata cm;
    cm.name = class_name;
    cm.pool_index = UINT32_MAX;
    class_metadata_[class_name] = cm;
    current_class_meta_ = &class_metadata_[class_name];
    current_class_ = class_name;
    return true;
}

bool SymbolTable::set_class_super(const std::string& super_name) {
    if (!current_class_meta_) return false;
    current_class_meta_->super_name = super_name;
    return true;
}

bool SymbolTable::add_field_metadata(const FieldInfo& field) {
    if (!current_class_meta_) return false;
    current_class_meta_->fields.push_back(field);
    std::string key = make_field_key(field.owner_class, field.name);
    fields_[key] = field;
    return true;
}

bool SymbolTable::add_method_metadata(const MethodInfo& method) {
    if (!current_class_meta_) return false;
    current_class_meta_->methods.push_back(method);
    std::string key = make_method_key(current_class_meta_->name, method.name, method.signature);
    methods_[key] = method;
    return true;
}

bool SymbolTable::end_class_metadata() {
    current_class_meta_ = nullptr;
    current_class_.clear();
    return true;
}

std::pair<bool, ClassMetadata> SymbolTable::get_class_metadata(const std::string& class_name) const {
    auto it = class_metadata_.find(class_name);
    if (it == class_metadata_.end()) return {false, ClassMetadata{}};
    return {true, it->second};
}

// ----- Fields -----

bool SymbolTable::add_field(const std::string& owner_class,
                            const std::string& field_name,
                            const std::string& descriptor,
                            uint32_t pool_index) {
    FieldInfo fi{owner_class, field_name, descriptor, pool_index};
    std::string key = make_field_key(owner_class, field_name);
    if (fields_.find(key) != fields_.end()) return false;

    fields_[key] = fi;

    auto it = class_metadata_.find(owner_class);
    if (it != class_metadata_.end()) {
        it->second.fields.push_back(fi);
    }
    return true;
}

// ----- Methods -----

bool SymbolTable::begin_method(const std::string& method_name, const std::string& signature) {
    std::string key = make_method_key(current_class_, method_name, signature);
    if (methods_.find(key) != methods_.end()) return false;

    MethodInfo mi;
    mi.name = method_name;
    mi.signature = signature;
    mi.address = lc_bytes_;
    mi.size = 0;
    mi.stack_limit = 0;
    mi.locals_limit = 0;
    mi.pool_index = UINT32_MAX;

    methods_[key] = mi;

    // Add to class metadata if active
    if (!current_class_.empty() && current_class_meta_) {
        current_class_meta_->methods.push_back(mi);
    }

    current_method_key_ = key;
    return true;
}

bool SymbolTable::set_method_stack_limit(uint32_t limit) {
    if (current_method_key_.empty()) return false;
    auto it = methods_.find(current_method_key_);
    if (it == methods_.end()) return false;
    methods_[current_method_key_].stack_limit = limit;
    return true;
}

bool SymbolTable::set_method_locals_limit(uint32_t limit) {
    if (current_method_key_.empty()) return false;
    auto it = methods_.find(current_method_key_);
    if (it == methods_.end()) return false;
    it->second.locals_limit = limit;
    return true;
}

bool SymbolTable::set_method_address(uint32_t address) {
    if (current_method_key_.empty()) return false;
    auto it = methods_.find(current_method_key_);
    if (it == methods_.end()) return false;
    it->second.address = address;
    return true;
}

bool SymbolTable::end_method() {
    if (current_method_key_.empty()) return false;

    MethodInfo &mi = methods_[current_method_key_];
    if (lc_bytes_ >= mi.address)
        mi.size = lc_bytes_ - mi.address;
    else
        mi.size = 0;

    current_method_key_.clear();
    return true;
}

bool SymbolTable::define_method(const std::string& class_name,
                                const std::string& method_name,
                                const std::string& signature,
                                uint32_t address,
                                uint32_t stack_limit,
                                uint32_t locals_limit) {
    std::string key = make_method_key(class_name, method_name, signature);
    if (methods_.find(key) != methods_.end()) return false;

    MethodInfo mi;
    mi.name = method_name;
    mi.signature = signature;
    mi.address = address;
    mi.size = 0;
    mi.stack_limit = stack_limit;
    mi.locals_limit = locals_limit;
    mi.pool_index = UINT32_MAX;

    methods_[key] = mi;

    if (!class_name.empty()) {
        auto cit = class_metadata_.find(class_name);
        if (cit != class_metadata_.end())
            cit->second.methods.push_back(mi);
    }

    return true;
}

std::pair<bool, MethodInfo> SymbolTable::get_method(const std::string& method_key) const {
    auto it = methods_.find(method_key);
    if (it == methods_.end()) return {false, MethodInfo{}};
    return {true, it->second};
}

std::pair<bool, FieldInfo> SymbolTable::get_field(const std::string& field_key) const {
    auto it = fields_.find(field_key);
    if (it == fields_.end()) return {false, FieldInfo{}};
    return {true, it->second};
}

// ----- Key builders -----

std::string SymbolTable::make_field_key(const std::string& owner, const std::string& name) {
    return owner + "." + name;
}

std::string SymbolTable::make_method_key(const std::string& owner,
                                         const std::string& name,
                                         const std::string& sig) {
    if (owner.empty()) return name + "(" + sig + ")";
    return owner + "." + name + "(" + sig + ")";
}

// ----- Data symbols -----

bool SymbolTable::define_data_symbol(const std::string& name, const std::vector<Value>& values) {
    if (data_symbols_.find(name) != data_symbols_.end()) return false;
    data_symbols_[name] = values;
    return true;
}

std::pair<bool, std::vector<Value>> SymbolTable::get_data_symbol(const std::string& name) const {
    auto it = data_symbols_.find(name);
    if (it == data_symbols_.end()) return {false, {}};
    return {true, it->second};
}

// ----- Relocation table -----

std::vector<RelocationEntry> SymbolTable::generate_relocation_table() const {
    std::vector<RelocationEntry> relos;
    for (const auto &ref : pending_refs_) {
        if (!ref.resolved) {
            relos.push_back({ref.from_code_offset, ref.label, ref.section});
        }
    }
    return relos;
}