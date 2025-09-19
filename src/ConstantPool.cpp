// ============================================================================
// ConstantPool.cpp - Constant pool for toy VM (OOP-ready)
// ============================================================================
#include "assembler/ConstantPool.hpp"
#include <sstream>
#include <cstring>
#include <iostream>
#include <iomanip>

using namespace assembler;

// --- Helper to build unique keys for deduplication ---
static std::string make_key(ConstTag tag, const std::string &val, int r1 = -1, int r2 = -1) {
    return std::to_string(static_cast<int>(tag)) + "|" + val + "|" + std::to_string(r1) + "|" + std::to_string(r2);
}

// --- Generic add entry ---
int ConstantPool::add_entry(ConstTag tag, const std::string &val, int ref1, int ref2) {
    std::string key = make_key(tag, val, ref1, ref2);
    auto it = lookup_.find(key);
    if (it != lookup_.end()) return it->second;

    ConstEntry e;
    e.tag = tag;
    e.index = next_index_++;
    e.str = val;
    e.ref1 = ref1;
    e.ref2 = ref2;

    pool_.push_back(e);
    lookup_[key] = e.index;
    return e.index;
}

// --- Primitive constants ---
int ConstantPool::add_int(int32_t v) { return add_entry(ConstTag::INT, std::to_string(v)); }

int ConstantPool::add_float(float f) {
    uint32_t bits;
    std::memcpy(&bits, &f, sizeof(bits));
    std::ostringstream oss; oss << "0x" << std::hex << bits;
    return add_entry(ConstTag::FLOAT, oss.str());
}

int ConstantPool::add_string(const std::string &s) { return add_entry(ConstTag::STRING, s); }

// --- OOP references ---
int ConstantPool::add_class(const std::string &cls) {
    int nameIdx = add_string(cls);
    return add_entry(ConstTag::CLASSREF, "", nameIdx);
}

// Removed default arguments here
int ConstantPool::add_field(const std::string &cls, const std::string &field, const std::string &desc) {
    int classIdx = add_class(cls);
    int fieldNameIdx = add_string(field);
    int descIdx = desc.empty() ? -1 : add_string(desc);
    return add_entry(ConstTag::FIELDREF, "", classIdx, descIdx != -1 ? descIdx : fieldNameIdx);
}

int ConstantPool::add_method(const std::string &cls, const std::string &meth, const std::string &desc) {
    int classIdx = add_class(cls);
    int methIdx = add_string(meth);
    int descIdx = desc.empty() ? -1 : add_string(desc);
    return add_entry(ConstTag::METHODREF, "", classIdx, descIdx != -1 ? descIdx : methIdx);
}

// --- Lookup ---
int ConstantPool::find(ConstTag tag, const std::string &val, int ref1, int ref2) const {
    std::string key = make_key(tag, val, ref1, ref2);
    auto it = lookup_.find(key);
    return it != lookup_.end() ? it->second : -1;
}

// --- Size calculation ---
uint32_t ConstantPool::size_bytes() const {
    uint32_t sz = 0;
    for (auto &e : pool_) {
        sz += 1 + 4; // tag + index
        switch (e.tag) {
            case ConstTag::INT:
            case ConstTag::FLOAT: sz += 4; break;
            case ConstTag::STRING: sz += 4 + (uint32_t)e.str.size(); break;
            case ConstTag::CLASSREF: sz += 4; break;
            case ConstTag::FIELDREF:
            case ConstTag::METHODREF: sz += 8; break;
        }
    }
    return sz;
}

// --- Emit to buffer ---
void ConstantPool::emit(std::vector<uint8_t> &buf) const {
    auto write_u8 = [&](uint8_t v) { buf.push_back(v); };
    auto write_u32 = [&](uint32_t v) {
        buf.push_back(v & 0xFF); buf.push_back((v >> 8) & 0xFF);
        buf.push_back((v >> 16) & 0xFF); buf.push_back((v >> 24) & 0xFF);
    };

    for (auto &e : pool_) {
        write_u8((uint8_t)e.tag);
        write_u32(e.index);

        switch (e.tag) {
            case ConstTag::INT: write_u32(static_cast<uint32_t>(std::stoi(e.str))); break;
            case ConstTag::FLOAT: {
                uint32_t bits; std::stringstream ss(e.str); ss >> std::hex >> bits; write_u32(bits); break;
            }
            case ConstTag::STRING: write_u32((uint32_t)e.str.size()); buf.insert(buf.end(), e.str.begin(), e.str.end()); break;
            case ConstTag::CLASSREF: write_u32(e.ref1); break;
            case ConstTag::FIELDREF:
            case ConstTag::METHODREF: write_u32(e.ref1); write_u32(e.ref2); break;
        }
    }
}

// --- Print pool ---
void ConstantPool::print() const {
    std::cout << "=== CONSTANT POOL ===\n";
    for (auto &e : pool_) {
        std::cout << "#" << e.index << " ";
        switch (e.tag) {
            case ConstTag::INT: std::cout << "INT " << e.str; break;
            case ConstTag::FLOAT: std::cout << "FLOAT " << e.str; break;
            case ConstTag::STRING: std::cout << "STRING \"" << e.str << "\""; break;
            case ConstTag::CLASSREF: std::cout << "CLASSREF(name_idx=" << e.ref1 << ")"; break;
            case ConstTag::FIELDREF: std::cout << "FIELDREF(class=" << e.ref1 << ", name_or_desc=" << e.ref2 << ")"; break;
            case ConstTag::METHODREF: std::cout << "METHODREF(class=" << e.ref1 << ", name_or_desc=" << e.ref2 << ")"; break;
        }
        std::cout << "\n";
    }
}
