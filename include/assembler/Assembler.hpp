#pragma once
#include <string>
#include "parser.hpp"
#include "emitterObj.hpp"

// The Assembler class orchestrates the parsing and object file emission.
class Assembler {
public:
    // Reads a .vm source file, assembles it, and writes a .vmobj file.
    bool assemble(const std::string& inputFile, const std::string& outputObjFile);
};
