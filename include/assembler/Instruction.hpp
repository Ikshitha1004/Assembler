/*----------------------------------------------------------------------------------------
    This module was written by Ikshitha (CS22B027)
    Updated by Sahiti for Constant Pool integration(Module 4)
-------------------------------------------------------------------------------------------*/

#ifndef ASSEMBLER_Instruction_hpp
#define ASSEMBLER_Instruction_hpp

#include <string>
#include <vector>
#include <cstdint>

enum class OpCode : uint8_t {
    // Arithmetic
    IADD = 0x01,
    ISUB = 0x02,
    IMUL = 0x03,
    IDIV = 0x04,
    INEG = 0x05,

    // Stack
    PUSH = 0x10,
    POP  = 0x11,
    DUP  = 0x12,

    // Memory
    LOAD  = 0x20,
    STORE = 0x21,

    // Control flow
    JMP  = 0x30,
    JZ   = 0x31,
    JNZ  = 0x32,
    CALL = 0x33,
    RET  = 0x34,

    // Comparison
    ICMP_EQ = 0x40,
    ICMP_LT = 0x41,
    ICMP_GT = 0x42,

    // OO
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

    // Possible payloads
    int reg = -1;                     // for Register
    int imm = 0;                      // for Immediate
    int pool_index = -1;              // for ConstPoolIndex
    std::string label;                // for Label
    struct { std::string clazz, name, desc; } fieldref; // for FieldRef
};

struct Instruction {
    OpCode op {OpCode::INVALID};
    std::vector<Operand> operands; 
    int src_line {0};
    int src_col {0};
};

std::string opcode_to_string(OpCode oc);
OpCode mnemonic_to_opcode(const std::string &m);

#endif // ASSEMBLER_Instruction_hpp
