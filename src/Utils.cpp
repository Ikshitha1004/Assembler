/*----------------------------------------------------------------------------------------
    This module was written by Dakshayani
-------------------------------------------------------------------------------------------*/
#include "assembler/Utils.hpp"
#include "assembler/Token.hpp"
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


//Function to print symbol table contents,added by Sahiti
void print_symbol_table(const SymbolTable& symtab) {
    std::cout << "== Symbol Table ==\n";
    std::cout << "base: " << symtab.base() << ", code LC (bytes): " << symtab.lc() << "\n\n";

    std::cout << "[labels]\n";
    for (const auto& kv : symtab.labels()) {
        std::cout << "  " << kv.first << " => " << kv.second.address
                  << "  (src " << kv.second.line << ":" << kv.second.col << ")\n";
    }

    std::cout << "\n[constants]\n";
    for (const auto& kv : symtab.constants()) {
        std::cout << "  " << kv.first << " = " << kv.second.value << "\n";
    }

    std::cout << "\n[fields]\n";
    for (const auto& kv : symtab.fields()) {
        const auto& f = kv.second;
        std::cout << "  " << f.owner_class << "." << f.name
                  << " : " << f.descriptor
                  << "  pool=" << (f.pool_index == UINT32_MAX ? -1 : (int)f.pool_index)
                  << "\n";
    }

    std::cout << "\n[methods]\n";
    for (const auto& kv : symtab.methods()) {
        const auto& m = kv.second;
        std::cout << "  " << kv.first
                  << " @ " << m.address
                  << "  .limit stack " << m.stack_limit
                  << "  .limit locals " << m.locals_limit
                  << (m.is_entry ? "  [entry]" : "")
                  << "\n";
    }

    std::cout << "\n[classes]\n";
    for (const auto& kv : symtab.classes()) {
        const auto& c = kv.second;
        std::cout << "  .class " << c.name
                  << "  .super " << c.super_name
                  << "  pool=" << (c.pool_index == UINT32_MAX ? -1 : (int)c.pool_index)
                  << "\n";
        if (!c.fields.empty()) {
            std::cout << "    fields:";
            for (const auto& f : c.fields) {
                std::cout << " " << f.name;
            }
            std::cout << "\n";
        }
        if (!c.methods.empty()) {
            std::cout << "    methods:";
            for (const auto& mk : c.methods) {
                std::cout << " " << mk;
            }
            std::cout << "\n";
        }
    }
}

