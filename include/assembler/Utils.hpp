// ============================================================================
// Developed By: Sahiti Vempalli
// ============================================================================
#ifndef ASSEMBLER_Utils_hpp
#define ASSEMBLER_Utils_hpp

#include <string>
#include <vector>
#include "assembler/Instruction.hpp"
#include "assembler/SymbolTable.hpp"
#include "assembler/Token.hpp"
std::string read_file(const std::string &path);

std::string to_uppercopy(const std::string& s);
void print_tokens(const std::vector<Token> &toks);

void print_instructions(const std::vector<Instruction>& code);
void print_symbol_table(const SymbolTable& symtab);

// byte-size of encoded instruction (1B opcode + optional 4B operand)
inline std::size_t instruction_size(const Instruction& inst) {
    switch (inst.op) {
        case OpCode::IADD: case OpCode::ISUB: case OpCode::IMUL:
        case OpCode::IDIV: case OpCode::INEG:
        case OpCode::POP:  case OpCode::DUP:
        case OpCode::RET:
        case OpCode::ICMP_EQ: case OpCode::ICMP_LT: case OpCode::ICMP_GT:
            return 1;

        case OpCode::PUSH:
        case OpCode::LOAD:
        case OpCode::STORE:
        case OpCode::JMP: case OpCode::JZ: case OpCode::JNZ: case OpCode::CALL:
        case OpCode::NEW: case OpCode::GETFIELD: case OpCode::PUTFIELD:
        case OpCode::INVOKEVIRTUAL: case OpCode::INVOKESPECIAL:
            return 1 + 4;

        default:
            return 1;
    }
}

#endif // ARM_ALL_ASSEMBLER_UTILS_HPP
