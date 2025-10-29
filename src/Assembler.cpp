#include "assembler/Assembler.hpp"
#include "assembler/Tokenizer.hpp"
#include "assembler/Parser.hpp"
#include "assembler/EmitterObj.hpp"
#include "assembler/Utils.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>


// --- Assemble method ---
bool Assembler::assemble(const std::string& inputFile, const std::string& outputObjFile) {
    //  Read source
    std::ifstream in(inputFile);
    if (!in.is_open()) {
        std::cerr << "[Assembler] Cannot open " << inputFile << std::endl;
        return false;
    }
    std::string src((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    //  Tokenize
    Tokenizer tokenizer(src);
    auto tokens = tokenizer.tokenize();
    std::cout << "[Tokenizer] " << tokens.size() << " tokens read from " << inputFile << "\n";
    std::cout << "=== TOKENS ===\n";
    print_tokens(tokens);

    //  Parse
    Parser parser(tokens);
    auto instructions = parser.parse();
    std::cout << "\n=== INSTRUCTIONS ===\n";
    print_instructions(instructions);
    std::cout << "\n=== SYMBOL TABLE ===\n";
    const auto& symtab = parser.symbols();
    for (auto &kv : symtab.labels()) {
        std::cout << "Label " << kv.first
                  << " -> addr=" << kv.second.address
                  << " (defined at line " << kv.second.line
                  << ", col " << kv.second.col << ")\n";
    }
    for (auto &kv : symtab.constants()) {
        std::cout << "Const " << kv.first
                  << " = " << kv.second.value << "\n";
    }
    for (auto &kv : symtab.methods()) {
        std::cout << "Method " << kv.first
                  << " addr=" << kv.second.address
                  << " stack=" << kv.second.stack_limit
                  << " locals=" << kv.second.locals_limit
                  << "\n";
    }
    for (auto &kv : symtab.class_metadata()) {
        std::cout << "Class " << kv.first
                  << " super=" << kv.second.super_name << "\n";
    }



    if (!parser.errors().empty()) {
        std::cerr << "[Parser] Errors in " << inputFile << ":\n";
        for (const auto &err : parser.errors())
            std::cerr << "  " << err << "\n";
        return false;
    }

    // Encode instructions → bytecode
   // std::vector<uint8_t> code;
    // for (const auto& ins : instructions) {
    //     //auto encoded = encode_instruction(ins);
    //     code.insert(code.end(), encoded.begin(), encoded.end());
    // }
      // Build IR
    //     // === Constant Pool Debug Print ===
    const auto &cp = parser.get_constpool().entries();
    std::cout << "\n=== CONSTANT POOL ===\n";
    for (auto &e : cp) {
        std::cout << "#" << e.index << " ";
        switch (e.tag) {
            case assembler::ConstTag::INT:      std::cout << "INT "; break;
            case assembler::ConstTag::FLOAT:    std::cout << "FLOAT "; break;
            case assembler::ConstTag::STRING:   std::cout << "STRING "; break;
        }
        std::cout << e.str << "\n";
    }
    auto relocation_table = symtab.generate_relocation_table();

    std::cout << "\n=== RELOCATION TABLE ===\n";
    for (auto &r : relocation_table) {
        std::cout << "offset=" << r.offset
                << ", symbol=" << r.symbol_name<< "\n";
    }

    auto irrep = assembler::IRBuilder::build(instructions);

    std::cout << "\n=== IR WORDS ===\n";
    for (size_t i = 0; i < irrep.words.size(); ++i) {
        const auto &w = irrep.words[i];
        std::cout << i << ": opcode=0x"
                  << std::hex << std::setw(2) << std::setfill('0')
                  << (int)w.opcode << std::dec;
    for (auto &v : w.imm) {
        std::cout << " " << (v.isFloat ? v.floatValue : v.intValue);
    }        
    std::cout << "   (src line " << w.src_line << ")\n";
    }

    // Convert IR → raw bytecode
   std::vector<uint8_t> code;

//     for (auto &w : irrep.words) {
//         code.push_back(w.opcode);

//         std::size_t instrSize = instruction_size(static_cast<OpCode>(w.opcode));
//         std::size_t operandSize = (instrSize > 1) ? instrSize - 1 : 0;
// for (size_t i = 0; i < w.imm.size(); ++i) {
//     const Value &v = w.imm[i];
//     if (v.isFloat) {
//         // Treat as 32-bit float
//         uint32_t fbits;
//         static_assert(sizeof(fbits) == sizeof(v.floatValue));
//         std::memcpy(&fbits, &v.floatValue, sizeof(fbits));
//         for (size_t b = 0; b < operandSize; ++b) {
//             code.push_back((fbits >> (8 * b)) & 0xFF);
//         }
//     } else {
//         // Treat as 32-bit int
//         for (size_t b = 0; b < operandSize; ++b) {
//             code.push_back((v.intValue >> (8 * b)) & 0xFF);
//         }
//     }
// }

//     }
for (auto &w : irrep.words) {
    code.push_back(w.opcode);
    std::size_t instrSize = instruction_size(static_cast<OpCode>(w.opcode));
    std::size_t operandSize = (instrSize > 1) ? instrSize - 1 : 0;

    // Write numeric immediates
    for (size_t i = 0; i < w.imm.size(); ++i) {
        const Value &v = w.imm[i];
        if (v.isFloat) {
            uint32_t fbits;
            std::memcpy(&fbits, &v.floatValue, sizeof(fbits));
            for (size_t b = 0; b < operandSize; ++b)
                code.push_back((fbits >> (8*b)) & 0xFF);
        } else {
            for (size_t b = 0; b < operandSize; ++b)
                code.push_back((v.intValue >> (8*b)) & 0xFF);
        }
    }

    // Write string immediates for NEW
    // for (auto &s : w.str_imm) {
    //     // Option 1: write length-prefixed string
    //     uint16_t len = static_cast<uint16_t>(s.size());
    //     code.push_back(len & 0xFF);
    //     code.push_back((len >> 8) & 0xFF);

    //     // Then the raw bytes of the string
    //     code.insert(code.end(), s.begin(), s.end());
    // }
}



    // Emit constant pool bytes
    std::vector<uint8_t> pool_bytes;
    parser.get_constpool().emit(pool_bytes);

    // Combine constant pool + code
    std::vector<uint8_t> final_bytes;
    final_bytes.insert(final_bytes.end(), pool_bytes.begin(), pool_bytes.end());
    final_bytes.insert(final_bytes.end(), code.begin(), code.end());
    
    //  Write .vmobj file
    assembler::writeObjectFile(outputObjFile, final_bytes, symtab,relocation_table);

    std::cout << "[Assembler] " << inputFile << " → " << outputObjFile << " (" 
              << final_bytes.size() << " bytes)" << std::endl;
    return true;
}
