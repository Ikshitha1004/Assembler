/*----------------------------------------------------------------------------------------
    This module was written by Dakshayani
-------------------------------------------------------------------------------------------*/
#include "assembler/Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

std::string read_file(const std::string &path) {
    std::ifstream in(path);
    if (!in) return "";
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// NEW: implement to_uppercopy here
std::string to_uppercopy(const std::string &s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::toupper);
    return out;
}

void print_tokens(const std::vector<Token> &toks) {
    for (auto &t : toks) {
        std::cout << "Token(";
        switch (t.type) {
            case TokenType::MNEMONIC:    std::cout << "MNEMONIC"; break;
            case TokenType::NUMBER:      std::cout << "NUMBER"; break;
            case TokenType::IDENT:       std::cout << "IDENT"; break;
            case TokenType::LABEL_DEF:   std::cout << "LABEL_DEF"; break;
            case TokenType::COMMENT:     std::cout << "COMMENT"; break;
            case TokenType::COMMA:       std::cout << "COMMA"; break;
            case TokenType::END_OF_FILE: std::cout << "EOF"; break;
        }
        std::cout << ", \"" << t.value << "\" @"
                  << t.line << ":" << t.col << ")\n";
    }
}

void print_instructions(const std::vector<Instruction> &instrs) {
    for (size_t i = 0; i < instrs.size(); i++) {
        const auto &ins = instrs[i];
        std::cout << i << ": " << opcode_to_string(ins.op);
        for (auto &op : ins.operands)
            std::cout << " " << op;
        std::cout << "   (src line " << ins.src_line << ")\n";
    }
}
