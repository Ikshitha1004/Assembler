#include "assembler/Linker.hpp"
#include "assembler/EmitterObj.hpp" // just to keep headers consistent (not required)
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <cstring>

// helper read
static uint32_t read_u32(std::ifstream &in) {
    uint32_t v;
    in.read(reinterpret_cast<char*>(&v), sizeof(v));
    if (!in) throw std::runtime_error("Unexpected EOF reading u32");
    return v;
}

static std::string read_string_with_len(std::ifstream &in) {
    uint32_t len = read_u32(in);
    std::string s;
    if (len) {
        s.resize(len);
        in.read(&s[0], len);
        if (!in) throw std::runtime_error("Unexpected EOF reading string");
    }
    return s;
}

void Linker::addObjectFile(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open object: " + path);

    uint32_t magic = read_u32(in);
    uint32_t version = read_u32(in);
    if (version != 1) throw std::runtime_error("Unsupported object version");

    Module mod;
    uint32_t codeSize = read_u32(in);
    uint32_t symtabOffset = read_u32(in);
    uint32_t symtabSize = read_u32(in);
    uint32_t relocOffset = read_u32(in);
    uint32_t relocCount = read_u32(in);

    // read code without gaps
    if (codeSize) {
        mod.code.resize(codeSize);
        in.read(reinterpret_cast<char*>(mod.code.data()), codeSize);
    }

    // read symbol table
    if(symtabOffset){
    in.seekg(symtabOffset);
    uint32_t labelCount = read_u32(in);
    for (uint32_t i = 0; i < labelCount; ++i) {
        std::string name = read_string_with_len(in);
        uint32_t addr = read_u32(in);
        (void)read_u32(in); // line
        (void)read_u32(in); // col
        (void)read_u32(in); // placeholder
        mod.labels[name] = addr;
    }


    uint32_t methodCount = read_u32(in);
    for (uint32_t i = 0; i < methodCount; ++i) {
        std::string key = read_string_with_len(in);
        std::string shortname = read_string_with_len(in);
        std::string sig = read_string_with_len(in);
        uint32_t addr = read_u32(in);
        uint32_t size = read_u32(in);
        (void)read_u32(in); // stack
        (void)read_u32(in); // locals
        mod.methods[key] = addr;
        (void)shortname; (void)sig; (void)size;
    }
}

    // read relocations
    if(relocOffset) {
    in.seekg(relocOffset);
    uint32_t reloCountFromFile = read_u32(in); 
    for (uint32_t i = 0; i < reloCountFromFile; ++i) {
        RelocationEntry r;
        r.offset = read_u32(in);
        r.symbol_name = read_string_with_len(in);
        uint32_t sec = read_u32(in);
        r.section = static_cast<Section>(sec);
        mod.relocs.push_back(r);
    }
}

    mod.filename = path;
    modules_.push_back(std::move(mod));
    std::cout << "[Linker] Loaded module " << path << ", code=" << codeSize
              << " labels=" << mod.labels.size() << " methods=" << mod.methods.size()
              << " relocs=" << mod.relocs.size() << "\n";
}


void Linker::assignBaseAddresses() {
    uint32_t cur = 0;
    for (auto &m : modules_) {
        m.base_addr = cur;
        cur += static_cast<uint32_t>(m.code.size());  // contiguous, no gaps
    }
}

void Linker::buildGlobalSymbolTable() {
    for (const auto &m : modules_) {
        for (const auto &kv : m.labels) {
            uint32_t absAddr = m.base_addr + kv.second;
            if (global_symbols_.count(kv.first))
                throw std::runtime_error("Duplicate symbol: " + kv.first);
            global_symbols_[kv.first] = absAddr;
        }
        for (const auto &kv : m.methods) {
            uint32_t absAddr = m.base_addr + kv.second;
            if (global_symbols_.count(kv.first))
                throw std::runtime_error("Duplicate symbol: " + kv.first);
            global_symbols_[kv.first] = absAddr;
        }
    }
}

static void patch_bytes(std::vector<uint8_t> &code, size_t offset, uint32_t value, size_t bytes) {
    if (offset + bytes > code.size()) throw std::runtime_error("Patch out of bounds");
    for (size_t i = 0; i < bytes; ++i) {
        code[offset + i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
    }
}

void Linker::applyRelocations() {
    for (auto &m : modules_) {
        for (const auto &r : m.relocs) {
            auto it = global_symbols_.find(r.symbol_name);
            if (it == global_symbols_.end())
                throw std::runtime_error("Unresolved symbol: " + r.symbol_name);
            uint32_t target = it->second;
            uint32_t operandOffset = r.offset;

            if (operandOffset == 0 || operandOffset - 1 >= m.code.size())
                throw std::runtime_error("Relocation offset invalid");

            uint8_t opcode = m.code[operandOffset - 1];
            size_t patchBytes = 4;
            if (opcode == 0x30 || opcode == 0x31 || opcode == 0x32) patchBytes = 2;

            patch_bytes(m.code, operandOffset, target, patchBytes);
        }
    }
}

// --- Updated mergeModules to remove gaps and ensure sequential offsets ---
void Linker::mergeModules(std::vector<uint8_t>& outcode) {
    for (auto &m : modules_) {
        // Only copy actual code bytes, no gaps
        outcode.insert(outcode.end(), m.code.begin(), m.code.end());
    }
}

void Linker::writeFinalVM(const std::string& outPath, const std::vector<uint8_t>& finalCode, uint32_t entryPoint) {
    std::ofstream out(outPath, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open final VM file");

    uint8_t header[44] = {0};
    header[0] = 0x56; header[1] = 0x4D; header[2] = 0x00; header[3] = 0x01;
    uint32_t version = 1;
    std::memcpy(header + 4, &version, 4);
    std::memcpy(header + 8, &entryPoint, 4);
    uint32_t constPoolOffset = 44;
    std::memcpy(header + 12, &constPoolOffset, 4);
    uint32_t cpSize = 0;
    std::memcpy(header + 16, &cpSize, 4);
    uint32_t codeOffset = 44;
    std::memcpy(header + 20, &codeOffset, 4);
    uint32_t codeSize = static_cast<uint32_t>(finalCode.size());
    std::memcpy(header + 24, &codeSize, 4);
    uint32_t globalsOffset = codeOffset + codeSize;
    std::memcpy(header + 28, &globalsOffset, 4);
    uint32_t globalsSize = 0;
    std::memcpy(header + 32, &globalsSize, 4);
    uint32_t classMetaOffset = globalsOffset;
    std::memcpy(header + 36, &classMetaOffset, 4);
    uint32_t classMetaSize = 0;
    std::memcpy(header + 40, &classMetaSize, 4);

    out.write(reinterpret_cast<char*>(header), sizeof(header));
    if (!finalCode.empty()) out.write(reinterpret_cast<const char*>(finalCode.data()), finalCode.size());
    out.close();
    std::cout << "[Linker] Wrote final VM file: " << outPath << " size=" << finalCode.size() << "\n";
}

void Linker::link(const std::string& outputVmPath) {
    if (modules_.empty()) throw std::runtime_error("No input modules");

    assignBaseAddresses();
    buildGlobalSymbolTable();
    applyRelocations();

    std::vector<uint8_t> finalCode;
    mergeModules(finalCode);

    uint32_t entryPoint = 0;
    auto it = global_symbols_.find("main");
    if (it != global_symbols_.end()) entryPoint = it->second;

    writeFinalVM(outputVmPath, finalCode, entryPoint);
}
