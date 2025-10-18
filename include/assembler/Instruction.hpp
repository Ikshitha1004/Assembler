#ifndef ASSEMBLER_INSTRUCTION_HPP
#define ASSEMBLER_INSTRUCTION_HPP

#include <string>
#include <vector>
#include <cstdint>

enum class OpCode : uint8_t {
    // Integer arithmetic
    IADD = 0x01,
    ISUB = 0x02,
    IMUL = 0x03,
    IDIV = 0x04,
    INEG = 0x05,

    // Floating point arithmetic
    FADD = 0x06,
    FSUB = 0x07,
    FMUL = 0x08,
    FDIV = 0x09,
    FNEG = 0x0A,

    // Stack ops
    PUSH = 0x10,
    POP = 0x11,
    DUP = 0x12,
    FPOP = 0x13,
    FPUSH = 0x14,

    // Memory / locals
    LOAD = 0x20,
    STORE = 0x21,
    LOAD_ARG = 0x22,

    // Control flow
    JMP = 0x30,
    JZ = 0x31,
    JNZ = 0x32,
    CALL = 0x33,
    RET = 0x34,

    // Integer comparisons
    ICMP_EQ = 0x40,
    ICMP_LT = 0x41,
    ICMP_GT = 0x42,
    ICMP_GEQ = 0x46,
    ICMP_NEQ = 0x47,
    ICMP_LEQ = 0x48,

    // Float comparisons
    FCMP_EQ = 0x43,
    FCMP_LT = 0x44,
    FCMP_GT = 0x45,
    FCMP_GEQ = 0x49,
    FCMP_NEQ = 0x4A,
    FCMP_LEQ = 0x4B,

    // Object / Constant pool operations
    NEW           = 0x50,
    GETFIELD      = 0x51,
    PUTFIELD      = 0x52,
    INVOKEVIRTUAL = 0x53,
    INVOKESPECIAL = 0x54,

    // System and array ops
    SYS_CALL = 0x60,
    NEWARRAY = 0x70,
    INVALID = 0xFF

};
enum class Syscall : uint8_t
{
    OPEN = 0x01,
    READ = 0x02,
    SBRK = 0x03,
    CLOSE = 0x04,
    FSTAT = 0x05,
    LSEEK = 0x06,
    WRITE = 0x07,
    GETPID = 0x09,
    EXIT = 0x0A,
    TIME = 0x0B,
    STAT = 0x0C,
    SYSTEM = 0x0D,
    GETCWD = 0x0E,
    CHDIR = 0x0F,
    RENAME = 0x10,
    UNLINK = 0x11,
    MKDIR = 0x12,
    ISATTY = 0x13,
};
enum class FieldType : uint8_t {
    INT = 1,
    FLOAT = 2,
    OBJECT = 3
    //add after compiler support-char..
};

struct Value {
    int32_t intValue;
    float floatValue;
    bool isFloat;

    Value() : intValue(0), isFloat(false) {}
    Value(int v) : intValue(v), isFloat(false) {}
    Value(float v) : floatValue(v), isFloat(true) {}
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
    Value val;
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
int mnemonic_to_syscall(const std::string &name);
std::string syscall_to_mnemonic(uint8_t code);


#endif // ASSEMBLER_INSTRUCTION_HPP