#include <iostream>
#include "assembler/Tokenizer.hpp"
#include "assembler/Parser.hpp"
#include "assembler/Utils.hpp"

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

    // Show labels
    std::cout << "\n=== LABELS ===\n";
    for (auto &kv : parser.labels()) {
        std::cout << kv.first << " -> " << kv.second << "\n";
    }

    // Report errors if any
    if (!parser.errors().empty()) {
        std::cerr << "\n=== ERRORS ===\n";
        for (auto &err : parser.errors())
            std::cerr << err << "\n";
        return 3;
    }

    return 0;
}
