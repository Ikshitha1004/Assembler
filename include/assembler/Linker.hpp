#ifndef ASSEMBLER_LINKER_HPP
#define ASSEMBLER_LINKER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "assembler/SymbolTable.hpp"
#include "assembler/Instruction.hpp"
#include "assembler/Utils.hpp"
struct Module {
    std::string filename;
    std::vector<uint8_t> code;
    std::unordered_map<std::string, uint32_t> labels; // local label -> address (absolute in module)
    std::unordered_map<std::string, uint32_t> methods; // method_key -> address
    std::vector<RelocationEntry> relocs;
    uint32_t base_addr = 0;
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

    void assignBaseAddresses();
    void buildGlobalSymbolTable();
    void applyRelocations();
    void mergeModules(std::vector<uint8_t>& outcode);
    void writeFinalVM(const std::string& outPath, const std::vector<uint8_t>& finalCode, uint32_t entryPoint);
};

#endif // ASSEMBLER_LINKER_HPP
