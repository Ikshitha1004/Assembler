#include <iostream>
#include <iomanip>
#include "assembler/Tokenizer.hpp"
#include "assembler/Parser.hpp"
#include "assembler/Utils.hpp"
#include "assembler/SymbolTable.hpp"
#include "assembler/IR.hpp"
#include "assembler/Emitter.hpp"
#include "assembler/ConstantPool.hpp"
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
