/*----------------------------------------------------------------------------------------
    This module was written by Ikshitha (CS22B027)
-------------------------------------------------------------------------------------------*/

#ifndef ASSEMBLER_Parser_hpp
#define ASSEMBLER_Parser_hpp



#include <vector>
#include <string>
#include "Token.hpp"
#include "Instruction.hpp"
#include "SymbolTable.hpp"

class Parser {
public:
    Parser(const std::vector<Token> &toks);

    std::vector<Instruction> parse();

    const std::vector<std::string>& errors() const;

    const SymbolTable& symbols() const { return symtab; }

private:
    std::vector<Token> toks;
    size_t idx {0};

    std::vector<Instruction> instrs;
    std::vector<std::string> errlist;

    SymbolTable symtab; //added for symbol table

    const Token& cur() const;
    void advance();
    bool accept(TokenType t);
    bool expect(TokenType t);

    void parse_line();
    void parse_operands(Instruction &ins);
    void validate_instruction(const Instruction &ins);
   

};

#endif // ASSEMBLER_PARSER_HPP
