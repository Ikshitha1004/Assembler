// ============================================================================
// ConstantPool.cpp -Developed by Sahiti(CS22B056)
// ============================================================================

#include "assembler/ConstantPool.hpp"
#include <sstream>
#include <cstring>
#include <iostream>
#include <iomanip>

using namespace assembler;

// --- Helper to build unique keys for deduplication ---
static std::string make_key(ConstTag tag, const std::string &val) {
    return std::to_string(static_cast<int>(tag)) + "|" + val;
}

// --- Primitive constants ---
int ConstantPool::add_int(int32_t v) {
    std::string key = make_key(ConstTag::INT, std::to_string(v));
    auto it = lookup_.find(key);
    if (it != lookup_.end()) return it->second;

    ConstEntry e{ConstTag::INT, next_index_++, std::to_string(v)};
    pool_.push_back(e);
    lookup_[key] = e.index;
    return e.index;
}

int ConstantPool::add_float(float f) {
    uint32_t bits;
    std::memcpy(&bits, &f, sizeof(bits));
    std::ostringstream oss; oss << "0x" << std::hex << bits;
    std::string val = oss.str();

    std::string key = make_key(ConstTag::FLOAT, val);
    auto it = lookup_.find(key);
    if (it != lookup_.end()) return it->second;

    ConstEntry e{ConstTag::FLOAT, next_index_++, val};
    pool_.push_back(e);
    lookup_[key] = e.index;
    return e.index;
}

int ConstantPool::add_string(const std::string &s) {
    std::string key = make_key(ConstTag::STRING, s);
    auto it = lookup_.find(key);
    if (it != lookup_.end()) return it->second;

    ConstEntry e{ConstTag::STRING, next_index_++, s};
    pool_.push_back(e);
    lookup_[key] = e.index;
    return e.index;
}

// --- Size calculation ---
uint32_t ConstantPool::size_bytes() const {
    uint32_t sz = 0;
    for (auto &e : pool_) {
        sz += 1 + 4; // tag + index
        switch (e.tag) {
            case ConstTag::INT:   sz += 4; break;
            case ConstTag::FLOAT: sz += 4; break;
            case ConstTag::STRING: sz += 4 + (uint32_t)e.str.size(); break;
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
            case ConstTag::INT:   write_u32(static_cast<uint32_t>(std::stoi(e.str))); break;
            case ConstTag::FLOAT: {
                uint32_t bits; std::stringstream ss(e.str); ss >> std::hex >> bits;
                write_u32(bits); break;
            }
            case ConstTag::STRING:
                write_u32((uint32_t)e.str.size());
                buf.insert(buf.end(), e.str.begin(), e.str.end());
                break;
        }
    }
}

// --- Print pool ---
void ConstantPool::print() const {
    std::cout << "=== CONSTANT POOL ===\n";
    for (auto &e : pool_) {
        std::cout << "#" << e.index << " ";
        switch (e.tag) {
            case ConstTag::INT:    std::cout << "INT " << e.str; break;
            case ConstTag::FLOAT:  std::cout << "FLOAT " << e.str; break;
            case ConstTag::STRING: std::cout << "STRING \"" << e.str << "\""; break;
        }
        std::cout << "\n";
    }
}
