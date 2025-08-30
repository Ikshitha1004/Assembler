
/*----------------------------------------------------------------------------------------
    This module was written by Ikshitha (CS22B027) 
-------------------------------------------------------------------------------------------*/
#include "assembler/Parser.hpp"
#include "assembler/Utils.hpp"
#include <sstream>
#include <iostream>

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
            if (cur().type == TokenType::NUMBER) {
            Operand op;
            op.kind = Operand::Kind::Immediate;
            op.imm = std::stoi(cur().value);
            ins.operands.push_back(op);
            advance();
        }
        else if (cur().type == TokenType::IDENT) {
            Operand op;
           if (ins.op == OpCode::NEW || 
            ins.op== OpCode::GETFIELD || 
            ins.op == OpCode::PUTFIELD || 
            ins.op == OpCode::INVOKEVIRTUAL) {
            
            op.kind = Operand::Kind::ConstPoolIndex;
            op.label = cur().value;  
        } 
        else {
            op.kind = Operand::Kind::Label;  // normal label case (for jumps, etc.)
            op.label = cur().value;
        }
            ins.operands.push_back(op);
            advance();
        }
      else {
            break;
        }
        if (cur().type == TokenType::COMMA) advance();
        else break;
    }
}
/*----------------------------------------------------------------------------------------
    This module was written by Dakshayani
-------------------------------------------------------------------------------------------*/
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

void Parser::parse_line() {
    if (cur().type == TokenType::COMMENT) {
        advance();
        return;
    }
    if (cur().type == TokenType::DIRECTIVE) { parse_directive(); return; }
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
         std::vector<std::string> idents;
       if (opcode_to_string(oc) == "PUTFIELD" || opcode_to_string(oc) == "GETFIELD") {
        Operand op;
        op.kind = Operand::Kind::FieldRef;

        std::vector<std::string> idents;
        advance(); // consume mnemonic
        while (cur().type == TokenType::IDENT) {
            std::cout<<"came here "<<cur().value<<std::endl;
            idents.push_back(cur().value);
            advance();
        }

        if (idents.size() < 2) {
            errlist.push_back("Malformed field reference, need at least class/field and descriptor");
        } else {
            // Everything except last two = class (with slashes if needed)
            std::string clazz;
            for (size_t i = 0; i < idents.size(); ++i) {
                if (!clazz.empty()) clazz += "/";
                clazz += idents[i];
            }
            
            // field name = second-to-last
            std::string fieldName = idents[idents.size() - 2];

            // descriptor = last
            std::string desc = idents.back();
            op.fieldref.clazz = clazz;
            op.fieldref.name  = fieldName;
            op.fieldref.desc  = desc;
        }
        
        ins.operands.push_back(op);
        return;
}

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
void Parser::parse_directive() {
    std::string dir = cur().value; // like ".class" or ".method"
    int line = cur().line, col = cur().col;
    advance();

    if (dir == ".class") {
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected class name after .class");
            return;
        }
        currentClass = ClassInfo{};
        currentClass.name = cur().value;
        classes.push_back(currentClass);
        advance();
    }
    else if (dir == ".method") {
        currentMethod = new MethodInfo{};
        // read modifiers + name
        
        while (cur().type == TokenType::IDENT) {
           // std::cout << "came here "<<std::endl;
            currentMethod->modifiers.push_back(cur().value);
            advance();
        }
        // if (cur().type != TokenType::IDENT) {
        //    std::cout << "here: " << tokenTypeToString(cur().type) << "\n";
        //     errlist.push_back("Expected method name after .method");
        //     return;
        // }
        //advance();
    }
    else if (dir == ".limit") {
        if (!currentMethod) {
            errlist.push_back("'.limit' outside of method at line " + std::to_string(line));
            return;
        }
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected 'stack' or 'locals' after .limit");
            return;
        }
        std::string kind = cur().value;
        advance();
        if (cur().type != TokenType::NUMBER) {
            errlist.push_back("Expected number after .limit " + kind);
            return;
        }
        int val = std::stoi(cur().value);
        if (kind == "stack") currentMethod->maxStack = val;
        else if (kind == "locals") currentMethod->maxLocals = val;
        else errlist.push_back("Unknown limit kind: " + kind);
        advance();
    }
    else if (dir == ".end") {
        if (currentMethod) {
            // push back method into class
            classes.back().methods.push_back(*currentMethod);
            delete currentMethod;
            currentMethod = nullptr;
        }
    }
    else {
        errlist.push_back("Unknown directive '" + dir + "' at line " +
                          std::to_string(line));
    }
}

std::vector<Instruction> Parser::parse() {
    idx = 0;
    instrs.clear();
    label_map.clear();
    errlist.clear();

    while (cur().type != TokenType::END_OF_FILE) {
        parse_line();
    }
/*----------------------------------------------------------------------------------------
    This module was written by Ikshitha (CS22B027) 
-------------------------------------------------------------------------------------------*/
   for (auto &ins : instrs) {
    for (auto &op : ins.operands) {
        
        if (op.kind == Operand::Kind::Label && opcode_to_string(ins.op)!="GETFIELD" && opcode_to_string(ins.op)!="PUTFIELD") {
            auto it = label_map.find(op.label);
            if (it != label_map.end()) {
                // convert to immediate
                op.kind = Operand::Kind::Immediate;
                op.imm = it->second;
            } else {
                errlist.push_back("Undefined label: " + op.label);
            }
        }
    }
}


    return instrs;
}

const std::vector<std::string>& Parser::errors() const { return errlist; }
const std::unordered_map<std::string,int>& Parser::labels() const { return label_map; }
