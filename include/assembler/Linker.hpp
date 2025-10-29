#ifndef ASSEMBLER_LINKER_HPP
#define ASSEMBLER_LINKER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "assembler/SymbolTable.hpp"
#include "assembler/Instruction.hpp"
#include "assembler/Utils.hpp"


struct ClassInfo {
    std::string name;
    std::string super_name;
    uint32_t index = UINT32_MAX;       // this class’s index
    uint32_t super_index = UINT32_MAX; // index of the superclass
    std::unordered_map<std::string, uint32_t> methods; // methodName -> addr
};

struct Module {
    std::string filename;
    std::vector<uint8_t> code;
    std::unordered_map<std::string, uint32_t> labels;
    std::unordered_map<std::string, MethodInfo> methods;
    std::vector<RelocationEntry> relocs;
    std::unordered_map<std::string, ClassInfo> classes; // NEW
    uint32_t base_addr = 0;
    bool isEntryModule = false;
};

class Linker {
public:
    Linker() = default;

    // Read a .vmobj and add it
    void addObjectFile(const std::string& path);

    // Perform full link and write final VM file
    void link(const std::string& outputVmPath);

private:
    std::vector<Module> modules_;
    // global symbol map: name -> (absolute address)
    std::unordered_map<std::string, uint32_t> global_symbols_;
    std::vector<std::string> referenced_classes_; // NEW
    void assignBaseAddresses();
    void buildGlobalSymbolTable();
    void applyRelocations();
    void mergeModules(std::vector<uint8_t>& outcode);
    Module* entryModule_ = nullptr; 
    void detectEntryModule();
    void writeFinalVM(const std::string& outPath, const std::vector<uint8_t>& finalCode, uint32_t entryPoint);
    std::unordered_map<std::string, uint32_t> class_indices_;  // className -> index
    std::vector<std::string> index_to_class_;  
};

#endif // ASSEMBLER_LINKER_HPP
