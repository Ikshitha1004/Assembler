// ============================================================================
// Parser.cpp - Developed by Ikshitha,Sahiti
// ============================================================================

#include "assembler/Parser.hpp"
#include "assembler/Utils.hpp" 
#include <cctype>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <iostream> 
#include <utility> // for std::move
// #include <regex>

const std::vector<std::string> Parser::known_libs = {
    "arithmetic", "io", "terminos", "utility", "filehandler", "stringhandler", "vector"
};
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
bool Parser::is_valid_lib(const std::string &name)  {
   
    for (const auto &lib : known_libs) {
        if (lib == name) return true;
    }
    return false;
}


// bool Parser::expect(TokenType t) {
//     if (accept(t)) return true;
//     std::ostringstream os;
//     os << "Parse error: expected token at line " << cur().line
//        << ", col " << cur().col;
//     errlist.push_back(os.str());
//     return false;
// }

// Checks if it's a valid integer literal (e.g. 42, -3)
static bool is_int_literal(const std::string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

// Checks if it's a valid float literal (e.g. 3.14, -0.5, .25)
static bool is_float_literal(const std::string& s) {
    if (s.empty()) return false;
    bool has_digit = false;
    bool has_dot = false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i >= s.size()) return false;

    for (; i < s.size(); ++i) {
        char c = s[i];
        if (std::isdigit(static_cast<unsigned char>(c))) {
            has_digit = true;
            continue;
        }
        if (c == '.' && !has_dot) {
            has_dot = true;
            continue;
        }
        return false;
    }
    // must have at least one digit and one dot for float
    return has_digit && has_dot;
}



void Parser::parse_operands(Instruction &ins) {
     if (ins.op == OpCode::CALL) {
        if (cur().type != TokenType::IDENT) {
            throw std::runtime_error("Expected method name after CALL");
        }

        // Get method name
        std::string methodName = cur().value;
        advance();
        // std::cout << "[Parser] CALL method: " << methodName << "\n";

        // Collect method signature types (all consecutive IDENTs)
      // Count number of '@' characters in the method name
      int argCount = std::count(methodName.begin(), methodName.end(), '@');
        std::cout<<"[Parser] CALL method: " << methodName << " with signature (" << argCount << ")\n";

        // Resolve owner (if your symbol table tracks current class)
        std::string owner = symtab.get_current_class();

        // Construct method key like "Class.add(int,int)" or "add(int,int)"
        // std::string key = SymbolTable::make_method_key(owner, methodName, sig);

        // Push operand as label (method reference)
        Operand op;
        op.kind = Operand::Kind::Label;
        op.label = methodName;
        ins.operands.push_back(op);
        Operand op2;
        op2.kind = Operand::Kind::Immediate;
        //int s = Value(argCount);
        op2.val =Value(argCount); // number of arguments
        // std::cout << "[Parser] CALL arg count: " << "\n";
        ins.operands.push_back(op2);
        return;
    }
    if (ins.op == OpCode::NEWARRAY) {
        if (cur().type != TokenType::NUMBER && cur().type != TokenType::IDENT) {
            throw std::runtime_error("NEWARRAY expects a type operand (INT/FLOAT/OBJECT)");
        }

        Operand op;
        if (is_int_literal(cur().value)) {
            op.kind = Operand::Kind::Immediate;
            op.val.intValue = std::stoi(cur().value);
            op.val.isFloat = false;
        } else {
            // Support named type like "INT", "FLOAT", "OBJECT"
            std::string typeStr = cur().value;
            int typeVal = -1;
            if (typeStr == "I" || typeStr=="int") typeVal = static_cast<int>(FieldType::INT);
            else if (typeStr == "F" || typeStr=="float" )typeVal = static_cast<int>(FieldType::FLOAT);
            // else if (typeStr == "O" || typeStr=="object") typeVal = static_cast<int>(FieldType::OBJECT);
            else if (typeStr == "C" || typeStr=="char") typeVal = static_cast<int>(FieldType::CHAR);
            else {
    bool foundClass = false;

    // Check if the type name matches any class in the symbol table
    for (auto &kv : symtab.class_metadata()) {
        if (kv.first == typeStr) {
            foundClass = true;
            break;
        }
    }

    if (foundClass) {
        typeVal = static_cast<int>(FieldType::OBJECT);
    } else {
        throw std::runtime_error("Invalid type for NEWARRAY: " + typeStr);
    }
}


            op.kind = Operand::Kind::Immediate;
            op.val.intValue = typeVal;
            op.val.isFloat = false;
        }

        ins.operands.push_back(op);
        advance();
        return;
    }

  while (true) {
    if (cur().type == TokenType::NUMBER || cur().type == TokenType::IDENT) {
        Operand op;

        if (is_float_literal(cur().value)) {
            op.kind = Operand::Kind::Immediate;
            op.val.floatValue = std::stof(cur().value);
            op.val.isFloat = true;
        } 
        else if (is_int_literal(cur().value)) {
            op.kind = Operand::Kind::Immediate;
            op.val.intValue = std::stoi(cur().value);
            op.val.isFloat = false;
        } 
        else {
            // Treat as label for now, resolved later
            op.kind = Operand::Kind::Label;
            op.label = cur().value;
        }

        ins.operands.push_back(op);
        advance(); // move to next token after processing operand

        // If next token is a comma, consume it and continue
        if (cur().type == TokenType::COMMA) {
            advance();
            continue;
        } 
        else {
            break; // no more operands
        }
    } 
    else {
        break; // not a valid operand
    }
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

        // ---- One operand required ----
        case OpCode::PUSH:
        case OpCode::FPUSH:
        case OpCode::LOAD:
        case OpCode::STORE:
        case OpCode::LOAD_ARG:
        case OpCode::JMP:
        case OpCode::JZ:
        case OpCode::JNZ:
        case OpCode::NEW:
        case OpCode::GETFIELD:
        case OpCode::PUTFIELD:
        case OpCode::SYS_CALL:       
        case OpCode::NEWARRAY:       
            if (ins.operands.size() != 1)
                bad("Instruction requires exactly 1 operand");
            break;

        // ---- Zero operand instructions ----
        case OpCode::POP:
        case OpCode::DUP:
        case OpCode::FPOP:
        case OpCode::IADD:
        case OpCode::ISUB:
        case OpCode::IMUL:
        case OpCode::IDIV:
        case OpCode::INEG:
        case OpCode::FADD:
        case OpCode::FSUB:
        case OpCode::FMUL:
        case OpCode::FDIV:
        case OpCode::FNEG:
        case OpCode::IMOD:
        case OpCode::ICMP_EQ:
        case OpCode::ICMP_LT:
        case OpCode::ICMP_GT:
        case OpCode::ICMP_GEQ:
        case OpCode::ICMP_NEQ:
        case OpCode::ICMP_LEQ:
        case OpCode::FCMP_EQ:
        case OpCode::FCMP_LT:
        case OpCode::FCMP_GT:
        case OpCode::FCMP_GEQ:
        case OpCode::FCMP_NEQ:
        case OpCode::FCMP_LEQ:
        case OpCode::ALOAD:
        case OpCode::ASTORE:
        case OpCode::RET:
            if (!ins.operands.empty())
                bad("Instruction takes no operands");
            break;
        case OpCode::CALL:
        case OpCode::INVOKEVIRTUAL:
        case OpCode::INVOKESPECIAL:
            if (ins.operands.size() != 2)
                bad("CALL instruction requires exactly 2 operands");
            break;

        // ---- Catch invalid mnemonics ----
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
        // parse_data_section();
    } 
    else if (dir == ".code") {
        symtab.begin_text();
    } 
    else if (dir == ".class_metadata") {
        parse_class_metadata(); // assume this parses class metadata block
    } 
    else if (dir == ".word") {
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected label before .word at line " + std::to_string(line));
            return;
        }
        std::string name = cur().value;
        advance();

        if (cur().type != TokenType::NUMBER) {
            errlist.push_back("Expected number after .word " + name);
            return;
        }

        std::vector<Value> vals;
        while (cur().type == TokenType::NUMBER) {
            const std::string &numStr = cur().value;
            if (numStr.find('.') != std::string::npos)
                vals.push_back(Value(std::stof(numStr)));
            else
                vals.push_back(Value(std::stoi(numStr)));
            advance();
            if (cur().type == TokenType::COMMA) advance();
        }

        if (!symtab.define_data_symbol(name, vals)) {
            errlist.push_back("Duplicate or invalid data symbol: " + name);
        }
    } 
    else if (dir == ".endclass") {
        if (!symtab.end_class_metadata()) {
            errlist.push_back("'.endclass' without active class at line " + std::to_string(line));
        }
    } 
    else if (dir == ".endmethod") {
        if (!symtab.end_method()) {
            errlist.push_back("'.endmethod' without active method at line " + std::to_string(line));
        }
    } 
    // else if (dir == ".class") {
    //     if (cur().type != TokenType::IDENT) {
    //         errlist.push_back("Expected class name after .class");
    //         return;
    //     }
    //     std::string className = cur().value;
    //     if (!symtab.begin_class_metadata(className,)) {
    //         errlist.push_back("Duplicate or invalid class: " + className);
    //     }
    //     advance();
    // } 
    else if (dir == ".super") {
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected superclass name after .super");
            return;
        }
        std::string superName = cur().value;
        if (!symtab.set_class_super(superName)) {
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
        advance();

        FieldInfo fi{symtab.get_current_class(), fieldName, descriptor, std::numeric_limits<uint32_t>::max()};
        if (!symtab.add_field_metadata(fi)) {
            errlist.push_back("Duplicate field: " + fieldName);
        }
    } 
    else if (dir == ".method") {
    if (cur().type != TokenType::IDENT) {
        errlist.push_back("Expected method name after .method");
        return;
    }

    std::string methodName = cur().value;
    // auto dotPos = methodName.find('.');
    // if (dotPos != std::string::npos) {
    //     std::string className = methodName.substr(0, dotPos);
    //     std::string actualMethodName = methodName.substr(dotPos + 1);

    //     if (className == actualMethodName) {
    //         std::cout << "Class name and method name are the same: " << className << std::endl;
    //         symtab.set_current_class(className);

    //     }
    // }
    std::cout << "Parsing method: " << methodName << std::endl;
    advance();



    if (!symtab.begin_method(methodName, "")) {
        errlist.push_back("Duplicate method: " + methodName);
        return;
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
    else if (dir == ".end") {
        if (!symtab.end_method()) {
            if (!symtab.end_class_metadata()) {
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
        errlist.push_back("Unknown directive '" + dir + "' at line " + std::to_string(line));
    }
}
// void Parser::parse_data_section() {
//     // Consume the ".data" directive

//     while (idx < toks.size()) {
//         const Token& t = cur();

//         // Stop if another section begins
//         if (t.value == ".code" || t.value == ".text" || t.value == ".method" || t.value == ".end_metadata")
//             break;

//         // Expect label definition like STR_0:
//         if (t.type != TokenType::LABEL_DEF) {
//             errlist.push_back("[Parser] Expected label in .data section, got: " + t.value);
//             advance();
//             continue;
//         }

//         std::string label = t.value;
//         advance();

//         // Expect ".word"
//         if (cur().type != TokenType::DIRECTIVE || cur().value != ".word") {
//             errlist.push_back("[Parser] Expected '.word' after label " + label);
//             advance();
//             continue;
//         }
//         advance();

//         // Expect a STRING token
//         if (cur().type != TokenType::STRING) {
//             errlist.push_back("[Parser] Expected string literal after .word for label " + label);
//             advance();
//             continue;
//         }

//         std::string value = cur().value;
//         advance();

//         int cpIndex = constpool.add_string(value);
//         std::cout << "[ConstPool] Added " << label << " -> #" << cpIndex
//                   << " \"" << value << "\"\n";
//     }
// }


void Parser::parse_class_metadata() {
    // Current token is `.class_metadata`

    // class_count
    if (cur().type != TokenType::IDENT || cur().value != "class_count") {
        errlist.push_back("Expected 'class_count' after .class_metadata");
        return;
    }
    advance(); // consume 'class_count'

    if (cur().type != TokenType::NUMBER) {
        errlist.push_back("Expected number after 'class_count'");
        return;
    }
    int class_count = std::stoi(cur().value);
    advance(); // consume number

    for (int i = 0; i < class_count; ++i) {
        // class_begin
        if (cur().type != TokenType::IDENT || cur().value != "class_begin") {
            errlist.push_back("Expected 'class_begin' in metadata");
            return;
        }
        advance(); // consume 'class_begin'

        // class name
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected class name after 'class_begin'");
            return;
        }
        std::string class_name = cur().value;
        advance();

        // super index
        if (cur().type != TokenType::IDENT) {
            errlist.push_back("Expected super class name after class name");
            return;
        }
        std::string super_name = cur().value;
        advance();

        if (!symtab.begin_class_metadata(class_name,super_name)) {
            errlist.push_back("Duplicate class: " + class_name);
        }

        // --- Fields ---
        if (cur().type != TokenType::IDENT || cur().value != "field_count") {
            errlist.push_back("Expected 'field_count'");
            return;
        }
        advance();

        if (cur().type != TokenType::NUMBER) {
            errlist.push_back("Expected number after 'field_count'");
            return;
        }
        int field_count = std::stoi(cur().value);
        advance();

        for (int f = 0; f < field_count; ++f) {
            if (cur().type != TokenType::IDENT || cur().value != "field") {
                errlist.push_back("Expected 'field' declaration");
                return;
            }
            advance();

            if (cur().type != TokenType::IDENT) { errlist.push_back("Expected field name"); return; }
            std::string field_name = cur().value;
            advance();

            if (cur().type != TokenType::IDENT) { errlist.push_back("Expected field type"); return; }
            std::string field_type = cur().value;
            advance();

            if (cur().type != TokenType::NUMBER) { errlist.push_back("Expected field pool index"); return; }
            uint32_t index = std::stoi(cur().value);
            advance();

            symtab.add_field(class_name, field_name, field_type, index);
        }

        // --- Methods ---
        if (cur().type != TokenType::IDENT || cur().value != "method_count") {
            errlist.push_back("Expected 'method_count'");
            return;
        }
        advance();

        if (cur().type != TokenType::NUMBER) { errlist.push_back("Expected number after 'method_count'"); return; }
        int method_count = std::stoi(cur().value);
        advance();

        for (int m = 0; m < method_count; ++m) {
            if (cur().type != TokenType::IDENT || cur().value != "method") {
                errlist.push_back("Expected 'method' declaration");
                return;
            }
            advance();

            if (cur().type != TokenType::IDENT) { errlist.push_back("Expected method name"); return; }
            std::string method_name = cur().value;
            advance();

            if (cur().type != TokenType::IDENT) { errlist.push_back("Expected qualified method"); return; }
            std::string qualified_name = cur().value; 
            advance();

            // signature also inside qualified name 
            symtab.define_method(class_name, qualified_name, "", 0, 0, 0,false);
        }

        if (cur().type != TokenType::IDENT || cur().value != "class_end") {
            errlist.push_back("Expected 'class_end'");
            return;
        }
        advance();
        symtab.end_class_metadata();
    }

    // .end_metadata
    if (cur().type != TokenType::DIRECTIVE || cur().value != ".end_metadata") {
        errlist.push_back("Expected '.end_metadata'");
        return;
    }
    advance();
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
    if (cur().type == TokenType::INCLUDE) {
    advance(); // move past INCLUDE
    if (cur().type == TokenType::LIBNAME) {
        if (!is_valid_lib(cur().value)) {
        std::cerr << "[Parser] Warning: Unknown library '" << cur().value
                  << "'. It may not be linked." << std::endl;
    }
        includes.push_back(cur().value);
        advance(); // move past LIBNAME
    } else {
        std::cerr << "[Parser] Error: expected library filename after #include" << std::endl;
    }
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
        if (oc == OpCode::NEW) {
            if (cur().type != TokenType::IDENT) {
               errlist.push_back("Expected class name after NEW");
            }

            std::string className = cur().value;
            advance();

            auto p = symtab.get_class_metadata(className);
            bool found = p.first;
            const ClassMetadata& clsMeta = p.second;

                        // store class name as label
             Operand op;
            if (found) {
               
                // op.kind = Operand::Kind::Immediate;  // label points to a class name
                // op.val = Value((int)clsMeta.index); 
                symtab.set_current_class(className);
            } 
                
            op.kind = Operand::Kind::Label;  // label points to a class name
            op.label = className; 
            symtab.add_reference(instrs.size(), 0, className,
                                    ins.src_line, ins.src_col,2);  // false = not a method
            ins.operands.push_back(op);
        }
       
    if (oc == OpCode::GETFIELD || oc == OpCode::PUTFIELD) {
    Operand op;

    if (cur().type != TokenType::NUMBER) {
        errlist.push_back("Expected numeric field index after GETFIELD/PUTFIELD");
    } else {
        
            int fieldIndex = std::stoi(cur().value);
            op.kind = Operand::Kind::Immediate; // direct integer operand
            op.val = Value(fieldIndex);
            auto p = symtab.get_class_metadata(symtab.get_current_class());
            bool found = p.first;
            const ClassMetadata& clsMeta = p.second;
            if (found && fieldIndex >= clsMeta.fields.size()) {
                // errlist.push_back("Invalid field index for class " + clsMeta.name+"field index: " + std::to_string(fieldIndex) +"at line " + std::to_string(ins.src_line));
            }
        advance();
    }

    ins.operands.push_back(op);
}
if (oc == OpCode::INVOKEVIRTUAL || oc == OpCode::INVOKESPECIAL) {
    Operand op;

    // Expect a numeric method index
    if (cur().type != TokenType::NUMBER) {
        errlist.push_back("Expected numeric method index after INVOKEVIRTUAL/INVOKESPECIAL");
    } else {
        Operand op2;
        int methodIndex = std::stoi(cur().value);
        op2.kind = Operand::Kind::Immediate;
        op2.val = Value(methodIndex); // store index as immediate

        // Validate index against current class
       
        //  auto p = symtab.get_class_metadata(symtab.get_current_class());
        //     bool found = p.first;
        //     const ClassMetadata& clsMeta = p.second;
        // if (found) {
        //     if (methodIndex < 0 || methodIndex >= clsMeta.methods.size()) {
        //         errlist.push_back("Invalid method index " + std::to_string(methodIndex) +
        //                           " for class " + clsMeta.name);
        //     }
        // } else {
        //     errlist.push_back("Current class metadata not found for validation");
        // }
        
        ins.operands.push_back(op2);
        advance();
        if (cur().type != TokenType::NUMBER) {
        errlist.push_back("Expected numeric method index after INVOKEVIRTUAL/INVOKESPECIAL");
    }
        op.kind = Operand::Kind::Immediate;
        //int s = Value(argCount);
        op.val =std::stoi(cur().value); // number of arguments
        advance(); // consume the number
    }

    ins.operands.push_back(op);
    symtab.advance_lc(1);

}


    else if (oc == OpCode::SYS_CALL) {
        Operand op;

        if (cur().type != TokenType::SYS_CALL) {
            errlist.push_back("Expected syscall name after SYS_CALL");
        } else {
            std::string sysName = to_uppercopy(cur().value);
            advance();

            try {
                int sysnum = mnemonic_to_syscall(sysName); // convert name -> number
                op.kind = Operand::Kind::Immediate;
                op.val = sysnum;
            } catch (const std::exception &e) {
                errlist.push_back(e.what());
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
                        if (op.kind == Operand::Kind::Label && !is_int_literal(op.label) && !is_float_literal(op.label)) {
                            std::cout<<"Jump to label: " << op.label << " at line " << ins.src_line << "\n";
                            symtab.add_reference(instrs.size(), 0, op.label,
                                                      ins.src_line, ins.src_col);
                        }
                        break;
                    }
                    
                    default:
                        break;
                }
            }
            if(oc == OpCode::CALL) {
                    
       

                    const Operand& op = ins.operands[0];

                    if (op.kind == Operand::Kind::Label && !is_int_literal(op.label)  && !is_float_literal(op.label)) {
                         
                        std::cout<<"Method call to label: " << op.label << " at line " << ins.src_line << "\n";
                       std:: string label = op.label;
                        auto p= symtab.get_method(op.label);
                        bool found = p.first;
                        std::cout<<"Method found: " << found << "\n";
                        const MethodInfo& methodInfo = p.second;
                        Operand newOp;
                        newOp.kind = Operand::Kind::Immediate;
                        if (found) {
                            // Replace label operand with an immediate numeric one
                            //put in immediate
                            newOp.val =(int) methodInfo.address;
                            ins.operands[0] = newOp;
                        } else {
                            symtab.add_reference(instrs.size(), 0, label,
                               ins.src_line, ins.src_col,1);
                            newOp.val = 0;
                            ins.operands[0] = newOp;
                            //std::cout<<"Adding pending reference for method: " << op.label << "\n";
                           
                            
                            // std::cerr << "Error: undefined method " << op.label
                            //         << " at line " << ins.src_line << "\n";
                        }
                    }
                    symtab.advance_lc(1);
            }
        }

        validate_instruction(ins);

        instrs.push_back(std::move(ins));
        symtab.advance_lc(instruction_size(instrs.back().op));

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
    auto& refs = symtab.pending_refs();
    // std::cout << "[Parser] Resolving " << refs.size() << " pending label references\n";
    for (auto& r : refs) {

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
        r.resolved=true;
    }

    return instrs;
}


const std::vector<std::string>& Parser::errors() const {
    return errlist;
}