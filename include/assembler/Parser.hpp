/*----------------------------------------------------------------------------------------
    This module was written by Ikshitha (CS22B027)
-------------------------------------------------------------------------------------------*/

#ifndef ASSEMBLER_Parser_hpp
#define ASSEMBLER_Parser_hpp

#include "Token.hpp"
#include "Instruction.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class Parser {
public:
    explicit Parser(const std::vector<Token> &toks);
    std::vector<Instruction> parse();
    const std::vector<std::string>& errors() const;
    const std::unordered_map<std::string,int>& labels() const;

private:
    std::vector<Token> toks;
    size_t idx;
    std::vector<Instruction> instrs;
    std::unordered_map<std::string,int> label_map;
    std::vector<std::string> errlist;

    const Token& cur() const;
    void advance();
    bool accept(TokenType t);
    bool expect(TokenType t);

    void parse_line();
    void parse_operands(Instruction &ins);
    void validate_instruction(const Instruction &ins);
};

#endif // ASSEMBLER_Parser_hpp