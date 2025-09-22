#include <iostream>
#include <iomanip>
#include "assembler/Tokenizer.hpp"
#include "assembler/Parser.hpp"
#include "assembler/Utils.hpp"
#include "assembler/SymbolTable.hpp"
#include "assembler/IR.hpp"
#include "assembler/Emitter.hpp"
#include "assembler/ConstantPool.hpp"
using assembler::ConstTag;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: assembler <source.asm>\n";
        return 1;
    }

    // Read source file
    std::string src = read_file(argv[1]);
    if (src.empty()) {
        std::cerr << "Error: could not read file '" << argv[1] << "'\n";
        return 2;
    }

    // Tokenize
    Tokenizer tokenizer(src);
    auto tokens = tokenizer.tokenize();

    std::cout << "=== TOKENS ===\n";
    print_tokens(tokens);

    // Parse
    Parser parser(tokens);
    auto instructions = parser.parse();

    std::cout << "\n=== INSTRUCTIONS ===\n";
    print_instructions(instructions);

    // Show symbol table contents (labels, constants, etc.)
    std::cout << "\n=== SYMBOL TABLE ===\n";
    const SymbolTable& symtab = parser.symbols();

    for (auto &kv : symtab.labels()) {
        std::cout << "Label " << kv.first
                  << " -> addr=" << kv.second.address
                  << " (defined at line " << kv.second.line
                  << ", col " << kv.second.col << ")\n";
    }
    for (auto &kv : symtab.constants()) {
        std::cout << "Const " << kv.first
                  << " = " << kv.second.value << "\n";
    }
    for (auto &kv : symtab.methods()) {
        std::cout << "Method " << kv.first
                  << " addr=" << kv.second.address
                  << " stack=" << kv.second.stack_limit
                  << " locals=" << kv.second.locals_limit
                  << (kv.second.is_entry ? " [ENTRY]" : "")
                  << "\n";
    }
    for (auto &kv : symtab.classes()) {
        std::cout << "Class " << kv.first
                  << " super=" << kv.second.super_name << "\n";
    }

    if (!parser.errors().empty()) {
        std::cerr << "\n=== ERRORS ===\n";
        for (auto &err : parser.errors())
            std::cerr << err << "\n";
        return 3;
    }

    // === Constant Pool Debug Print ===
const auto &cp = parser.get_constpool().entries();
std::cout << "\n=== CONSTANT POOL ===\n";
for (auto &e : cp) {
    std::cout << "#" << e.index << " ";
    switch (e.tag) {
        case ConstTag::INT:      std::cout << "INT "; break;
        case ConstTag::FLOAT:    std::cout << "FLOAT "; break;
        case ConstTag::STRING:   std::cout << "STRING "; break;
    }
    std::cout << e.str<< "\n";
}

    // Build IR
    auto irrep = assembler::IRBuilder::build(instructions);

    // if (!irrep.errors.empty()) {
    //     std::cerr << "\n=== IR BUILD ERRORS ===\n";
    //     for (auto &e : irrep.errors) std::cerr << e << "\n";
    // }

    std::cout << "\n=== IR WORDS ===\n";
    for (size_t i = 0; i < irrep.words.size(); ++i) {
        const auto &w = irrep.words[i];
        std::cout << i << ": opcode=0x" 
          << std::hex << std::setw(2) << std::setfill('0') 
          << (int)w.opcode << std::dec;
        for (auto v : w.imm) std::cout << " " << v;
        std::cout << "   (src line " << w.src_line << ")\n";
    }

    //  Convert IR → raw bytecode
    std::vector<uint8_t> code;
    for (auto &w : irrep.words) {
        code.push_back(w.opcode);
        for (auto imm : w.imm) {
            // store as little-endian 32-bit
            for (int i = 0; i < 4; i++) {
                code.push_back((imm >> (8 * i)) & 0xFF);
            }
        }
    }

  std::vector<uint8_t> pool_bytes;
parser.get_constpool().emit(pool_bytes);


    // final buffer = [constant pool][code]
    std::vector<uint8_t> final_bytes;
    final_bytes.insert(final_bytes.end(), pool_bytes.begin(), pool_bytes.end());
    final_bytes.insert(final_bytes.end(), code.begin(), code.end());

    // Prepare empty class metadata (extend later)
    std::vector<assembler::ClassMeta> classes;
    std::string inputFile = argv[1];
    std::string outFile;

    // replace ".asm" with ".vm" if it exists, else append
    if (inputFile.size() >= 4 && inputFile.substr(inputFile.size() - 4) == ".asm") {
        outFile = inputFile.substr(0, inputFile.size() - 4) + ".vm";
    } else {
        outFile = inputFile + ".vm";
    }

    //  Write VM binary file
    assembler::writeVMFile(outFile, final_bytes, classes, 0);

    std::cout << "\nWrote binary file: " << outFile << "\n";

    return 0;
}
