// ============================================================================
// Parser.cpp - Developed by Ikshitha,Sahiti
// ============================================================================

#include "assembler/Parser.hpp"
#include "assembler/Utils.hpp" 
#include <cctype>
#include <sstream>
#include <iostream>
#include <limits>
#include <iostream> 
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


void Parser::parse_operands(Instruction &ins) {
    while (true) {
       if (cur().type == TokenType::NUMBER) {
    int val = std::stoi(cur().value);
    Operand op;

    // // Only PUSH goes through constant pool
    // if (ins.op == OpCode::PUSH) {
    //     int idx = constpool.add_int(val);
    //     op.kind = Operand::Kind::ConstPoolIndex;
    //     op.pool_index = idx;
    // } else {
        // LOAD, STORE, etc. use immediate
        op.kind = Operand::Kind::Immediate;
        op.imm = val;
    // }

    ins.operands.push_back(op);
    advance();
}

       else if (cur().type == TokenType::IDENT) {
    // Could be a number (as string) or a label
    Operand op;

    if (is_number_literal(cur().value)) {
        // Directly store as immediate
        int val = std::stoi(cur().value);
        op.kind = Operand::Kind::Immediate;
        op.imm = val;
    } else {
        // Treat as label for now, resolved later
        op.kind = Operand::Kind::Label;
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

void Parser::parse_directive() {
    std::string dir = cur().value; 
    int line = cur().line, col = cur().col;
    advance();
    if (dir == ".data") {
        symtab.begin_data();
    }
    else if (dir == ".text") {
        symtab.begin_text();
    }
    else if (dir == ".word") {
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected label before .word at line " + std::to_string(line));
            return;
        }
        std::string name = cur().value;
        advance();

        if (cur().type != TokenType::NUMBER) {
            errlist.push_back("Expected numbers after .word " + name);
            return;
        }

        std::vector<int32_t> vals;
        while (cur().type == TokenType::NUMBER) {
            vals.push_back(std::stoi(cur().value));
            advance();
            if (cur().type == TokenType::COMMA) advance();
        }

        if (!symtab.define_data_symbol(name, vals)) {
            errlist.push_back("Duplicate or invalid data symbol: " + name);
        }
    }
        else if (dir == ".endclass") {
            if (!symtab.end_class()) {
                errlist.push_back("'.endclass' without active class at line " + std::to_string(line));
            }
        }
        else if (dir == ".endmethod") {
            if (!symtab.end_method()) {
                errlist.push_back("'.endmethod' without active method at line " + std::to_string(line));
            }
        }

    else  if (dir == ".class") {
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected class name after .class");
            return;
        }
        std::string className = cur().value;
        if (!symtab.begin_class(className)) {
            errlist.push_back("Duplicate or invalid class: " + className);
        }
        advance();
    }
    else if (dir == ".super") {
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected superclass name after .super");
            return;
        }
        std::string superName = cur().value;
        if (!symtab.set_super(superName)) {
            errlist.push_back("Failed to set superclass: " + superName);
        }
        advance();
    }
    else if (dir == ".field") {
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected field name after .field");
            return;
        }
        std::string fieldName = cur().value;
        advance();
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected field descriptor after field name");
            return;
        }
        std::string descriptor = cur().value;
        // pool_index unknown here, set to max()
        if (!symtab.add_field(symtab.current_class(), fieldName, descriptor,
                              std::numeric_limits<uint32_t>::max())) {
            errlist.push_back("Duplicate field: " + fieldName);
        }
        advance();
    }
  else if (dir == ".method") {
    if (cur().type != TokenType::IDENT) {
        errlist.push_back("Expected method name after .method");
        return;
    }
    std::string methodName = cur().value;
    advance();

    // if (cur().type != TokenType::IDENT) {
    //     errlist.push_back("Expected method signature after method name");
    //     return;
    // }
    // std::string signature = cur().value;
    // advance();

    // Use current_class_ if we are inside a class, else global method
    auto owner = symtab.get_current_class();  // expose current_class_ via getter
    if (!symtab.begin_method(methodName, "")) {
        std::string fullKey = owner.empty()
            ? methodName 
            : owner + "." + methodName ;
        errlist.push_back("Duplicate method: " + fullKey);
    }
}

    else if (dir == ".limit") {
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
        uint32_t val = std::stoi(cur().value);
        if (kind == "stack") {
            if (!symtab.set_method_stack_limit(val))
                errlist.push_back("Invalid .limit stack placement");
        } else if (kind == "locals") {
            if (!symtab.set_method_locals_limit(val))
                errlist.push_back("Invalid .limit locals placement");
        } else {
            errlist.push_back("Unknown limit kind: " + kind);
        }
        advance();
    }
    else if (dir == ".entry") {
        if (!symtab.set_method_entry(true)) {
            errlist.push_back("'.entry' outside of method at line " + std::to_string(line));
        }
    }
    else if (dir == ".end") {
        // end method or class depending on context
        if (!symtab.end_method()) {
            if (!symtab.end_class()) {
                errlist.push_back("'.end' without active method or class");
            }
        }
    }
    else if (dir == ".const") {
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected constant name after .const");
            return;
        }
        std::string constName = cur().value;
        advance();
        if (cur().type != TokenType::NUMBER) {
            errlist.push_back("Expected value after constant name");
            return;
        }
        int val = std::stoi(cur().value);
        if (!symtab.define_constant(constName, val)) {
            errlist.push_back("Duplicate constant: " + constName);
        }
        advance();
    }
    else {
        errlist.push_back("Unknown directive '" + dir + "' at line " +
                          std::to_string(line));
    }
}



/*----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------*/

void Parser::parse_line() {
    if (cur().type == TokenType::COMMENT) {
        advance();
        return;
    }

    if (cur().type == TokenType::DIRECTIVE) {
        parse_directive();
        return;
    }

    if (cur().type == TokenType::LABEL_DEF) {
        std::string lab = cur().value;
        int l = cur().line, c = cur().col;
        if (!symtab.define_label(lab, l, c)) {
            std::ostringstream os;
            os << "Duplicate label '" << lab << "' at " << l << ":" << c;
            errlist.push_back(os.str());
        }
        advance();
        return;
    }

    if (cur().type == TokenType::MNEMONIC) {
        std::string m = to_uppercopy(cur().value);
        OpCode oc = mnemonic_to_opcode(m);

        Instruction ins;
        ins.op = oc;
        ins.src_line = cur().line;
        ins.src_col  = cur().col;

        advance(); // consume mnemonic

        Operand op;

        // --- NEW ---
        // if (oc == OpCode::NEW) {
        //     if (cur().type != TokenType::IDENT) {
        //         errlist.push_back("Expected class name after NEW");
        //     } else {
        //         std::string clsName = cur().value;
        //         advance();
        //         int poolIdx = constpool.add_class(clsName);

        //         op.kind = Operand::Kind::ConstPoolIndex;
        //         op.pool_index = poolIdx;
        //         ins.operands.push_back(op);
        //     }
        // }
        // // --- GETFIELD / PUTFIELD ---
        // else if (oc == OpCode::GETFIELD || oc == OpCode::PUTFIELD) {
        //     if (cur().type != TokenType::IDENT) {
        //         errlist.push_back("Expected field reference Class.field after GETFIELD/PUTFIELD");
        //     } else {
        //         std::string fullIdent = cur().value;
        //         advance();

        //         auto dotPos = fullIdent.find('.');
        //         if (dotPos == std::string::npos) {
        //             errlist.push_back("Malformed field reference, expected Class.field");
        //         } else {
        //             std::string clazz = fullIdent.substr(0, dotPos);
        //             std::string fieldName = fullIdent.substr(dotPos + 1);

        //             auto p_field = symtab.get_field(SymbolTable::make_field_key(clazz, fieldName));
        //             bool found_field = p_field.first;
        //             const FieldInfo &finfo = p_field.second;

        //             if (!found_field) {
        //                 errlist.push_back("Undefined field reference: " + clazz + "." + fieldName);
        //             }

        //             op.kind = Operand::Kind::ConstPoolIndex;
        //             op.pool_index = finfo.pool_index;
        //             ins.operands.push_back(op);
        //         }
        //     }
        // }
        
        // --- INVOKEVIRTUAL / INVOKESPECIAL / CALL ---
        // else if (oc == OpCode::INVOKEVIRTUAL || oc == OpCode::INVOKESPECIAL) {
        //     if (cur().type != TokenType::IDENT) {
        //         errlist.push_back("Expected method reference Class.method after INVOKE");
        //     } else {
        //         std::string fullIdent = cur().value;
        //         advance();

        //         auto dotPos = fullIdent.find('.');
        //         if (dotPos == std::string::npos) {
        //             errlist.push_back("Malformed method reference, expected Class.method");
        //         } else {
        //             std::string clazz = fullIdent.substr(0, dotPos);
        //             std::string methodName = fullIdent.substr(dotPos + 1);

        //             auto p_method = symtab.get_method(methodName); // adjust key if needed
        //             bool found_method = p_method.first;
        //             const MethodInfo &minfo = p_method.second;

        //             if (!found_method) {
        //                 errlist.push_back("Undefined method reference: " + clazz + "." + methodName);
        //             }

        //             op.kind = Operand::Kind::ConstPoolIndex;
        //             op.pool_index = minfo.pool_index;
        //             ins.operands.push_back(op);
        //         }
        //     }
        // }
    if (oc == OpCode::GETFIELD || oc == OpCode::PUTFIELD) {
    Operand op;

    if (cur().type != TokenType::IDENT) {
        errlist.push_back("Expected field reference after GETFIELD/PUTFIELD");
    } else {
        std::string fullIdent = cur().value; 
        advance();

        // Split at '.' into class and field
        auto dotPos = fullIdent.find('.');
        if (dotPos == std::string::npos) {
            errlist.push_back("Malformed field reference, need ClassName.fieldName");
        } else {
            std::string clazz     = fullIdent.substr(0, dotPos);
            std::string fieldName = fullIdent.substr(dotPos + 1);

            // Build key for symbol table
            std::string key = SymbolTable::make_field_key(clazz, fieldName);

            // Lookup or auto-register
            auto p= symtab.get_field(key);
            bool found = p.first;
            const FieldInfo& fieldInfo = p.second;
            if (!found) {
                errlist.push_back("Undefined field reference: " + key);
            }

            // Encode operand
           op.kind = Operand::Kind::ConstPoolIndex;
           op.pool_index = fieldInfo.pool_index;
        }
    }

    ins.operands.push_back(op);
}
        // --- Default parsing for other instructions ---
        else {
            parse_operands(ins);

            // For jumps: record label references
            if (ins.operands.size() == 1) {
                switch (oc) {
                    case OpCode::JMP:
                    case OpCode::JZ:
                    case OpCode::JNZ:
                    {
                        const Operand& op = ins.operands[0];
                        if (op.kind == Operand::Kind::Label && !is_number_literal(op.label)) {
                            symtab.add_label_reference(instrs.size(), 0, op.label,
                                                      ins.src_line, ins.src_col);
                        }
                        break;
                    }
                    case OpCode::INVOKESPECIAL:
                    case OpCode::INVOKEVIRTUAL:
                    {
                    const Operand& op = ins.operands[0];
                    if (op.kind == Operand::Kind::Label && !is_number_literal(op.label)) {
                        std::cout<<"Method call to label: " << op.label << " at line " << ins.src_line << "\n";
                        auto p= symtab.get_method(op.label);
                        bool found = p.first;
                        std::cout<<"Method found: " << found << "\n";
                        const MethodInfo& methodInfo = p.second;
                        if (found) {
                            // Replace label operand with an immediate numeric one
                            Operand newOp;
                            newOp.kind = Operand::Kind::Immediate;
                            //pu tin immediate
                            newOp.imm = methodInfo.address;
                            ins.operands[0] = newOp;
                        } else {
                            std::cerr << "Error: undefined method " << op.label
                                    << " at line " << ins.src_line << "\n";
                        }
                    }
                    break;
                    }
                     case OpCode::CALL:
                    {
                    const Operand& op = ins.operands[0];
                    if (op.kind == Operand::Kind::Label && !is_number_literal(op.label)) {
                        std::cout<<"Method call to label: " << op.label << " at line " << ins.src_line << "\n";
                        auto p= symtab.get_method(op.label);
                        bool found = p.first;
                        std::cout<<"Method found: " << found << "\n";
                        const MethodInfo& methodInfo = p.second;
                        if (found) {
                            // Replace label operand with an immediate numeric one
                            Operand newOp;
                            newOp.kind = Operand::Kind::Immediate;
                            //pu tin immediate
                            newOp.imm = methodInfo.address;
                            ins.operands[0] = newOp;
                        } else {
                            std::cerr << "Error: undefined method " << op.label
                                    << " at line " << ins.src_line << "\n";
                        }
                    }
                    break;
                    }
                    default:
                        break;
                }
            }
        }

        validate_instruction(ins);

        instrs.push_back(std::move(ins));
        symtab.advance_lc(instr_size_bytes(instrs.back()));

        return;
    }
}


/*----------------------------------------------------------------------------------------
    Main parse (single pass + resolve forward label refs)
-----------------------------------------------------------------------------------------*/
std::vector<Instruction> Parser::parse() {
    idx = 0;
    instrs.clear();
    errlist.clear();

    uint32_t base = symtab.base();
    symtab = SymbolTable(base);
    symtab.reset_lc();

    // pass 1: read tokens into IR and collect labels/refs
    while (cur().type != TokenType::END_OF_FILE) {
        std::cout << "[DEBUG] idx=" << idx 
                  << " token=" << cur().value 
                  << " type=" << static_cast<int>(cur().type) << std::endl;

        size_t old_idx = idx;
        parse_line();
        if (idx == old_idx) {
            std::cerr << "[ERROR] idx did not advance, breaking to avoid infinite loop!" << std::endl;
            break;
        }
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
        uint32_t addr = found.second.address;
        if (r.operand_index >= target_ins.operands.size()) {
            std::ostringstream os;
            os << "Internal error: operand index OOB on '" << r.label << "'";
            errlist.push_back(os.str());
            continue;
        }
        target_ins.operands[r.operand_index].label = std::to_string(addr);
    }

    return instrs;
}


const std::vector<std::string>& Parser::errors() const {
    return errlist;
}
//TODO :need anorher pass of ins before writing to vm fir resolving and putting only operandsthat areneeded (oprand is a struct)
//TODO : access modifiers and polymorphism not handled in the gien output
//TODO : data seg needs to be handled.
