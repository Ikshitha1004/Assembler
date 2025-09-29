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
    if (c == '\n') {
        ++line;
        col = 1;
    } else {
        ++col;
    }
    return c;
}

void Tokenizer::skip_space() {
    while (!eof() && std::isspace((unsigned char)peek())) get();
}

Token Tokenizer::make(TokenType t, const std::string &v, int l, int c) const {
    return Token{t, v, l, c};
}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> toks;

    static const std::unordered_set<std::string> MNEMONICS = {
    "PUSH","POP","DUP","FPUSH","FPOP",
    "IADD","ISUB","IMUL","IDIV","INEG",
    "FADD","FSUB","FMUL","FDIV","FNEG",
    "LOAD","STORE","LOAD_ARG",
    "JMP","JZ","JNZ","CALL","RET",
    "ICMP_EQ","ICMP_LT","ICMP_GT",
    "ICMP_GEQ","ICMP_NEQ","ICMP_LEQ",
    "FCMP_EQ","FCMP_LT","FCMP_GT",
    "FCMP_GEQ","FCMP_NEQ","FCMP_LEQ",
    "NEW","GETFIELD","PUTFIELD",
    "INVOKEVIRTUAL","INVOKESPECIAL"
};

    while (!eof()) {
        skip_space();
        if (eof()) break;

        int start_line = line, start_col = col;
        char c = peek();

        // Comment: ';' or '//'
        if (c == ';') {
            std::string val;
            while (!eof() && get() != '\n')
                val.push_back(src[pos - 1]);
            toks.push_back(make(TokenType::COMMENT, val, start_line, start_col));
            continue;
        }
        if (c == '/' && pos + 1 < src.size() && src[pos + 1] == '/') {
            get(); get(); // consume both slashes
            std::string val;
            while (!eof() && peek() != '\n') val.push_back(get());
            toks.push_back(make(TokenType::COMMENT, val, start_line, start_col));
            continue;
        }

        // Identifiers, mnemonics, labels
        if (std::isalpha((unsigned char)c) || c == '_' || c == '.') {
            std::string ident;
            while (!eof() && (std::isalnum((unsigned char)peek()) || peek() == '_' || peek() == '.'))
                ident.push_back(get());
              // Directives start with '.'
            if (!ident.empty() && ident[0] == '.') {
                toks.push_back(make(TokenType::DIRECTIVE, ident, start_line, start_col));
                continue;
            }
            skip_space();
            if (!eof() && peek() == ':') {
                get(); // consume ':'
                toks.push_back(make(TokenType::LABEL_DEF, ident, start_line, start_col));
                continue;
            }

            std::string up = to_uppercopy(ident);
            if (MNEMONICS.count(up)) {
                toks.push_back(make(TokenType::MNEMONIC, up, start_line, start_col));
            } else {
                toks.push_back(make(TokenType::IDENT, ident, start_line, start_col));
            }
            continue;
        }

        // Numbers (support negative)
        if (std::isdigit((unsigned char)c) || (c == '-' && pos + 1 < src.size() && std::isdigit((unsigned char)src[pos + 1]))) {
            std::string num;
            if (peek() == '-') num.push_back(get());
            while (!eof() && std::isdigit((unsigned char)peek())) num.push_back(get());
            toks.push_back(make(TokenType::NUMBER, num, start_line, start_col));
            continue;
        }

        // Comma
        if (c == ',') {
            get();
            toks.push_back(make(TokenType::COMMA, ",", start_line, start_col));
            continue;
        }

        // Unknown: skip but consume
        get();
    }

    toks.push_back(make(TokenType::END_OF_FILE, "", line, col));
    return toks;
}
