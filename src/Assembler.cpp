#include "assembler/Assembler.hpp"
#include "assembler/Tokenizer.hpp"
#include "assembler/Parser.hpp"
#include "assembler/EmitterObj.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>


// --- Assemble method ---
bool Assembler::assemble(const std::string& inputFile, const std::string& outputObjFile) {
    // 1️⃣ Read source
    std::ifstream in(inputFile);
    if (!in.is_open()) {
        std::cerr << "[Assembler] Cannot open " << inputFile << std::endl;
        return false;
    }
    std::string src((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    // 2️⃣ Tokenize
    Tokenizer tokenizer(src);
    auto tokens = tokenizer.tokenize();
    std::cout << "[Tokenizer] " << tokens.size() << " tokens read from " << inputFile << "\n";

    // 3️⃣ Parse
    Parser parser(tokens);
    auto instructions = parser.parse();
    const auto& symtab = parser.symbols();

    if (!parser.errors().empty()) {
        std::cerr << "[Parser] Errors in " << inputFile << ":\n";
        for (const auto &err : parser.errors())
            std::cerr << "  " << err << "\n";
        return false;
    }

    // 4️⃣ Encode instructions → bytecode
   // std::vector<uint8_t> code;
    // for (const auto& ins : instructions) {
    //     //auto encoded = encode_instruction(ins);
    //     code.insert(code.end(), encoded.begin(), encoded.end());
    // }
      // Build IR
    auto irrep = assembler::IRBuilder::build(instructions);

    std::cout << "\n=== IR WORDS ===\n";
    for (size_t i = 0; i < irrep.words.size(); ++i) {
        const auto &w = irrep.words[i];
        std::cout << i << ": opcode=0x"
                  << std::hex << std::setw(2) << std::setfill('0')
                  << (int)w.opcode << std::dec;
        for (auto v : w.imm) std::cout << " " << v;
        std::cout << "   (src line " << w.src_line << ")\n";
    }

    // Convert IR → raw bytecode
    std::vector<uint8_t> code;
    for (auto &w : irrep.words) {
        code.push_back(w.opcode);
        for (size_t i = 0; i < w.imm.size(); ++i) {
    int imm = w.imm[i];
    if (w.opcode == static_cast<uint8_t>(OpCode::JMP)|| w.opcode == static_cast<uint8_t>(OpCode::JZ) || w.opcode == static_cast<uint8_t>(OpCode::JNZ)) {
        // write 16-bit little-endian
        code.push_back(imm & 0xFF);
        code.push_back((imm >> 8) & 0xFF);
    } else {
        // write 32-bit little-endian
        for (int b = 0; b < 4; ++b)
            code.push_back((imm >> (8 * b)) & 0xFF);
    }
}
}

    // 5️⃣ Emit constant pool bytes
    std::vector<uint8_t> pool_bytes;
    parser.get_constpool().emit(pool_bytes);

    // 6️⃣ Combine constant pool + code
    std::vector<uint8_t> final_bytes;
    final_bytes.insert(final_bytes.end(), pool_bytes.begin(), pool_bytes.end());
    final_bytes.insert(final_bytes.end(), code.begin(), code.end());

    // 7️⃣ Generate relocation entries
    auto relocs = symtab.generate_relocation_table();
//add ir
    // 8️⃣ Write .vmobj file
    assembler::writeObjectFile(outputObjFile, final_bytes, symtab);

    std::cout << "[Assembler] " << inputFile << " → " << outputObjFile << " (" 
              << final_bytes.size() << " bytes)" << std::endl;
    return true;
}
