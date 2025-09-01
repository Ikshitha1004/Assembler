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
DIRECTIVE,
END_OF_FILE,
};

inline const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::MNEMONIC:   return "MNEMONIC";
        case TokenType::NUMBER:     return "NUMBER";
        case TokenType::IDENT:      return "IDENT";
        case TokenType::LABEL_DEF:  return "LABEL_DEF";
        case TokenType::COMMENT:    return "COMMENT";
        case TokenType::COMMA:      return "COMMA";
        case TokenType::END_OF_FILE:return "END_OF_FILE";
        case TokenType::DIRECTIVE:  return "DIRECTIVE";
        default:                    return "UNKNOWN";
    }
}
struct Token {
TokenType type;
std::string value;
int line;
int col;
};


#endif // ASSEMBLER_Token_hpp