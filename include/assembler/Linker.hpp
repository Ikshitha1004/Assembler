#ifndef ASSEMBLER_LINKER_HPP
#define ASSEMBLER_LINKER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <set>

#include "assembler/SymbolTable.hpp"
#include "assembler/Instruction.hpp"
#include "assembler/Utils.hpp"

struct ClassInfo {
    std::string name;
    std::string super_name;
    uint32_t index = UINT32_MAX;
    uint32_t super_index = UINT32_MAX;
    std::vector<std::pair<std::string, uint32_t>> methods; // methodName -> addr (relative in module until patched)
    std::vector<std::pair<std::string, FieldInfo>> fields;
    int field_count = 0;
};

struct Module; // forward

struct GlobalSymbol {
    uint32_t address;
    Module* module;
};

struct Module {
    std::string filename;
    std::vector<uint8_t> code;
    std::unordered_map<std::string, uint32_t> labels;
    std::unordered_map<std::string, MethodInfo> methods;
    std::vector<RelocationEntry> relocs;
    std::unordered_map<std::string, ClassInfo> classes;

    uint32_t base_addr = 0;
    bool isEntryModule = false;

    // DCE bookkeeping
    std::set<std::string> exports;
    std::set<std::string> imports;
    bool live = false;
};

class Linker {
public:
    Linker() = default;

    void addObjectFile(const std::string& path);
    void link(const std::string& outputVmPath);

private:
    std::vector<Module> modules_;
    std::unordered_map<std::string, GlobalSymbol> global_symbols_;

    std::unordered_map<std::string, uint32_t> class_indices_;
    std::vector<std::string> index_to_class_;
    std::vector<std::string> referenced_classes_;

    // Core passes
    void detectEntryModule();
    void patchModuleSymbols(Module &m);
    void assignBaseAddressesAndOrder(); // Builds graph, marks live, topo order, reorders modules_, assigns base
    size_t preparePatch(const Module &m, uint32_t offset);
    void buildGlobalSymbolTable();
    void applyRelocations();
    void mergeModules(std::vector<uint8_t>& outcode);
    void writeFinalVM(const std::string& outPath, const std::vector<uint8_t>& finalCode, uint32_t entryPoint);

    Module* entryModule_ = nullptr;
};

#endif // ASSEMBLER_LINKER_HPP