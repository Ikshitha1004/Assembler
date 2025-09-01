#include <iostream>
#include "assembler/Tokenizer.hpp"
#include "assembler/Parser.hpp"
#include "assembler/Utils.hpp"
#include "assembler/SymbolTable.hpp"
#include "assembler/IR.hpp"    

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

    // Labels
    for (auto &kv : symtab.labels()) {
        std::cout << "Label " << kv.first
                  << " -> addr=" << kv.second.address
                  << " (defined at line " << kv.second.line
                  << ", col " << kv.second.col << ")\n";
    }

    // Constants
    for (auto &kv : symtab.constants()) {
        std::cout << "Const " << kv.first
                  << " = " << kv.second.value << "\n";
    }

    // Methods
    for (auto &kv : symtab.methods()) {
        std::cout << "Method " << kv.first
                  << " addr=" << kv.second.address
                  << " stack=" << kv.second.stack_limit
                  << " locals=" << kv.second.locals_limit
                  << (kv.second.is_entry ? " [ENTRY]" : "")
                  << "\n";
    }

    // Classes
    for (auto &kv : symtab.classes()) {
        std::cout << "Class " << kv.first
                  << " super=" << kv.second.super_name << "\n";
    }

    // Report errors if any
    if (!parser.errors().empty()) {
        std::cerr << "\n=== ERRORS ===\n";
        for (auto &err : parser.errors())
            std::cerr << err << "\n";
        return 3;
    }
    // (Future) Resolver stage goes here
    // auto resolved = Resolver::resolve(instructions, parser.symbol_table());

    // For now we directly hand parsed instructions to IRBuilder
    auto irrep = assembler::IRBuilder::build(instructions);

    if (!irrep.errors.empty()) {
        std::cerr << "\n=== IR BUILD ERRORS ===\n";
        for (auto &e : irrep.errors) std::cerr << e << "\n";
    }

    std::cout << "\n=== IR WORDS ===\n";
    for (size_t i = 0; i < irrep.words.size(); ++i) {
        const auto &w = irrep.words[i];
        std::cout << i << ": opcode=0x" << std::hex << (int)w.opcode << std::dec;
        for (auto v : w.imm) std::cout << " " << v;
        std::cout << "   (src line " << w.src_line << ")\n";
    }

    return 0;
}