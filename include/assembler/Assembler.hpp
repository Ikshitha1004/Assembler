#ifndef ASSEMBLER_ASSEMBLER_HPP
#define ASSEMBLER_ASSEMBLER_HPP
#include <string>
#include <vector>
#include <utility>

// The Assembler class orchestrates the parsing and object file emission.
class Assembler {
public:
    // Reads a .vm source file, assembles it, and writes a .vmobj file.

    std::pair<bool,std::vector<std::string>>assemble(const std::string& inputFile, const std::string& outputObjFile);
};
#endif // ASSEMBLER_ASSEMBLER_HPP