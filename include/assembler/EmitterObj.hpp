#ifndef ASSEMBLER_EMITTEROBJ_HPP
#define ASSEMBLER_EMITTEROBJ_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "SymbolTable.hpp"

namespace assembler {

void writeObjectFile(const std::string& filename,
                     const std::vector<uint8_t>& code,
                     const SymbolTable& symtab);

} // namespace assembler

#endif // ASSEMBLER_EMITTEROBJ_HPP
