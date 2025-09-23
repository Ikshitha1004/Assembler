/*----------------------------------------------------------------------------------------
    This module was written by Ikshitha (CS22B027)
-------------------------------------------------------------------------------------------*/

#ifndef ASSEMBLER_Parser_hpp
#define ASSEMBLER_Parser_hpp

#include "assembler/Token.hpp"
#include "assembler/IR.hpp"
#include "assembler/SymbolTable.hpp"
#include "assembler/ConstantPool.hpp"   
#include <vector>
#include <string>

class Parser {
public:
    Parser(const std::vector<Token>& t);

    std::vector<Instruction> parse();
    const std::vector<std::string>& errors() const;

    const assembler::ConstantPool& get_constpool() const { return constpool; }
    const SymbolTable& symbols() const { return symtab; }


private:
    const std::vector<Token>& toks;
    size_t idx;

    std::vector<Instruction> instrs;
    std::vector<std::string> errlist;

    SymbolTable symtab; //added for symbol table
    assembler::ConstantPool constpool;

    const Token& cur() const;
    void advance();
    bool accept(TokenType t);
    bool expect(TokenType t);

    void parse_line();
    void parse_operands(Instruction &ins);
    void parse_directive();
    void validate_instruction(const Instruction &ins);
};

#endif // ASSEMBLER_Parser_hpp
