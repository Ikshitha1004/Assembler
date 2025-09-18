// ============================================================================
// SymbolTable.cpp -Developed by Sahiti
// ============================================================================

#include "assembler/SymbolTable.hpp"
#include <limits>
#include <iostream>

// ----- Labels -----

bool SymbolTable::define_label(const std::string& name, int line, int col) {
    if (labels_.find(name) != labels_.end()) {
        return false; // duplicate
    }
    LabelInfo li;
    li.address = base_address_ + lc_bytes_; // absolute byte address at definition
    li.line = line;
    li.col  = col;
    labels_[name] = li;
    return true;
}

std::pair<bool, LabelInfo> SymbolTable::get_label(const std::string& name) const {
    auto it = labels_.find(name);
    if (it == labels_.end()) return {false, LabelInfo{}};
    return {true, it->second};
}

void SymbolTable::add_label_reference(std::size_t instr_index,
                                      std::size_t operand_index,
                                      const std::string& label,
                                      int line, int col) {
    PendingRef pr;
    pr.instr_index = instr_index;
    pr.operand_index = operand_index;
    pr.label = label;
    pr.line = line;
    pr.col  = col;
    pr.from_code_offset = lc_bytes_; // snapshot of LC when reference recorded (debug)
    pending_refs_.push_back(pr);
}

// ----- Constants (.const) -----

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

// ----- Classes (.class / .super / fields) -----

bool SymbolTable::begin_class(const std::string& class_name) {
    if (classes_.find(class_name) != classes_.end()) {
        // already known class - still allow begin if we want to re-enter? keep simple -> reject
        return false;
    }
    ClassInfo ci;
    ci.name = class_name;
    ci.super_name = "";
    ci.pool_index = std::numeric_limits<uint32_t>::max();
    classes_[class_name] = ci;
    current_class_ = class_name;
    return true;
}

bool SymbolTable::set_super(const std::string& super_name) {
    if (current_class_.empty()) return false;
    auto it = classes_.find(current_class_);
    if (it == classes_.end()) return false;
    it->second.super_name = super_name;
    return true;
}

bool SymbolTable::add_field(const std::string& owner_class,
                            const std::string& field_name,
                            const std::string& descriptor,
                            uint32_t pool_index) {
    // ensure class exists
    auto cit = classes_.find(owner_class);
    if (cit == classes_.end()) {
        // auto-create class entry if not present
        ClassInfo ci;
        ci.name = owner_class;
        ci.super_name = "";
        ci.pool_index = std::numeric_limits<uint32_t>::max();
        classes_[owner_class] = ci;
        cit = classes_.find(owner_class);
    }

    std::string key = make_field_key(owner_class, field_name);
    if (fields_.find(key) != fields_.end()) {
        return false; // duplicate field
    }

    FieldInfo fi;
    fi.owner_class = owner_class;
    fi.name = field_name;
    fi.descriptor = descriptor;
    fi.pool_index = pool_index;
    fields_[key] = fi;
    cit->second.fields.push_back(fi);
    return true;
}

bool SymbolTable::end_class() {
    if (current_class_.empty()) return false;
    current_class_.clear();
    return true;
}

std::pair<bool, ClassInfo> SymbolTable::get_class(const std::string& class_name) const {
    auto it = classes_.find(class_name);
    if (it == classes_.end()) return {false, ClassInfo{}};
    return {true, it->second};
}

// ----- Methods (.method / .limit / .entry / .end) -----

bool SymbolTable::begin_method(const std::string& method_name,
                               const std::string& signature) {
    std::string key = make_method_key(current_class_, method_name, signature);

     std::cout<<"KEY"<<key<<std::endl;
    if (methods_.find(key) != methods_.end()) {
        return false; // duplicate method key
    }
    MethodInfo mi;
    mi.name = method_name;
    mi.signature = signature;
    mi.address = 0;
    mi.stack_limit = 0;
    mi.locals_limit = 0;
    mi.is_entry = false;
    methods_[key] = mi;

    // also attach method name to class list (store keys or names)
    if (!current_class_.empty()) {
        auto cit = classes_.find(current_class_);
        if (cit != classes_.end()) {
            cit->second.methods.push_back(key); // store key for unique identification
        }
    }

    current_method_key_ = key;
    return true;
}

bool SymbolTable::set_method_stack_limit(uint32_t limit) {
    // std::cout<<"curr key"<<current_method_key_<<std::endl;
    if (current_method_key_.empty()) return false;
    auto it = methods_.find(current_method_key_);
    if (it == methods_.end()) return false;
    it->second.stack_limit = limit;
    return true;
}
bool SymbolTable::define_data_symbol(const std::string& name, const std::vector<int32_t>& values) {
    if (data_symbols_.find(name) != data_symbols_.end()) return false;
    data_symbols_[name] = values;
    return true;
}

std::pair<bool, std::vector<int32_t>> SymbolTable::get_data_symbol(const std::string& name) const {
    auto it = data_symbols_.find(name);
    if (it == data_symbols_.end()) return {false, {}};
    return {true, it->second};
}


bool SymbolTable::set_method_locals_limit(uint32_t limit) {
    if (current_method_key_.empty()) return false;
    auto it = methods_.find(current_method_key_);
    if (it == methods_.end()) return false;
    it->second.locals_limit = limit;
    return true;
}

bool SymbolTable::set_method_entry(bool is_entry) {
    if (current_method_key_.empty()) return false;
    auto it = methods_.find(current_method_key_);
    if (it == methods_.end()) return false;
    it->second.is_entry = is_entry;
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
    current_method_key_.clear();
    return true;
}

bool SymbolTable::define_method(const std::string& class_name,
                                const std::string& method_name,
                                const std::string& signature,
                                uint32_t address,
                                uint32_t stack_limit,
                                uint32_t locals_limit,
                                bool is_entry) {
    std::string key = make_method_key(class_name, method_name, signature);
    if (methods_.find(key) != methods_.end()) return false;

    MethodInfo mi;
    mi.name = method_name;
    mi.signature = signature;
    mi.address = address;
    mi.stack_limit = stack_limit;
    mi.locals_limit = locals_limit;
    mi.is_entry = is_entry;

    methods_[key] = mi;

    // ensure class exists and attach
    if (!class_name.empty()) {
        auto cit = classes_.find(class_name);
        if (cit == classes_.end()) {
            ClassInfo ci;
            ci.name = class_name;
            ci.super_name = "";
            ci.pool_index = std::numeric_limits<uint32_t>::max();
            classes_[class_name] = ci;
            cit = classes_.find(class_name);
        }
        cit->second.methods.push_back(key);
    }

    return true;
}

std::pair<bool, MethodInfo> SymbolTable::get_method(const std::string& method_key) const {
    auto it = methods_.find(method_key);
    if (it == methods_.end()) return {false, MethodInfo{}};
    return {true, it->second};
}

// ----- Key builders -----

std::string SymbolTable::make_field_key(const std::string& owner,
                                        const std::string& name) {
    return owner + "." + name;
}

std::string SymbolTable::make_method_key(const std::string& owner,
                                         const std::string& name,
                                         const std::string& sig) {
    if (owner.empty()) return name ;
    return owner + "." + name ;
}
