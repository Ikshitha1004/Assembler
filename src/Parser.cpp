// ============================================================================
// Parser.cpp - Developed by Ikshitha,Sahiti
// ============================================================================

#include "assembler/Parser.hpp"
#include "assembler/Utils.hpp"          // to_uppercopy
#include <cctype>
#include <sstream>
#include <utility> // for std::move

static uint32_t instr_size_bytes(const Instruction& ins) {
    switch (ins.op) {
        // 0-operand ops: 1 byte opcode
        case OpCode::IADD: case OpCode::ISUB: case OpCode::IMUL:
        case OpCode::IDIV: case OpCode::INEG:
        case OpCode::POP:  case OpCode::DUP:
        case OpCode::RET:
        case OpCode::ICMP_EQ: case OpCode::ICMP_LT: case OpCode::ICMP_GT:
            return 1;

        // 1-operand ops: opcode + 4-byte operand = 5 bytes
        case OpCode::PUSH:
        case OpCode::LOAD: case OpCode::STORE:
        case OpCode::JMP: case OpCode::JZ: case OpCode::JNZ: case OpCode::CALL:
        case OpCode::NEW:
        case OpCode::GETFIELD: case OpCode::PUTFIELD:
        case OpCode::INVOKEVIRTUAL: case OpCode::INVOKESPECIAL:
            return 5;

        default:
            return 1;
    }
}

Parser::Parser(const std::vector<Token>& t)
    : toks(t), idx(0) {}

const Token& Parser::cur() const {
    if (idx < toks.size()) return toks[idx];
    return toks.back();
}

void Parser::advance() {
    if (idx < toks.size()) ++idx;
}

bool Parser::accept(TokenType t) {
    if (cur().type == t) { advance(); return true; }
    return false;
}

bool Parser::expect(TokenType t) {
    if (accept(t)) return true;
    std::ostringstream os;
    os << "Parse error: expected token at line " << cur().line
       << ", col " << cur().col;
    errlist.push_back(os.str());
    return false;
}

void Parser::parse_operands(Instruction &ins) {
    while (cur().type == TokenType::NUMBER || cur().type == TokenType::IDENT) {
        ins.operands.push_back(cur().value);
        advance();
        if (cur().type == TokenType::COMMA) {
            advance();
        } else {
            break;
        }
    }
}

static bool is_number_literal(const std::string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

/*----------------------------------------------------------------------------------------
This function was written by Dakshayani
-----------------------------------------------------------------------------------------*/
void Parser::validate_instruction(const Instruction &ins) {
    auto bad = [&](const std::string &msg) {
        std::ostringstream os;
        os << "Validation error at " << ins.src_line << ":" << ins.src_col
           << " -> " << msg;
        errlist.push_back(os.str());
    };

    switch (ins.op) {
        case OpCode::PUSH:
        case OpCode::LOAD:
        case OpCode::STORE:
        case OpCode::JMP:
        case OpCode::JZ:
        case OpCode::JNZ:
        case OpCode::CALL:
        case OpCode::NEW:
        case OpCode::GETFIELD:
        case OpCode::PUTFIELD:
        case OpCode::INVOKEVIRTUAL:
        case OpCode::INVOKESPECIAL:
            if (ins.operands.size() != 1)
                bad("Instruction requires exactly 1 operand");
            break;

        case OpCode::POP:
        case OpCode::DUP:
        case OpCode::IADD: case OpCode::ISUB:
        case OpCode::IMUL: case OpCode::IDIV: case OpCode::INEG:
        case OpCode::ICMP_EQ: case OpCode::ICMP_LT: case OpCode::ICMP_GT:
        case OpCode::RET:
            if (!ins.operands.empty())
                bad("Instruction takes no operands");
            break;

        default:
            if (ins.op == OpCode::INVALID)
                bad("Invalid mnemonic");
            break;
    }
}

/*----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------*/
void Parser::parse_line() {
    if (cur().type == TokenType::COMMENT) { advance(); return; }

    if (cur().type == TokenType::LABEL_DEF) {
        const std::string lab = cur().value;
        int l = cur().line, c = cur().col;
        if (!symtab.define_label(lab, l, c)) {
            std::ostringstream os;
            os << "Duplicate label '" << lab << "' at " << l << ":" << c;
            errlist.push_back(os.str());
        }
        advance();
        
    }

    // If end or comment, done
    if (cur().type == TokenType::END_OF_FILE || cur().type == TokenType::COMMENT) return;

    // Instruction?
    if (cur().type == TokenType::MNEMONIC) {
        std::string m = to_uppercopy(cur().value);
        Instruction ins;
        ins.op = mnemonic_to_opcode(m);
        ins.src_line = cur().line;
        ins.src_col  = cur().col;

        advance();               
        parse_operands(ins);    

        validate_instruction(ins);

        if (ins.operands.size() == 1) {
            switch (ins.op) {
                case OpCode::JMP:
                case OpCode::JZ:
                case OpCode::JNZ:
                case OpCode::CALL: {
                    const std::string &opnd = ins.operands[0];
                    if (!is_number_literal(opnd)) {
                        // record reference to be resolved after pass 1
                        symtab.add_label_reference(
                            instrs.size(),  // this instruction index
                            0,              // operand index
                            opnd,
                            ins.src_line,
                            ins.src_col
                        );
                    }
                    break;
                }
                default:
                    break;
            }
        }

        // Append instruction then advance LC in bytes
        instrs.push_back(std::move(ins));
        const Instruction& just = instrs.back();
        symtab.advance_lc(instr_size_bytes(just));

        return;
    }

    // Any stray identifiers / numbers here are unexpected at line start
    if (cur().type == TokenType::IDENT || cur().type == TokenType::NUMBER) {
        std::ostringstream os;
        os << "Unexpected token '" << cur().value << "' at "
           << cur().line << ":" << cur().col;
        errlist.push_back(os.str());
        advance();
        return;
    }

    advance();
}

/*----------------------------------------------------------------------------------------
    Main parse (single pass + resolve forward label refs)
-----------------------------------------------------------------------------------------*/
std::vector<Instruction> Parser::parse() {
    // reset state (keep base address)
    idx = 0;
    instrs.clear();
    errlist.clear();

    // Clear symbol table but preserve base; easiest is reassign:
    {
        uint32_t base = symtab.base();
        symtab = SymbolTable(base);
        symtab.reset_lc();
    }

    // pass 1: read tokens into IR and collect labels/refs
    while (cur().type != TokenType::END_OF_FILE) {
        parse_line();
    }

    // pass 2: resolve pending label references
    const auto& refs = symtab.pending_refs();
    for (const auto& r : refs) {
        if (r.instr_index >= instrs.size()) {
            std::ostringstream os;
            os << "Internal error: bad reference index " << r.instr_index;
            errlist.push_back(os.str());
            continue;
        }
        Instruction& target_ins = instrs[r.instr_index];

        auto found = symtab.get_label(r.label);
        if (!found.first) {
            std::ostringstream os;
            os << "Undefined label '" << r.label << "' referenced at "
               << r.line << ":" << r.col;
            errlist.push_back(os.str());
            continue;
        }
        // Write absolute address into operand string
        uint32_t addr = found.second.address;
        if (r.operand_index >= target_ins.operands.size()) {
            std::ostringstream os;
            os << "Internal error: operand index OOB on '" << r.label << "'";
            errlist.push_back(os.str());
            continue;
        }
        target_ins.operands[r.operand_index] = std::to_string(addr);
    }

    return instrs;
}

const std::vector<std::string>& Parser::errors() const {
    return errlist;
}
