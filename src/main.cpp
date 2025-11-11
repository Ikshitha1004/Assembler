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
    if (argc < 2) {
        cerr << "Usage:\n"
             << "  ./bin/assembler <output.vm> <input1.asm> [input2.asm ...]\n"
             << "  ./bin/assembler [input1.asm ...] --obj-only" << endl;
        return 1;
    }

    bool stopAtObj = false;
    vector<string> inputFiles;
    string outputFile;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--obj-only") {
            stopAtObj = true;
        } else {
            inputFiles.push_back(arg);
        }
    }

    // When in full mode (assemble + link)
    if (!stopAtObj) {
        if (inputFiles.size() < 2) {
            cerr << "[Error] Missing arguments. Need output and at least one input file." << endl;
            return 1;
        }
        outputFile = inputFiles[0];
        inputFiles.erase(inputFiles.begin()); // remove output from list
    }

    vector<string> objFiles;

    try {
        // Assemble each .asm → .vmobj
        for (const auto& src : inputFiles) {
            string obj = get_stem(src) + ".vmobj";
            Assembler assembler;
            auto result = assembler.assemble(src, obj);
            if (!result.first) {
                cerr << "[Error] Failed to assemble " << src << endl;
                return 1;
            }
            objFiles.push_back(obj);
            cout << "[Assembler] " << src << " → " << obj << endl;
            for (const auto& lib : result.second) {
                string lib_obj = lib + ".vmobj";
                objFiles.push_back(lib_obj);
                cout << "   [Library] Linked with: " << lib_obj << endl;
            }
        }

        if (stopAtObj) {
            cout << "[Info] Stopped after object generation (--obj-only)." << endl;
            return 0;
        }
        // Otherwise, link all .vmobj → .vm
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
