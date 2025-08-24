// ============================================================================
// Developed By: Sahiti Vempalli
// ============================================================================
#ifndef ASSEMBLER_Utils_hpp
#define ASSEMBLER_Utils_hpp

#include <string>
#include <vector>
#include "assembler/Token.hpp"
#include "assembler/Instruction.hpp"

std::string read_file(const std::string &path);

std::string to_uppercopy(const std::string &s);

void print_tokens(const std::vector<Token> &toks);
void print_instructions(const std::vector<Instruction> &instrs);

#endif // ASSEMBLER_Utils_hpp
