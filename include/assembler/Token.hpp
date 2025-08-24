// ============================================================================
// Developed By: Sahiti Vempalli
// ============================================================================
#ifndef ASSEMBLER_Token_hpp
#define ASSEMBLER_Token_hpp


#include <string>


enum class TokenType {
MNEMONIC,
NUMBER,
IDENT,
LABEL_DEF,
COMMENT,
COMMA,
END_OF_FILE,
};


struct Token {
TokenType type;
std::string value;
int line;
int col;
};


#endif // ASSEMBLER_Token_hpp