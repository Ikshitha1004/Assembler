#include <iostream>
#include <iomanip>
#include "assembler/Tokenizer.hpp"
#include "assembler/Parser.hpp"
#include "assembler/Utils.hpp"
#include "assembler/SymbolTable.hpp"
#include "assembler/IR.hpp"
#include "assembler/Emitter.hpp"
#include "assembler/ConstantPool.hpp"


// int main(int argc, char** argv) {
//     if (argc < 2) {
//         std::cerr << "Usage: assembler <source.asm>\n";
//         return 1;
//     }

//     // Read source file
//     std::string src = read_file(argv[1]);
//     if (src.empty()) {
//         std::cerr << "Error: could not read file '" << argv[1] << "'\n";
//         return 2;
//     }

//     // Tokenize
//     Tokenizer tokenizer(src);
//     auto tokens = tokenizer.tokenize();

//     std::cout << "=== TOKENS ===\n";
//     print_tokens(tokens);

//     // Parse
//     Parser parser(tokens);
//     auto instructions = parser.parse();

//     std::cout << "\n=== INSTRUCTIONS ===\n";
//     print_instructions(instructions);

//     // Show symbol table contents
//     std::cout << "\n=== SYMBOL TABLE ===\n";
//     const SymbolTable& symtab = parser.symbols();

//     for (auto &kv : symtab.labels()) {
//         std::cout << "Label " << kv.first
//                   << " -> addr=" << kv.second.address
//                   << " (defined at line " << kv.second.line
//                   << ", col " << kv.second.col << ")\n";
//     }
//     for (auto &kv : symtab.constants()) {
//         std::cout << "Const " << kv.first
//                   << " = " << kv.second.value << "\n";
//     }
//     for (auto &kv : symtab.methods()) {
//         std::cout << "Method " << kv.first
//                   << " addr=" << kv.second.address
//                   << " stack=" << kv.second.stack_limit
//                   << " locals=" << kv.second.locals_limit
//                   << "\n";
//     }
//     for (auto &kv : symtab.classes()) {
//         std::cout << "Class " << kv.first
//                   << " super=" << kv.second.super_name << "\n";
//     }

//     if (!parser.errors().empty()) {
//         std::cerr << "\n=== ERRORS ===\n";
//         for (auto &err : parser.errors())
//             std::cerr << err << "\n";
//         return 3;
//     }

//     // === Constant Pool Debug Print ===
//     const auto &cp = parser.get_constpool().entries();
//     std::cout << "\n=== CONSTANT POOL ===\n";
//     for (auto &e : cp) {
//         std::cout << "#" << e.index << " ";
//         switch (e.tag) {
//             case assembler::ConstTag::INT:      std::cout << "INT "; break;
//             case assembler::ConstTag::FLOAT:    std::cout << "FLOAT "; break;
//             case assembler::ConstTag::STRING:   std::cout << "STRING "; break;
//         }
//         std::cout << e.str << "\n";
//     }
//     auto relocation_table = symtab.generate_relocation_table();

// std::cout << "\n=== RELOCATION TABLE ===\n";
// for (auto &r : relocation_table) {
//     std::cout << "offset=" << r.offset
//               << ", symbol=" << r.symbol_name<< "\n";
// }


//     // Build IR
//     auto irrep = assembler::IRBuilder::build(instructions);

//     std::cout << "\n=== IR WORDS ===\n";
//     for (size_t i = 0; i < irrep.words.size(); ++i) {
//         const auto &w = irrep.words[i];
//         std::cout << i << ": opcode=0x"
//                   << std::hex << std::setw(2) << std::setfill('0')
//                   << (int)w.opcode << std::dec;
//         for (auto v : w.imm) std::cout << " " << v;
//         std::cout << "   (src line " << w.src_line << ")\n";
//     }

//     // Convert IR → raw bytecode
//     std::vector<uint8_t> code;
//     for (auto &w : irrep.words) {
//         code.push_back(w.opcode);
//         for (size_t i = 0; i < w.imm.size(); ++i) {
//     int imm = w.imm[i];
//     if (w.opcode == static_cast<uint8_t>(OpCode::JMP)|| w.opcode == static_cast<uint8_t>(OpCode::JZ) || w.opcode == static_cast<uint8_t>(OpCode::JNZ)) {
//         // write 16-bit little-endian
//         code.push_back(imm & 0xFF);
//         code.push_back((imm >> 8) & 0xFF);
//     } else {
//         // write 32-bit little-endian
//         for (int b = 0; b < 4; ++b)
//             code.push_back((imm >> (8 * b)) & 0xFF);
//     }
// }

//     }
    

//     // Emit constant pool bytes
//     std::vector<uint8_t> pool_bytes;
//     parser.get_constpool().emit(pool_bytes);

//     // Final buffer = [constant pool][code]
//     std::vector<uint8_t> final_bytes;
//     final_bytes.insert(final_bytes.end(), pool_bytes.begin(), pool_bytes.end());
//     final_bytes.insert(final_bytes.end(), code.begin(), code.end());

//     // Prepare output filename
//     std::string inputFile = argv[1];
//     std::string outFile;
//     if (inputFile.size() >= 4 && inputFile.substr(inputFile.size() - 4) == ".asm") {
//         outFile = inputFile.substr(0, inputFile.size() - 4) + ".vm";
//     } else {
//         outFile = inputFile + ".vm";
//     }

//     // Write VM binary file using SymbolTable directly
//     assembler::writeVMFile(outFile, final_bytes, symtab);

//     std::cout << "\nWrote binary file: " << outFile << "\n";

//     return 0;
// }
#include "assembler/Linker.hpp"
#include "assembler/Assembler.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Helper to get filename without extension
string get_stem(const string& path) {
    size_t slash = path.find_last_of("/\\");
    size_t start = (slash == string::npos) ? 0 : slash + 1;
    size_t dot = path.find_last_of('.');
    if (dot == string::npos || dot < start)
        return path.substr(start);
    return path.substr(start, dot - start);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: vmc <output.vm> <input1.asm> [input2.asm ...]" << endl;
        return 1;
    }

    string outputFile = argv[1];
    vector<string> inputFiles;
    for (int i = 2; i < argc; ++i){
        cout<<"i/p files"<<endl;
        inputFiles.push_back(argv[i]);
    }

    vector<string> objFiles;

    try {
        // 1️⃣ Assemble each .asm → .vmobj
        for (const auto& src : inputFiles) {
            string obj = get_stem(src) + ".vmobj";
            Assembler assembler;
            if (!assembler.assemble(src, obj)) {
                cerr << "[Error] Failed to assemble " << src << endl;
                return 1;
            }
            objFiles.push_back(obj);
            cout << "[Assembler] " << src << " → " << obj << endl;
        }

        // 2️⃣ Link all .vmobj → .vm
        Linker linker;
        for (const auto& obj : objFiles)
            linker.addObjectFile(obj);

        linker.link(outputFile);

        cout << "[Success] Linked into " << outputFile << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
