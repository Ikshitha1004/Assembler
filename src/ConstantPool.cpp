// ============================================================================
// Developed by Sahiti
// ============================================================================
#include "assembler/ConstantPool.hpp"
#include <sstream>
#include <cstring>
#include <cmath>

using namespace assembler;

static std::string make_key(ConstTag tag, const std::string &val) {
    return std::to_string(static_cast<int>(tag)) + "|" + val;
}

int ConstantPool::add_entry(ConstTag tag, const std::string &val) {
    std::string key = make_key(tag, val);
    auto it = lookup_.find(key);
    if (it != lookup_.end()) return it->second;

    ConstEntry e;
    e.tag = tag;
    e.value = val;
    e.index = next_index_++;
    pool_.push_back(e);
    lookup_[key] = e.index;
    return e.index;
}

int ConstantPool::add_int(int32_t v) {
    return add_entry(ConstTag::INT, std::to_string(v));
}

int ConstantPool::add_float(float f) {
    // store exact bit pattern as hex string for reproducibility
    uint32_t bits;
    std::memcpy(&bits, &f, sizeof(bits));
    std::ostringstream oss;
    oss << "0x" << std::hex << bits;
    return add_entry(ConstTag::FLOAT, oss.str());
}

int ConstantPool::add_string(const std::string &s) {
    return add_entry(ConstTag::STRING, s);
}

int ConstantPool::add_class(const std::string &cls) {
    return add_entry(ConstTag::CLASSREF, cls);
}

int ConstantPool::add_method(const std::string &meth) {
    return add_entry(ConstTag::METHODREF, meth);
}

int ConstantPool::add_field(const std::string &field) {
    return add_entry(ConstTag::FIELDREF, field);
}

int ConstantPool::find(const std::string &val, ConstTag tag) const {
    std::string key = make_key(tag, val);
    auto it = lookup_.find(key);
    if (it != lookup_.end()) return it->second;
    return -1;
}

uint32_t ConstantPool::size_bytes() const {
    uint32_t sz = 0;
    for (auto &e : pool_) {
        sz += 1; // tag
        sz += 4; // index (for alignment / reference)
        switch (e.tag) {
            case ConstTag::INT:
                sz += 4;
                break;
            case ConstTag::FLOAT:
                sz += 4;
                break;
            case ConstTag::STRING:
            case ConstTag::CLASSREF:
            case ConstTag::METHODREF:
            case ConstTag::FIELDREF:
                sz += 4; // length
                sz += (uint32_t)e.value.size();
                break;
        }
    }
    return sz;
}

void ConstantPool::emit(std::vector<uint8_t> &buf) const {
    auto write_u8 = [&](uint8_t v) { buf.push_back(v); };
    auto write_u32 = [&](uint32_t v) {
        buf.push_back((uint8_t)(v & 0xFF));
        buf.push_back((uint8_t)((v >> 8) & 0xFF));
        buf.push_back((uint8_t)((v >> 16) & 0xFF));
        buf.push_back((uint8_t)((v >> 24) & 0xFF));
    };

    for (auto &e : pool_) {
        write_u8((uint8_t)e.tag);
        write_u32(e.index);
        switch (e.tag) {
            case ConstTag::INT: {
                int32_t v = std::stoi(e.value);
                write_u32((uint32_t)v);
                break;
            }
            case ConstTag::FLOAT: {
                uint32_t bits;
                sscanf(e.value.c_str(), "%x", &bits);
                write_u32(bits);
                break;
            }
            case ConstTag::STRING:
            case ConstTag::CLASSREF:
            case ConstTag::METHODREF:
            case ConstTag::FIELDREF: {
                uint32_t len = (uint32_t)e.value.size();
                write_u32(len);
                buf.insert(buf.end(), e.value.begin(), e.value.end());
                break;
            }
        }
    }
}
