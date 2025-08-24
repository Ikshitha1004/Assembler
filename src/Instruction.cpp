/*----------------------------------------------------------------------------------------
    This module was written by Ikshitha (CS22B027)
-------------------------------------------------------------------------------------------*/

#include "assembler/Instruction.hpp"
#include <unordered_map>

OpCode mnemonic_to_opcode(const std::string &m) {
    static const std::unordered_map<std::string, OpCode> map = {
        {"IADD", OpCode::IADD}, {"ISUB", OpCode::ISUB}, {"IMUL", OpCode::IMUL},
        {"IDIV", OpCode::IDIV}, {"INEG", OpCode::INEG},
        {"PUSH", OpCode::PUSH}, {"POP", OpCode::POP}, {"DUP", OpCode::DUP},
        {"LOAD", OpCode::LOAD}, {"STORE", OpCode::STORE},
        {"JMP", OpCode::JMP}, {"JZ", OpCode::JZ}, {"JNZ", OpCode::JNZ},
        {"CALL", OpCode::CALL}, {"RET", OpCode::RET},
        {"ICMP_EQ", OpCode::ICMP_EQ}, {"ICMP_LT", OpCode::ICMP_LT}, {"ICMP_GT", OpCode::ICMP_GT},
        {"NEW", OpCode::NEW}, {"GETFIELD", OpCode::GETFIELD}, {"PUTFIELD", OpCode::PUTFIELD},
        {"INVOKEVIRTUAL", OpCode::INVOKEVIRTUAL}, {"INVOKESPECIAL", OpCode::INVOKESPECIAL}
    };

    auto it = map.find(m);
    return it != map.end() ? it->second : OpCode::INVALID;
}

std::string opcode_to_string(OpCode oc) {
    switch (oc) {
        case OpCode::IADD: return "IADD";
        case OpCode::ISUB: return "ISUB";
        case OpCode::IMUL: return "IMUL";
        case OpCode::IDIV: return "IDIV";
        case OpCode::INEG: return "INEG";
        case OpCode::PUSH: return "PUSH";
        case OpCode::POP: return "POP";
        case OpCode::DUP: return "DUP";
        case OpCode::LOAD: return "LOAD";
        case OpCode::STORE: return "STORE";
        case OpCode::JMP: return "JMP";
        case OpCode::JZ: return "JZ";
        case OpCode::JNZ: return "JNZ";
        case OpCode::CALL: return "CALL";
        case OpCode::RET: return "RET";
        case OpCode::ICMP_EQ: return "ICMP_EQ";
        case OpCode::ICMP_LT: return "ICMP_LT";
        case OpCode::ICMP_GT: return "ICMP_GT";
        case OpCode::NEW: return "NEW";
        case OpCode::GETFIELD: return "GETFIELD";
        case OpCode::PUTFIELD: return "PUTFIELD";
        case OpCode::INVOKEVIRTUAL: return "INVOKEVIRTUAL";
        case OpCode::INVOKESPECIAL: return "INVOKESPECIAL";
        default: return "INVALID";
    }
}
