#ifndef ASSEMBLER_Instruction_hpp
#define ASSEMBLER_Instruction_hpp

#include <string>
#include <vector>
#include <cstdint>

enum class OpCode : uint8_t {
    // Int arithmetic
    IADD = 0x01, ISUB = 0x02, IMUL = 0x03, IDIV = 0x04, INEG = 0x05,

    // Float arithmetic
    FADD = 0x06, FSUB = 0x07, FMUL = 0x08, FDIV = 0x09, FNEG = 0x0A,

    // Stack
    PUSH = 0x10, POP = 0x11, DUP = 0x12,
    FPOP = 0x13, FPUSH = 0x14,

    // Memory
    LOAD = 0x20, STORE = 0x21, LOAD_ARG = 0x22,

    // Control flow
    JMP = 0x30, JZ = 0x31, JNZ = 0x32,
    CALL = 0x33, RET = 0x34,

    // Int comparisons
    ICMP_EQ = 0x40, ICMP_LT = 0x41, ICMP_GT = 0x42,
    ICMP_GEQ = 0x46, ICMP_NEQ = 0x47, ICMP_LEQ = 0x48,

    // Float comparisons
    FCMP_EQ = 0x43, FCMP_LT = 0x44, FCMP_GT = 0x45,
    FCMP_GEQ = 0x49, FCMP_NEQ = 0x4A, FCMP_LEQ = 0x4B,

    // OO / Constant pool refs
    NEW           = 0x50,
    GETFIELD      = 0x51,
    PUTFIELD      = 0x52,
    INVOKEVIRTUAL = 0x53,
    INVOKESPECIAL = 0x54,

    INVALID = 0xFF
};

struct Operand {
    enum class Kind {
        Register,
        Immediate,
        Label,
        FieldRef,
        MethodRef,
        ConstPoolIndex
    };

    Kind kind {Kind::Immediate};
    int reg = -1;
    int imm = 0;
    int pool_index = -1;
    std::string label;
    struct { std::string clazz, name, desc; } fieldref;
};

struct Instruction {
    OpCode op {OpCode::INVALID};
    std::vector<Operand> operands;
    int src_line {0};
    int src_col {0};
};

std::string opcode_to_string(OpCode oc);
OpCode mnemonic_to_opcode(const std::string &m);

inline std::size_t encoded_size(const Instruction& ins) {
    switch (ins.op) {
        // no operand
        case OpCode::IADD: case OpCode::ISUB: case OpCode::IMUL:
        case OpCode::IDIV: case OpCode::INEG:
        case OpCode::FADD: case OpCode::FSUB: case OpCode::FMUL:
        case OpCode::FDIV: case OpCode::FNEG:
        case OpCode::POP:  case OpCode::DUP:
        case OpCode::FPOP:
        case OpCode::RET:
        case OpCode::ICMP_EQ: case OpCode::ICMP_LT: case OpCode::ICMP_GT:
        case OpCode::ICMP_GEQ: case OpCode::ICMP_NEQ: case OpCode::ICMP_LEQ:
        case OpCode::FCMP_EQ: case OpCode::FCMP_LT: case OpCode::FCMP_GT:
        case OpCode::FCMP_GEQ: case OpCode::FCMP_NEQ: case OpCode::FCMP_LEQ:
            return 1;

        // has 4-byte operand
        case OpCode::PUSH: case OpCode::FPUSH:
        case OpCode::LOAD: case OpCode::STORE: case OpCode::LOAD_ARG:
        case OpCode::JMP: case OpCode::JZ: case OpCode::JNZ: case OpCode::CALL:
        case OpCode::NEW: case OpCode::GETFIELD: case OpCode::PUTFIELD:
        case OpCode::INVOKEVIRTUAL: case OpCode::INVOKESPECIAL:
            return 1 + 4;

        default:
            return 1;
    }
}

#endif // ASSEMBLER_Instruction_hpp
