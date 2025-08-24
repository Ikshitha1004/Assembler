
/*----------------------------------------------------------------------------------------
    This module was written by Ikshitha (CS22B027)
-------------------------------------------------------------------------------------------*/
#include "assembler/Parser.hpp"
#include "assembler/Utils.hpp"
#include <sstream>

Parser::Parser(const std::vector<Token> &toks)
    : toks(toks), idx(0) {}

const Token& Parser::cur() const { return toks[idx]; }
void Parser::advance() { if (idx < toks.size()) ++idx; }
bool Parser::accept(TokenType t) { if (cur().type == t) { advance(); return true; } return false; }
bool Parser::expect(TokenType t) {
    if (accept(t)) return true;
    std::ostringstream os;
    os << "Parse error: expected token at line " << cur().line << ", col " << cur().col;
    errlist.push_back(os.str());
    return false;
}

void Parser::parse_operands(Instruction &ins) {
    while (true) {
        if (cur().type == TokenType::NUMBER || cur().type == TokenType::IDENT) {
            ins.operands.push_back(cur().value);
            advance();
        } else {
            break;
        }
        if (cur().type == TokenType::COMMA) advance();
        else break;
    }
}

void Parser::validate_instruction(const Instruction &ins) {
   //error handlin got be done
}

void Parser::parse_line() {
    if (cur().type == TokenType::COMMENT) {
        advance();
        return;
    }
    if (cur().type == TokenType::LABEL_DEF) {
        std::string lab = cur().value;
        if (label_map.find(lab) != label_map.end()) {
            std::ostringstream os;
            os << "Duplicate label '" << lab << "' at " << cur().line << ":" << cur().col;
            errlist.push_back(os.str());
        } else {
            label_map[lab] = static_cast<int>(instrs.size());
        }
        advance();
        return;
    }
    if (cur().type == TokenType::MNEMONIC) {
        std::string m = cur().value;
        OpCode oc = mnemonic_to_opcode(m);

        Instruction ins;
        ins.op = oc;
        ins.src_line = cur().line;
        ins.src_col = cur().col;

        advance();
        parse_operands(ins);
        validate_instruction(ins);
        instrs.push_back(std::move(ins));
        return;
    }
    if (cur().type == TokenType::IDENT || cur().type == TokenType::NUMBER) {
        std::ostringstream os;
        os << "Unexpected token '" << cur().value << "' at " << cur().line << ":" << cur().col;
        errlist.push_back(os.str());
        advance();
        return;
    }
    advance();
}

std::vector<Instruction> Parser::parse() {
    idx = 0;
    instrs.clear();
    label_map.clear();
    errlist.clear();

    while (cur().type != TokenType::END_OF_FILE) {
        parse_line();
    }

    // Resolve labels
    for (auto &ins : instrs) {
        for (auto &op : ins.operands) {
            if (label_map.find(op) != label_map.end()) {
                op = std::to_string(label_map[op]);
            }
        }
    }

    return instrs;
}

const std::vector<std::string>& Parser::errors() const { return errlist; }
const std::unordered_map<std::string,int>& Parser::labels() const { return label_map; }
