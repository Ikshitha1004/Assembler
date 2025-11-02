// ============================================================================
// Developed By: Sahiti Vempalli
// ============================================================================

#ifndef ASSEMBLER_TOKEN_HPP
#define ASSEMBLER_TOKEN_HPP

#include <string>

enum class TokenType {
    MNEMONIC,     
    NUMBER,        
    IDENT,         
    LABEL_DEF,     
    COMMENT,       
    COMMA,        
    DIRECTIVE,     
    SYS_CALL,   
    INCLUDE,
    LIBNAME,
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
        case TokenType::LIBNAME:    return "LIBNAME";
        case TokenType::DIRECTIVE:  return "DIRECTIVE";
        case TokenType::SYS_CALL:   return "SYS_CALL";
        case TokenType::INCLUDE:    return "INCLUDE";
        case TokenType::END_OF_FILE:return "END_OF_FILE";
        default:                    return "UNKNOWN";
    }
}

struct Token {
    TokenType type;
    std::string value; // Text of the token
    int line;          // Line number in source
    int col;           // Column number in source
};

#endif // ASSEMBLER_TOKEN_HPP
