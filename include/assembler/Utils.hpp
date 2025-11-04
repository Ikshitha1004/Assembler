// ============================================================================
// Developed By: Sahiti Vempalli
// ============================================================================
#ifndef ASSEMBLER_UTILS_HPP
#define ASSEMBLER_UTILS_HPP

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

inline std::size_t instruction_size(const OpCode op) {
    switch (op) {
        // --- No operand (1B total) ---
        case OpCode::IADD: case OpCode::ISUB: case OpCode::IMUL:
        case OpCode::IDIV: case OpCode::INEG:
        case OpCode::FADD: case OpCode::FSUB: case OpCode::FMUL:
        case OpCode::FDIV: case OpCode::FNEG:
        case OpCode::IMOD:
        case OpCode::ALOAD: case OpCode::ASTORE:
        case OpCode::POP:  case OpCode::FPOP: case OpCode::DUP:
        case OpCode::RET:
        case OpCode::ICMP_EQ: case OpCode::ICMP_LT: case OpCode::ICMP_GT:
        case OpCode::ICMP_GEQ: case OpCode::ICMP_NEQ: case OpCode::ICMP_LEQ:
        case OpCode::FCMP_EQ: case OpCode::FCMP_LT: case OpCode::FCMP_GT:
        case OpCode::FCMP_GEQ: case OpCode::FCMP_NEQ: case OpCode::FCMP_LEQ:
            return 1;

        // --- 1B operand (opcode + 8-bit operand) ---
        case OpCode::LOAD_ARG:
        case OpCode::NEW: case OpCode::GETFIELD: case OpCode::PUTFIELD:
        case OpCode::INVOKEVIRTUAL: case OpCode::INVOKESPECIAL:
        case OpCode::SYS_CALL: case OpCode::NEWARRAY:
            return 1 + 1;

        // --- 2B operand (opcode + 16-bit offset) ---
        case OpCode::JMP:
        case OpCode::JZ:
        case OpCode::JNZ:
            return 1 + 2;

        // --- 4B operand (opcode + 32-bit operand) ---
        case OpCode::PUSH: case OpCode::FPUSH:
        case OpCode::LOAD: case OpCode::STORE: 
        return 1 + 4;
        case OpCode::CALL:
            return 1 + 4 + 4;

        // --- Default fallback ---
        default:
            return 1;
    }
}



#endif // ASSEMBLER_UTILS_HPP