/*----------------------------------------------------------------------------------------
    This module was written by Dakshayini (CS21B016)
-------------------------------------------------------------------------------------------*/
#include "assembler/IR.hpp"
#include <cstdlib>
#include <sstream>
#include <cerrno>
#include <climits>

namespace assembler {

static bool parse_int32(const std::string &s, int32_t &out) {
    errno = 0;
    char *endp = nullptr;
    long val = std::strtol(s.c_str(), &endp, 0);
    if (endp == s.c_str() || *endp != '\0') return false;
    if (errno == ERANGE) return false;
    if (val < INT32_MIN || val > INT32_MAX) return false;
    out = static_cast<int32_t>(val);
    return true;
}


IRBuilder::Report IRBuilder::build(const std::vector<Instruction>& program) {
    Report rep;
    rep.words.reserve(program.size());

    for (size_t i = 0; i < program.size(); ++i) {
        const Instruction &ins = program[i];
        IRWord w;
        w.opcode   = static_cast<uint8_t>(ins.op);
        w.src_line = ins.src_line;
        w.src_col  = ins.src_col;

        for (size_t oi = 0; oi < ins.operands.size(); ++oi) {
            const Operand &op = ins.operands[oi];

            switch (op.kind) {
                case Operand::Kind::Immediate:
                    w.imm.push_back(op.imm);
                    break;

                case Operand::Kind::Label: {
                    int32_t val = 0;
                    if (!parse_int32(op.label, val)) {
                        // Label not numeric → placeholder 0
                        val = 0;
                    }
                    w.imm.push_back(val);
                    break;
                }

                case Operand::Kind::ConstPoolIndex:
                    w.imm.push_back(op.pool_index);
                    break;

                case Operand::Kind::MethodRef:
                case Operand::Kind::FieldRef:
                case Operand::Kind::Register:
                    rep.errors.push_back(
                        "IR build error: unsupported operand kind at line " +
                        std::to_string(ins.src_line) + ", col " +
                        std::to_string(ins.src_col) +
                        " (instr " + std::to_string(i) +
                        ", operand " + std::to_string(oi) + ")"
                    );
                    break;
            }
        }

        rep.words.push_back(std::move(w));
    }

    return rep;
}

} // namespace assembler
