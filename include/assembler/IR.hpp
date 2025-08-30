#ifndef ASSEMBLER_IR_hpp
#define ASSEMBLER_IR_hpp

#include <cstdint>
#include <vector>
#include <string>
#include "assembler/Instruction.hpp"

namespace assembler {

// Final numeric-only IR node (after resolution)
struct IRWord {
    uint8_t                 opcode;
    std::vector<int32_t>    imm;   
    int                     src_line = 0; 
    int                     src_col  = 0;
};

class IRBuilder {
public:
    struct Report {
        std::vector<std::string> errors;
        std::vector<IRWord>      words;
    };

    static Report build(const std::vector<Instruction>& program);
};

} // namespace assembler

#endif // ARM_ALL_ASSEMBLER_IR_hpp