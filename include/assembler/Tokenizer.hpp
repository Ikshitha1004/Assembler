// ============================================================================
// Developed By: Sahiti Vempalli
// ============================================================================
#ifndef ASSEMBLER_TOKENIZER_HPP
#define ASSEMBLER_TOKENIZER_HPP

#include "Token.hpp"
#include <vector>
#include <string>

class Tokenizer {
public:
    explicit Tokenizer(const std::string &src);
    std::vector<Token> tokenize();

private:
    std::string src;
    size_t pos;
    int line, col;

    bool eof() const;
    char peek() const;
    char get();
    void skip_space();
    Token make(TokenType t, const std::string &v, int l, int c) const;
};

#endif // ASSEMBLER_TOKENIZER_HPP