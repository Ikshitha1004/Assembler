// ============================================================================
// Developed By: Sahiti Vempalli
// ============================================================================
#include "assembler/Tokenizer.hpp"
#include "assembler/Utils.hpp"
#include <cctype>
#include <unordered_set>

Tokenizer::Tokenizer(const std::string &src)
    : src(src), pos(0), line(1), col(1) {}

bool Tokenizer::eof() const { return pos >= src.size(); }
char Tokenizer::peek() const { return eof() ? '\0' : src[pos]; }

char Tokenizer::get() {
    if (eof()) return '\0';
    char c = src[pos++];
    if (c == '\n') { ++line; col = 1; }
    else { ++col; }
    return c;
}

void Tokenizer::skip_space() {
    while (!eof()) {
        char p = peek();
        if (std::isspace((unsigned char)p)) { get(); continue; }
        // block comments /* ... */
        if (p == '/' && pos + 1 < src.size() && src[pos + 1] == '*') {
            get(); get(); // consume "/*"
            while (!eof()) {
                if (peek() == '*' && pos + 1 < src.size() && src[pos + 1] == '/') {
                    get(); get(); // consume "*/"
                    break;
                }
                get();
            }
            continue;
        }
        break;
    }
}

Token Tokenizer::make(TokenType t, const std::string &v, int l, int c) const {
    return Token{t, v, l, c};
}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> toks;

    static const std::unordered_set<std::string> MNEMONICS = {
        "PUSH","POP","DUP",
        "IADD","ISUB","IMUL","IDIV","INEG",
        "LOAD","STORE",
        "JMP","JZ","JNZ","CALL","RET",
        "ICMP_EQ","ICMP_LT","ICMP_GT",
        "NEW","GETFIELD","PUTFIELD","INVOKEVIRTUAL","INVOKESPECIAL"
    };

    while (!eof()) {
        skip_space();
        if (eof()) break;

        int start_line = line, start_col = col;
        char c = peek();

        // line comments ';' or '//'
        if (c == ';' || (c == '/' && pos + 1 < src.size() && src[pos + 1] == '/')) {
            if (c == '/') { get(); get(); } // consume //
            else { get(); }                  // consume ;
            std::string val;
            while (!eof() && peek() != '\n') val.push_back(get());
            toks.push_back(make(TokenType::COMMENT, val, start_line, start_col));
            continue;
        }

        // identifiers / directives / labels / mnemonics
        if (std::isalpha((unsigned char)c) || c == '_' || c == '.') {
            std::string ident;
            while (!eof() && (std::isalnum((unsigned char)peek()) || peek() == '_' || peek() == '.'))
                ident.push_back(get());

            // Directives start with '.'
            if (!ident.empty() && ident[0] == '.') {
                toks.push_back(make(TokenType::DIRECTIVE, ident, start_line, start_col));
                continue;
            }

            // label definitions: <ident> :
            skip_space();
            if (!eof() && peek() == ':') {
                get(); // consume ':'
                toks.push_back(make(TokenType::LABEL_DEF, ident, start_line, start_col));
                continue;
            }

            // mnemonics vs plain identifiers
            std::string up = to_uppercopy(ident);
            if (MNEMONICS.count(up)) {
                toks.push_back(make(TokenType::MNEMONIC, up, start_line, start_col));
            } else {
                toks.push_back(make(TokenType::IDENT, ident, start_line, start_col));
            }
            continue;
        }

        // numbers (allow leading minus)
        if (std::isdigit((unsigned char)c) ||
            (c == '-' && pos + 1 < src.size() && std::isdigit((unsigned char)src[pos + 1]))) {
            std::string num;
            if (peek() == '-') num.push_back(get());
            while (!eof() && std::isdigit((unsigned char)peek())) num.push_back(get());
            toks.push_back(make(TokenType::NUMBER, num, start_line, start_col));
            continue;
        }

        // comma
        if (c == ',') {
            get();
            toks.push_back(make(TokenType::COMMA, ",", start_line, start_col));
            continue;
        }

        // unknown: consume to avoid infinite loop
        get();
    }

    toks.push_back(make(TokenType::END_OF_FILE, "", line, col));
    return toks;
}
