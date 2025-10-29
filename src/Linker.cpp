#include "assembler/Linker.hpp"
#include "assembler/EmitterObj.hpp" // just to keep headers consistent (not required)
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <cstring>
#include <iomanip>
#include <set>
#include<functional>
// helper read
static uint32_t read_u32(std::ifstream &in, const std::string &label = "") {
    uint32_t v;
    in.read(reinterpret_cast<char*>(&v), sizeof(v));
    if (!in) throw std::runtime_error("Unexpected EOF reading u32");
    
    // // Print the value in hexadecimal with [DEBUG] tag
    if (!label.empty())
        std::cout << "[DEBUG] " << label << ": 0x" 
                  << std::hex << v << std::dec << std::endl;
    else
        std::cout << "[DEBUG] 0x" << std::hex << v << std::dec << std::endl;

    return v;
}
void Linker::detectEntryModule() {
    for (auto &m : modules_) {
        if(m.isEntryModule){
             entryModule_ = &m; // store pointer

            std::cout << "[Linker] Entry module detected: " << m.filename << "\n";
            break;
        }
    }

    if (!entryModule_) {
        throw std::runtime_error("No entry module with main() found!");
    }
}
void print_bytes(const std::vector<uint8_t>& data, const std::string &label = "Code") {
    std::cout << "[DEBUG] " << label << " bytes in hex:" << std::endl;
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << "0x" 
                  << std::setw(2) << std::setfill('0') 
                  << std::hex << static_cast<int>(data[i]) << " ";
        if ((i + 1) % 16 == 0) std::cout << std::endl; // newline every 16 bytes
    }
    std::cout << std::dec << std::endl; // reset to decimal
}

static std::string read_string_with_len(std::ifstream &in) {
    uint32_t len = read_u32(in);
    std::string s;
    if (len) {
        s.resize(len);
        in.read(&s[0], len);
        if (!in) throw std::runtime_error("Unexpected EOF reading string");
    }
    return s;
}
//TODO:check this fns correctness over already present one
// void Linker::addObjectFile(const std::string& path) {
//     std::ifstream in(path, std::ios::binary);
//     if (!in) throw std::runtime_error("Cannot open object: " + path);

//     uint32_t magic = read_u32(in);
//     uint32_t version = read_u32(in);
//     if (version != 1) throw std::runtime_error("Unsupported object version");

//     Module mod;
//     uint32_t codeSize = read_u32(in);
//     uint32_t symtabOffset = read_u32(in);
//     uint32_t symtabSize = read_u32(in);
//     uint32_t relocOffset = read_u32(in);
//     uint32_t relocCount = read_u32(in);

//     // --- Read code ---
//     if (codeSize) {
//         mod.code.resize(codeSize);
//         in.read(reinterpret_cast<char*>(mod.code.data()), codeSize);
//     }

//     // --- Read symbol table ---
//     if (symtabOffset) {
//         in.seekg(symtabOffset);

//         // Labels
//         uint32_t labelCount = read_u32(in);
//         for (uint32_t i = 0; i < labelCount; ++i) {
//             std::string name = read_string_with_len(in);
//             uint32_t addr = read_u32(in);
//             (void)read_u32(in); (void)read_u32(in); (void)read_u32(in); // unused
//             mod.labels[name] = addr;
//         }

//         // Methods
//         uint32_t methodCount = read_u32(in);
//         for (uint32_t i = 0; i < methodCount; ++i) {
//             MethodInfo mi;
//             mi.name = read_string_with_len(in);       // fully qualified method name
//             mi.signature = read_string_with_len(in);  // method signature
//             uint32_t addr = read_u32(in);
//             uint32_t size = read_u32(in);
//             (void)read_u32(in); (void)read_u32(in);  // unused stack/locals

//             mi.address = addr; // relative to module code start
//             mi.size = size;
//             mod.methods[mi.name] = mi;
//         }

//         // Classes
//         uint32_t classCount = read_u32(in);
//         for (uint32_t i = 0; i < classCount; ++i) {
//             ClassInfo cls;
//             cls.name = read_string_with_len(in);
//             cls.super_name = read_string_with_len(in);

//             uint32_t methodCount = read_u32(in);
//             for (uint32_t j = 0; j < methodCount; ++j) {
//                 std::string methodName = read_string_with_len(in);
//                 uint32_t addr = read_u32(in);

//                 // Lookup MethodInfo already read above
//                 auto it = mod.methods.find(methodName);
//                 if (it != mod.methods.end()) {
//                     cls.methods[methodName] = it->second.address;
//                 } else {
//                     cls.methods[methodName] = addr;
//                 }
//             }

//             mod.classes[cls.name] = cls;
//         }
//     }

//     // --- Read relocations ---
//     if (relocOffset) {
//         in.seekg(relocOffset);
//         uint32_t reloCountFromFile = read_u32(in);
//         for (uint32_t i = 0; i < reloCountFromFile; ++i) {
//             RelocationEntry r;
//             r.offset = read_u32(in) + 1;
//             r.symbol_name = read_string_with_len(in);
//             (void)read_u32(in); // section
//             mod.relocs.push_back(r);
//         }
//     }

//     mod.filename = path;
//     modules_.push_back(std::move(mod));
// }

//TODO:check thsi fn
void Linker::addObjectFile(const std::string &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open object: " + path);

    uint32_t magic = read_u32(in);
    uint32_t version = read_u32(in);
    if (version != 1) throw std::runtime_error("Unsupported object version");

    Module mod;
    uint32_t codeSize = read_u32(in);
    uint32_t symtabOffset = read_u32(in);
    uint32_t symtabSize = read_u32(in);
    uint32_t relocOffset = read_u32(in);
    uint32_t relocCount = read_u32(in);

    // --- Read code ---
    if (codeSize) {
        mod.code.resize(codeSize);
        in.read(reinterpret_cast<char*>(mod.code.data()), codeSize);
    }

    // --- Read symbol table ---
    if (symtabOffset) {
        in.seekg(symtabOffset);

        // Labels
        uint32_t labelCount = read_u32(in);
        for (uint32_t i = 0; i < labelCount; ++i) {
            std::string name = read_string_with_len(in);
            uint32_t addr = read_u32(in);
            (void)read_u32(in); (void)read_u32(in); (void)read_u32(in); // unused
            mod.labels[name] = addr;
        }

        // Methods
        uint32_t methodCount = read_u32(in);
        bool hasMain = false;
        for (uint32_t i = 0; i < methodCount; ++i) {
            std::string key = read_string_with_len(in); 
             std::cout<<"key"<<key;     // fully qualified method name
            std::string shortname = read_string_with_len(in); // method short name
            std::string sig = read_string_with_len(in);       // signature
            uint32_t addr = read_u32(in);
            uint32_t size = read_u32(in);
            (void)read_u32(in); (void)read_u32(in); 
            MethodInfo mi;
            mi.address=addr;
            mi.size = size;         // unused
            mod.methods[key]= mi;

            if (key == "main") hasMain = true;
            std::cout<<"HM"<<hasMain<<std::endl;
        }

        // Mark module as entry if it has main()
        if (hasMain) {
            mod.isEntryModule = true;
        }

        // Classes
        uint32_t classCount = read_u32(in);
        for (uint32_t i = 0; i < classCount; ++i) {
            ClassInfo cls;
            cls.name = read_string_with_len(in);
            cls.super_name = read_string_with_len(in);

            uint32_t methodCount = read_u32(in);
            for (uint32_t j = 0; j < methodCount; ++j) {
                std::string methodName = read_string_with_len(in);
                uint32_t addr = read_u32(in);
                cls.methods[methodName] = addr;
            }

            mod.classes[cls.name] = cls;

            // // If entry module, all classes go to referenced_classes_
            // if (mod.is_entry_module) {
            //     referenced_classes_.insert(cls.name);
            // }
        }
    }

    // --- Read relocations ---
  if (relocOffset) {
    in.seekg(relocOffset);
    uint32_t reloCountFromFile = read_u32(in);
    for (uint32_t i = 0; i < reloCountFromFile; ++i) {
        RelocationEntry r;
        r.offset = read_u32(in) + 1;
        r.symbol_name = read_string_with_len(in);
        std::cout<<"symbol name in relocation: "<<r.symbol_name<<"\n";
        r.is_method_ref = read_u32(in); // NEW
        (void)read_u32(in); // section
        mod.relocs.push_back(r);

        // Track referenced classes ONLY for entry module and NEW opcode
        std::cout<<"is entry module"<<mod.isEntryModule<<"\n";
        std::cout<<"is method ref"<<r.is_method_ref<<"\n";
       if (mod.isEntryModule && r.is_method_ref == 2) {
        // Extract class name from symbol_name
        std::string clsName = r.symbol_name;
        // Also add its superclass if present
        auto clsIt = mod.classes.find(clsName);
        if (clsIt != mod.classes.end()) {
            const std::string &superName = clsIt->second.super_name;
            if (!superName.empty()) {
                referenced_classes_.push_back(superName);
            }
        }
        std::cout << "[Linker] Referenced class from relocation: " << clsName << "\n";
        // Add class itself
        referenced_classes_.push_back(clsName);


   
}
    }
}
       std::cout << "[Linker] Loaded module " << path
              << " code=" << codeSize
              << " labels=" << mod.labels.size()
              << " methods=" << mod.methods.size()
              << " classes=" << mod.classes.size()
              << " relocs=" << mod.relocs.size()
              << (mod.isEntryModule ? " [ENTRY MODULE]" : "")
              << "\n";
    mod.filename = path;
    modules_.push_back(std::move(mod));

 
}



void Linker::assignBaseAddresses() {
    uint32_t cur = 0;
    for (auto &m : modules_) {
        m.base_addr = cur;
        cur += static_cast<uint32_t>(m.code.size());  // contiguous, no gaps
    }
}

void Linker::buildGlobalSymbolTable() {
    uint32_t classIndex = 0;

    if (entryModule_) {
        // Assign indices for entry module classes first
        for (const auto &ckv : entryModule_->classes) {
            const auto &clsName = ckv.first;
            if (!class_indices_.count(clsName)) {
                class_indices_[clsName] = classIndex++;
                index_to_class_.push_back(clsName);
            }
        }
    }
    //TODO: Assign indices for referenced classes from entry moduel only and not all classes
    for (const auto &clsName : referenced_classes_) {
        if (!class_indices_.count(clsName)) {
            class_indices_[clsName] = classIndex++;
            index_to_class_.push_back(clsName);
        }
    }
    // Assign indices for other modules
    for (const auto &m : modules_) {
        if (&m == entryModule_) continue;

        for (const auto &ckv : m.classes) {
            const auto &clsName = ckv.first;
            if (!class_indices_.count(clsName)) {
                class_indices_[clsName] = classIndex++;
                index_to_class_.push_back(clsName);
            }
        }
    }

    // Merge labels & methods into global symbols (unchanged)
    for (const auto &m : modules_) {
        for (const auto &kv : m.labels)
            global_symbols_[kv.first] = m.base_addr + kv.second;
        for (const auto &kv : m.methods)
            global_symbols_[kv.first] = m.base_addr + kv.second.address;
        for (const auto &ckv : m.classes) {
            const auto &cls = ckv.second;
            for (const auto &meth : cls.methods)
                global_symbols_[meth.first] = m.base_addr + meth.second;
        }
    }

    std::cout << "[Linker] Assigned " << classIndex << " class indices\n";
}


static void patch_bytes(std::vector<uint8_t> &code, size_t offset, uint32_t value, size_t bytes) {
    if (offset + bytes > code.size()) throw std::runtime_error("Patch out of bounds");
    for (size_t i = 0; i < bytes; ++i) {
        code[offset + i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
    }
    //print_bytes(code, "Patched Code");
}

// void Linker::applyRelocations() {
//     for (auto &m : modules_) {
//         std::cout << "\n=== Processing module ===\n";

//         for (const auto &r : m.relocs) {
//             std::cout << "Relocation for symbol: " << r.symbol_name
//                       << " at offset: " << r.offset << "\n";

//             auto it = global_symbols_.find(r.symbol_name);
//             std::cout << "  Looking up symbol: " << r.symbol_name << "\n";
//             if (it == global_symbols_.end())
//                 throw std::runtime_error("Unresolved symbol: " + r.symbol_name);

//             uint32_t target = it->second;
//             uint32_t operandOffset = r.offset;
//             std::cout<<"opr offset: "<<operandOffset<<"\n";
//             if (operandOffset == 0 || operandOffset - 1 >= m.code.size())
//                 throw std::runtime_error("Relocation offset invalid");
//                 std::cout << "  Module base address: 0x" 
//                           << std::hex << m.base_addr << std::dec << "\n";
//             for(int i=0;i<m.code.size();i++){
//                 std::cout << std::hex << std::setw(2) << std::setfill('0')
//                           << static_cast<int>(m.code[i]) << " ";
//                 if ((i + 1) % 16 == 0) std::cout << std::endl; // newline every 16 bytes
//             }
//             // uint8_t opcode = m.code[operandOffset-1];
//             // size_t patchBytes = 4;
//             // if (opcode == 0x30 || opcode == 0x31 || opcode == 0x32)
//             //     patchBytes = 2;
//             uint8_t opcode = m.code[operandOffset - 1];
//             auto op = static_cast<OpCode>(opcode);

//             // Determine operand size dynamically
//             std::size_t instrSize = instruction_size(op);
//             std::size_t patchBytes = (instrSize > 1) ? instrSize - 1 : 0;

//             // Sanity check: must have something to patch
//             if (patchBytes == 0) {
//                 throw std::runtime_error("Linker error: opcode at relocation site has no operand to patch");
//             }


//             // std::cout << "  Opcode before operand: 0x"
//             //           << std::hex << static_cast<int>(opcode)
//             //           << "  -> patching " << std::dec << patchBytes
//             //           << " bytes with target " << target << "\n";

//             // std::cout << "  Code before patch: ";
//             // for (size_t i = operandOffset; i < operandOffset + patchBytes && i < m.code.size(); ++i)
//             //     std::cout << std::hex << std::setw(2) << std::setfill('0')
//             //               << static_cast<int>(m.code[i]) << " ";
//             // std::cout << "\n";

//             patch_bytes(m.code, operandOffset, target, patchBytes);

//             // std::cout << "  Code after patch:  ";
//             // for (size_t i = operandOffset; i < operandOffset + patchBytes && i < m.code.size(); ++i)
//             //     std::cout << std::hex << std::setw(2) << std::setfill('0')
//             //               << static_cast<int>(m.code[i]) << " ";
//             // std::cout << "\n";
//         }
//     }
// }
void Linker::applyRelocations() {
    for (auto &m : modules_) {
        std::cout << "\n=== Processing module: " << m.filename << " ===\n";

        for (auto &r : m.relocs) {
            uint32_t operandOffset = r.offset;
            if (operandOffset == 0 || operandOffset - 1 >= m.code.size())
                throw std::runtime_error("Relocation offset invalid");

            uint8_t opcode = m.code[operandOffset - 1];
            auto op = static_cast<OpCode>(opcode);
            std::size_t instrSize = instruction_size(op);
            std::size_t patchBytes = (instrSize > 1) ? instrSize - 1 : 0;

            if (patchBytes == 0) {
                throw std::runtime_error("Linker error: opcode at relocation site has no operand to patch");
            }

            // --- NEW: patch NEW opcode with class index ---
            if (opcode == 0x50) { 
                std::string clsName = r.symbol_name;
                if (!class_indices_.count(clsName))
                    throw std::runtime_error("Unknown class for NEW: " + clsName);
                uint32_t clsIndex = class_indices_[clsName];
                std::cout<<"Patching NEW for class " << clsName << " with index " << clsIndex << "\n";
                patch_bytes(m.code, operandOffset, clsIndex, patchBytes);
                continue; // skip normal symbol patching for NEW
            }

            // --- Original relocation logic for functions/labels ---
            auto it = global_symbols_.find(r.symbol_name);
            if (it == global_symbols_.end())
                throw std::runtime_error("Unresolved symbol: " + r.symbol_name);

            uint32_t target = it->second;
            patch_bytes(m.code, operandOffset, target, patchBytes);
        }
    }
}



void Linker::mergeModules(std::vector<uint8_t> &outcode) {
    uint32_t mergedBase = 0; // running absolute offset in outcode
    std::unordered_map<std::string, uint32_t> method_abs_offset;

  
     if (!entryModule_) throw std::runtime_error("No entry module found");
    outcode.insert(outcode.end(), entryModule_->code.begin(), entryModule_->code.end());
    for (auto &methPair : entryModule_->methods) {
                MethodInfo &mi = methPair.second;
                mi.address = mi.address; // already absolute in entry module?
                method_abs_offset[methPair.first] = mi.address;
            }

    mergedBase = static_cast<uint32_t>(outcode.size());
   

    // --- Step 2: Recursively collect referenced classes + superclasses ---
    std::set<std::string> all_classes;
    std::function<void(const std::string&)> collect_class = [&](const std::string &clsName) {
        if (all_classes.count(clsName)) return;
       

        // Find superclass
        for (auto &m : modules_) {
            auto it = m.classes.find(clsName);
            if (it != m.classes.end()) {
                const ClassInfo &cls = it->second;
                if (!cls.super_name.empty()) collect_class(cls.super_name);
            }
        }
         all_classes.insert(clsName);
    };

    for (const auto &clsName : referenced_classes_) {
        collect_class(clsName);
    }
    std::cout << "[Linker] Total classes to merge: " << all_classes.size() << "\n";
    // --- Step 3: Copy methods of all_classes from modules ---
    for (const auto &clsName : all_classes) {
        for (auto &m : modules_) {
            auto it = m.classes.find(clsName);
            if (it != m.classes.end()) {
                const ClassInfo &cls = it->second;
                for (const auto &methPair : cls.methods) {
                    const std::string &methodName = methPair.first;
                    std::cout << "Merging method: " << methodName << "\n";
                    auto mit = m.methods.find(methodName);
                    if (mit == m.methods.end())
                        throw std::runtime_error("Method info not found: " + methodName);

                    MethodInfo &mi = mit->second;

                    // Compute relative offset in module code
                    if (mi.address + mi.size > m.code.size())
                        throw std::runtime_error("Method size exceeds module code bounds: " + methodName);

                    // Copy bytes
                    outcode.insert(outcode.end(),
                                   m.code.begin() + mi.address,
                                   m.code.begin() + mi.address + mi.size);

                    // Store absolute offset for later (e.g., class metadata section)
                    method_abs_offset[methodName] = mergedBase;
                    mi.address = mergedBase; // update absolute address
                    mergedBase += mi.size;
                }
            }
        }
    }
     for (auto &m : modules_) {
        for (auto &ckv : m.classes) {
            ClassInfo &cls = ckv.second;
            for (auto &methPair : cls.methods) {
                const std::string &methodName = methPair.first;
                if (method_abs_offset.count(methodName)) {
                    methPair.second = method_abs_offset[methodName]; // absolute offset
                }
            }
        }
    }
}

void write_u32(std::vector<uint8_t> &out, uint32_t val) {
    out.push_back(val & 0xFF);
    out.push_back((val >> 8) & 0xFF);
    out.push_back((val >> 16) & 0xFF);
    out.push_back((val >> 24) & 0xFF);
}


void Linker::writeFinalVM(const std::string& outPath,
                          const std::vector<uint8_t>& finalCode,
                          uint32_t entryPoint) {
    std::vector<uint8_t> classMetaSection;

    // --- Build class metadata section ---
    uint32_t classCount = static_cast<uint32_t>(index_to_class_.size());
    write_u32(classMetaSection, classCount); // Number of classes

    for (uint32_t idx = 0; idx < classCount; ++idx) {
        const std::string &clsName = index_to_class_[idx];

        // Find class info
        ClassInfo *clsPtr = nullptr;
        for (auto &m : modules_) {
            auto it = m.classes.find(clsName);
            if (it != m.classes.end()) {
                clsPtr = &it->second;
                break;
            }
        }
        if (!clsPtr) throw std::runtime_error("Class not found: " + clsName);
        ClassInfo &cls = *clsPtr;
        //TODO:check the vm complatibilty of this section(writing of  name)
        // --- Class name ---
        classMetaSection.push_back(static_cast<uint8_t>(cls.name.size()));
        classMetaSection.insert(classMetaSection.end(), cls.name.begin(), cls.name.end());

        // --- Superclass index ---
        int32_t superIdx = -1;
        if (!cls.super_name.empty()) {
            auto it = class_indices_.find(cls.super_name);
            if (it != class_indices_.end()) superIdx = static_cast<int32_t>(it->second);
        }
        write_u32(classMetaSection, static_cast<uint32_t>(superIdx));

        // --- Fields count ---
        write_u32(classMetaSection, 0);

        // --- Methods ---
        write_u32(classMetaSection, static_cast<uint32_t>(cls.methods.size()));
        for (const auto &methPair : cls.methods) {
            const std::string &mName = methPair.first;
            uint32_t absOffset = methPair.second;

            classMetaSection.push_back(static_cast<uint8_t>(mName.size()));
            classMetaSection.insert(classMetaSection.end(), mName.begin(), mName.end());
            write_u32(classMetaSection, absOffset);
        }
    }

    // --- Prepare header ---
    std::ofstream out(outPath, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open final VM file");

    uint32_t codeSize = static_cast<uint32_t>(finalCode.size());
    uint32_t classMetaSize = static_cast<uint32_t>(classMetaSection.size());
    uint32_t globalsOffset = 44 + codeSize;
    uint32_t classMetaOffset = globalsOffset;

    uint8_t header[44] = {0};

    // --- Magic bytes ---
    header[0] = 0x56; header[1] = 0x4D; header[2] = 0x00; header[3] = 0x01; // "VM\1"

    // --- Version ---
    uint32_t version = 1;
    std::memcpy(header + 4, &version, 4);

    // --- ✅ Entry point (written to bytes 8–11) ---
    std::memcpy(header + 8, &entryPoint, 4);

    // --- Const pool (none for now) ---
    uint32_t constPoolOffset = 44;
    uint32_t constPoolSize = 0;
    std::memcpy(header + 12, &constPoolOffset, 4);
    std::memcpy(header + 16, &constPoolSize, 4);

    // --- Code section ---
    uint32_t codeOffset = 44;
    std::memcpy(header + 20, &codeOffset, 4);
    std::memcpy(header + 24, &codeSize, 4);

    // --- Globals (none) ---
    uint32_t globalsSize = 0;
    std::memcpy(header + 28, &globalsOffset, 4);
    std::memcpy(header + 32, &globalsSize, 4);

    // --- Class metadata section ---
    std::memcpy(header + 36, &classMetaOffset, 4);
    std::memcpy(header + 40, &classMetaSize, 4);

    // --- Write all ---
    out.write(reinterpret_cast<char*>(header), sizeof(header));
    if (!finalCode.empty())
        out.write(reinterpret_cast<const char*>(finalCode.data()), finalCode.size());
    if (!classMetaSection.empty())
        out.write(reinterpret_cast<const char*>(classMetaSection.data()), classMetaSection.size());
    out.close();

    std::cout << "[Linker] Wrote final VM file: " << outPath
              << " | entryPoint=" << entryPoint
              << " | code=" << codeSize
              << " | classMeta=" << classMetaSize << "\n";
}
void Linker::link(const std::string& outputVmPath) {
    if (modules_.empty())
        throw std::runtime_error("No input modules");

    assignBaseAddresses();
    detectEntryModule();
    buildGlobalSymbolTable();
    applyRelocations();

    std::vector<uint8_t> finalCode;
    mergeModules(finalCode);

    uint32_t entryPoint = 0;

    // --- Detect main() in the entry module ---
     if (entryModule_) {
        //print entry module methods
        for (const auto &m : entryModule_->methods) {
            std::cout << "   - " << m.first << " (offset=" << m.second.address << ")\n";
        }
        auto it = entryModule_->methods.find("main");
        if (it != entryModule_->methods.end()) {
            entryPoint = it->second.address;  // Absolute address already assigned
            std::cout << "[Linker] Found global 'main' at offset " << entryPoint << "\n";
        } else {
            std::cerr << "[Linker] Warning: 'main' not found in entry module\n";
        }
    } else {
        std::cerr << "[Linker] Warning: No entry module detected\n";
    }

    if (entryPoint == 0)
        std::cerr << "[Linker] Warning: No 'main' method found, defaulting entryPoint=0\n";

    // --- Write final binary with correct header entry point ---
    writeFinalVM(outputVmPath, finalCode, entryPoint);
}
