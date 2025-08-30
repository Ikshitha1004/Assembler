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
        w.opcode = static_cast<uint8_t>(ins.op);
        w.src_line = ins.src_line;
        w.src_col  = ins.src_col;

        for (size_t oi = 0; oi < ins.operands.size(); ++oi) {
            const std::string &opstr = ins.operands[oi];
            int32_t val = 0;
            if (!parse_int32(opstr, val)) {
                std::ostringstream os;
                os << "IR build error: non-numeric operand '" << opstr
                   << "' at line " << ins.src_line << ", col " << ins.src_col
                   << " (instr " << i << ", operand " << oi << ")";
                rep.errors.push_back(os.str());
                continue; 
            }
            w.imm.push_back(val);
        }

        rep.words.push_back(std::move(w));
    }

    return rep;
}

} // namespace assembler