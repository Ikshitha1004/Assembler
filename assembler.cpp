#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>

// -------------------- Token types --------------------
enum class TokenType { LABEL, MNEMONIC, DIRECTIVE, REGISTER, IMMEDIATE, IDENTIFIER, COMMA, LBRACKET, RBRACKET, COMMENT, ENDLINE };

// -------------------- Token struct --------------------
struct Token {
    TokenType type;
    std::string text;
    int line;
};

// -------------------- Section states --------------------
enum class SectionType { TEXT, DATA, BSS, NONE };

// -------------------- AST node --------------------
struct ASTNode {
    enum class NodeType { LABEL, DIRECTIVE, INSTRUCTION } node_type;
    std::string name;                 // label name, directive name, or mnemonic
    std::vector<std::string> operands; // operands as strings
    int line;
};

// -------------------- Tokenizer --------------------
class Tokenizer {
public:
    Tokenizer(std::istream& in) : input(in), line_num(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string line;
        while (std::getline(input, line)) {
            ++line_num;
            remove_comment(line);
            trim(line);
            if (line.empty()) continue;
            size_t pos = 0;
            while (pos < line.size()) {
                if (std::isspace(line[pos])) { ++pos; continue; }
                if (line[pos] == ',') { tokens.push_back({TokenType::COMMA,",",line_num}); ++pos; }
                else if (line[pos] == '[') { tokens.push_back({TokenType::LBRACKET,"[",line_num}); ++pos; }
                else if (line[pos] == ']') { tokens.push_back({TokenType::RBRACKET,"]",line_num}); ++pos; }
                else if (line[pos] == '.') { // directive
                    size_t start = pos;
                    while (pos < line.size() && (std::isalnum(line[pos]) || line[pos]=='.')) pos++;
                    tokens.push_back({TokenType::DIRECTIVE, line.substr(start,pos-start), line_num});
                } 
                else if (isalpha(line[pos]) || line[pos]=='_') { // mnemonic, label, identifier
                    size_t start = pos;
                    while (pos < line.size() && (isalnum(line[pos])||line[pos]=='_'||line[pos]=='.'||line[pos]==':')) pos++;
                    std::string word = line.substr(start,pos-start);
                    if (!word.empty() && word.back()==':')
                        tokens.push_back({TokenType::LABEL, word.substr(0,word.size()-1), line_num});
                    else if (is_register(word))
                        tokens.push_back({TokenType::REGISTER, word, line_num});
                    else if (is_mnemonic(word))
                        tokens.push_back({TokenType::MNEMONIC, word, line_num});
                    else
                        tokens.push_back({TokenType::IDENTIFIER, word, line_num});
                } 
                else if (line[pos]=='#') { // immediate
                    size_t start = pos+1; pos++;
                    while (pos<line.size() && (isalnum(line[pos])||line[pos]=='x'||line[pos]=='-')) pos++;
                    tokens.push_back({TokenType::IMMEDIATE, line.substr(start,pos-start), line_num});
                } 
                else if (isdigit((unsigned char)line[pos])) {
    size_t start = pos;
    while (pos < line.size() && (isalnum((unsigned char)line[pos]) || line[pos]=='x' || line[pos]=='X')) pos++;
    tokens.push_back({TokenType::IMMEDIATE, line.substr(start,pos-start), line_num});
}

                else { ++pos; } // unknown, skip
            }
            tokens.push_back({TokenType::ENDLINE,"",line_num});
        }
        return tokens;
    }

private:
    std::istream& input;
    int line_num;

    void remove_comment(std::string &line) {
        size_t pos = line.find_first_of("@;");
        if (pos != std::string::npos) line=line.substr(0,pos);
        pos=line.find("//");
        if (pos != std::string::npos) line=line.substr(0,pos);
    }

    void trim(std::string &s) {
        size_t first=s.find_first_not_of(" \t\r\n");
        size_t last=s.find_last_not_of(" \t\r\n");
        if(first==std::string::npos) s="";
        else s=s.substr(first,last-first+1);
    }

    bool is_register(const std::string &word) {
        if(word=="sp"||word=="lr"||word=="pc") return true;
        if(word.size()==2 && word[0]=='R' && isdigit(word[1])) return true;
        if(word.size()==3 && word[0]=='R' && isdigit(word[1]) && isdigit(word[2])) return true;
        return false;
    }

    bool is_mnemonic(const std::string &word) {
        static const std::unordered_map<std::string,int> mnems = {
            {"MOV",1},{"ADD",1},{"SUB",1},{"RSB",1},{"AND",1},{"ORR",1},{"EOR",1},
            {"CMP",1},{"CMN",1},{"TST",1},{"TEQ",1},{"LDR",1},{"STR",1},{"LDM",1},
            {"STM",1},{"B",1},{"BL",1},{"BX",1},{"SWI",1},{"NOP",1},{"MVN",1},
            {"LSL",1},{"LSR",1},{"ASR",1},{"ROR",1}
        };
        return mnems.count(word)>0;
    }
};

// -------------------- Parser --------------------
class Parser {
public:
    Parser(const std::vector<Token>& toks) : tokens(toks), pos(0), current_section(SectionType::NONE), location_counter(0) {}

    std::vector<ASTNode> parse() {
        std::vector<ASTNode> ast;
        while(pos<tokens.size()) {
            Token t=tokens[pos];
            if(t.type==TokenType::LABEL) {
                ast.push_back({ASTNode::NodeType::LABEL,t.text,{},t.line}); pos++;
            } 
            else if(t.type==TokenType::DIRECTIVE) {
                std::string dir = t.text;
                pos++;
                auto ops = parse_operands();
                ast.push_back({ASTNode::NodeType::DIRECTIVE, dir, ops, t.line});
                handle_directive(dir, ops);
            } 
            else if(t.type==TokenType::MNEMONIC) {
                std::string mnem = t.text; pos++;
                auto ops = parse_operands();
                ast.push_back({ASTNode::NodeType::INSTRUCTION, mnem, ops, t.line});
                location_counter += 4; // all ARM instructions are 4 bytes
            } 
            else pos++;
        }
        return ast;
    }

    SectionType get_current_section() const { return current_section; }
    int get_location_counter() const { return location_counter; }

private:
    const std::vector<Token>& tokens;
    size_t pos;
    SectionType current_section;
    int location_counter;

    void handle_directive(const std::string &name, const std::vector<std::string>& operands) {
        if(name==".text"){ current_section=SectionType::TEXT; location_counter=0;}
        else if(name==".data"){ current_section=SectionType::DATA; location_counter=0;}
        else if(name==".bss"){ current_section=SectionType::BSS; location_counter=0;}
        else if(name==".word"){ location_counter+=4*std::max(1,(int)operands.size()); }
        else if(name==".asciz"){ location_counter += std::max(1,(int)operands.size()); } // rough
        else if(name==".skip" || name==".align") {} // friend handles actual size in pass1
    }

    std::vector<std::string> parse_operands() {
        std::vector<std::string> ops;
        while(pos<tokens.size() && tokens[pos].type!=TokenType::ENDLINE){
            if(tokens[pos].type==TokenType::COMMA){ pos++; continue; }
            ops.push_back(tokens[pos].text);
            pos++;
        }
        if(pos<tokens.size() && tokens[pos].type==TokenType::ENDLINE) pos++;
        return ops;
    }
};

// -------------------- Function to print AST --------------------
void printAST(const std::vector<ASTNode>& ast) {
    for (const auto& node : ast) {
        std::cout << "[" << node.line << "] ";
        switch (node.node_type) {
            case ASTNode::NodeType::LABEL:
                std::cout << "Label: ";
                break;
            case ASTNode::NodeType::DIRECTIVE:
                std::cout << "Directive: ";
                break;
            case ASTNode::NodeType::INSTRUCTION:
                std::cout << "Instruction: ";
                break;
        }
        std::cout << node.name;
        if (!node.operands.empty()) {
            std::cout << " Operands: ";
            for (const auto& op : node.operands)
                std::cout << op << " ";
        }
        std::cout << "\n";
    }
}

// -------------------- Main (unit testing) --------------------
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " file.asm\n";
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile) {
        std::cerr << "Cannot open " << argv[1] << "\n";
        return 1;
    }

    Tokenizer tokenizer(infile);
    auto tokens = tokenizer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();

    // Print AST for unit testing
    printAST(ast);

    // Print final section and location counter
    std::cout << "Final section: ";
    switch (parser.get_current_section()) {
        case SectionType::TEXT: std::cout << ".text"; break;
        case SectionType::DATA: std::cout << ".data"; break;
        case SectionType::BSS:  std::cout << ".bss";  break;
        default: std::cout << "none";
    }
    std::cout << ", LC=" << parser.get_location_counter() << "\n";

    return 0;
}
