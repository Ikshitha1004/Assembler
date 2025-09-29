#include "assembler/Instruction.hpp"
#include <unordered_map>

OpCode mnemonic_to_opcode(const std::string &m) {
    static const std::unordered_map<std::string, OpCode> map = {
        // Int arithmetic
        {"IADD", OpCode::IADD}, {"ISUB", OpCode::ISUB}, {"IMUL", OpCode::IMUL},
        {"IDIV", OpCode::IDIV}, {"INEG", OpCode::INEG},
        // Float arithmetic
        {"FADD", OpCode::FADD}, {"FSUB", OpCode::FSUB}, {"FMUL", OpCode::FMUL},
        {"FDIV", OpCode::FDIV}, {"FNEG", OpCode::FNEG},
        // Stack
        {"PUSH", OpCode::PUSH}, {"POP", OpCode::POP}, {"DUP", OpCode::DUP},
        {"FPOP", OpCode::FPOP}, {"FPUSH", OpCode::FPUSH},
        // Memory
        {"LOAD", OpCode::LOAD}, {"STORE", OpCode::STORE}, {"LOAD_ARG", OpCode::LOAD_ARG},
        // Control flow
        {"JMP", OpCode::JMP}, {"JZ", OpCode::JZ}, {"JNZ", OpCode::JNZ},
        {"CALL", OpCode::CALL}, {"RET", OpCode::RET},
        // Int comparisons
        {"ICMP_EQ", OpCode::ICMP_EQ}, {"ICMP_LT", OpCode::ICMP_LT}, {"ICMP_GT", OpCode::ICMP_GT},
        {"ICMP_GEQ", OpCode::ICMP_GEQ}, {"ICMP_NEQ", OpCode::ICMP_NEQ}, {"ICMP_LEQ", OpCode::ICMP_LEQ},
        // Float comparisons
        {"FCMP_EQ", OpCode::FCMP_EQ}, {"FCMP_LT", OpCode::FCMP_LT}, {"FCMP_GT", OpCode::FCMP_GT},
        {"FCMP_GEQ", OpCode::FCMP_GEQ}, {"FCMP_NEQ", OpCode::FCMP_NEQ}, {"FCMP_LEQ", OpCode::FCMP_LEQ},
        // OO
        {"NEW", OpCode::NEW}, {"GETFIELD", OpCode::GETFIELD}, {"PUTFIELD", OpCode::PUTFIELD},
        {"INVOKEVIRTUAL", OpCode::INVOKEVIRTUAL}, {"INVOKESPECIAL", OpCode::INVOKESPECIAL}
    };
    auto it = map.find(m);
    return it != map.end() ? it->second : OpCode::INVALID;
}

std::string opcode_to_string(OpCode oc) {
    switch (oc) {
        // Int arithmetic
        case OpCode::IADD: return "IADD";
        case OpCode::ISUB: return "ISUB";
        case OpCode::IMUL: return "IMUL";
        case OpCode::IDIV: return "IDIV";
        case OpCode::INEG: return "INEG";
        // Float arithmetic
        case OpCode::FADD: return "FADD";
        case OpCode::FSUB: return "FSUB";
        case OpCode::FMUL: return "FMUL";
        case OpCode::FDIV: return "FDIV";
        case OpCode::FNEG: return "FNEG";
        // Stack
        case OpCode::PUSH: return "PUSH";
        case OpCode::POP: return "POP";
        case OpCode::DUP: return "DUP";
        case OpCode::FPOP: return "FPOP";
        case OpCode::FPUSH: return "FPUSH";
        // Memory
        case OpCode::LOAD: return "LOAD";
        case OpCode::STORE: return "STORE";
        case OpCode::LOAD_ARG: return "LOAD_ARG";
        // Control flow
        case OpCode::JMP: return "JMP";
        case OpCode::JZ: return "JZ";
        case OpCode::JNZ: return "JNZ";
        case OpCode::CALL: return "CALL";
        case OpCode::RET: return "RET";
        // Int comparisons
        case OpCode::ICMP_EQ: return "ICMP_EQ";
        case OpCode::ICMP_LT: return "ICMP_LT";
        case OpCode::ICMP_GT: return "ICMP_GT";
        case OpCode::ICMP_GEQ: return "ICMP_GEQ";
        case OpCode::ICMP_NEQ: return "ICMP_NEQ";
        case OpCode::ICMP_LEQ: return "ICMP_LEQ";
        // Float comparisons
        case OpCode::FCMP_EQ: return "FCMP_EQ";
        case OpCode::FCMP_LT: return "FCMP_LT";
        case OpCode::FCMP_GT: return "FCMP_GT";
        case OpCode::FCMP_GEQ: return "FCMP_GEQ";
        case OpCode::FCMP_NEQ: return "FCMP_NEQ";
        case OpCode::FCMP_LEQ: return "FCMP_LEQ";
        // OO
        case OpCode::NEW: return "NEW";
        case OpCode::GETFIELD: return "GETFIELD";
        case OpCode::PUTFIELD: return "PUTFIELD";
        case OpCode::INVOKEVIRTUAL: return "INVOKEVIRTUAL";
        case OpCode::INVOKESPECIAL: return "INVOKESPECIAL";
        default: return "INVALID";
    }
}
