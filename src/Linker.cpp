#include "assembler/Linker.hpp"
#include "assembler/EmitterObj.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <cstring>
#include <iomanip>
#include <queue>
#include <algorithm>
#include <functional>
#include <set>

// --------------------------- helpers ---------------------------
static uint32_t read_u32(std::ifstream &in, const std::string &label = "")
{
    uint32_t v;
    in.read(reinterpret_cast<char *>(&v), sizeof(v));
    // if (!in)
    //     throw std::runtime_error("Unexpected EOF reading u32");
    // if (!label.empty())
    //     std::cout << "[DEBUG] " << label << ": 0x" << std::hex << v << std::dec << std::endl;
    // else
    //     std::cout << "[DEBUG] 0x" << std::hex << v << std::dec << std::endl;
    return v;
}

static std::string read_string_with_len(std::ifstream &in)
{
    uint32_t len = read_u32(in);
    std::string s;
    if (len)
    {
        s.resize(len);
        in.read(&s[0], len);
        if (!in)
            throw std::runtime_error("Unexpected EOF reading string");
    }
    return s;
}

static void print_bytes(const std::vector<uint8_t> &data, const std::string &label = "Code")
{
    //std::cout << "[DEBUG] " << label << " bytes in hex:" << std::endl;
    for (size_t i = 0; i < data.size(); ++i)
    {
        std::cout << "0x"
                  << std::setw(2) << std::setfill('0')
                  << std::hex << static_cast<int>(data[i]) << " ";
        if ((i + 1) % 16 == 0)
            std::cout << std::endl;
    }
    std::cout << std::dec << std::endl;
}

static void patch_bytes(std::vector<uint8_t> &code, size_t offset, uint32_t value, size_t bytes)
{
    if (offset + bytes > code.size())
        throw std::runtime_error("Patch out of bounds");
    for (size_t i = 0; i < bytes; ++i)
    {
        code[offset + i] = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
    }
}

// --------------------------- addObjectFile ---------------------------
void Linker::addObjectFile(const std::string &path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in)
        throw std::runtime_error("Cannot open object: " + path);

    uint32_t magic = read_u32(in);
    (void)magic;
    uint32_t version = read_u32(in);
    if (version != 1)
        throw std::runtime_error("Unsupported object version");

    Module mod;
    uint32_t codeSize = read_u32(in);
    uint32_t symtabOffset = read_u32(in);
    uint32_t symtabSize = read_u32(in);
    uint32_t relocOffset = read_u32(in);
    uint32_t relocCount = read_u32(in);
    (void)symtabSize;
    (void)relocCount;

    // --- Read code ---
    if (codeSize)
    {
        mod.code.resize(codeSize);
        in.read(reinterpret_cast<char *>(mod.code.data()), codeSize);
    }

    // --- Read symbol table ---
    if (symtabOffset)
    {
        in.seekg(symtabOffset);

        // Labels
        uint32_t labelCount = read_u32(in);
        for (uint32_t i = 0; i < labelCount; ++i)
        {
            std::string name = read_string_with_len(in);
            uint32_t addr = read_u32(in);
            (void)read_u32(in);
            (void)read_u32(in);
            (void)read_u32(in);
            mod.labels[name] = addr;
        }

        // Methods
        uint32_t methodCount = read_u32(in);
        bool hasMain = false;
        for (uint32_t i = 0; i < methodCount; ++i)
        {
            std::string fullName = read_string_with_len(in);
            //std::cout << "[ENTRY MOD] Reading method: " << fullName << std::endl;
            std::string shortname = read_string_with_len(in);
            std::string sig = read_string_with_len(in);
            uint32_t addr = read_u32(in);
            uint32_t size = read_u32(in);
            (void)read_u32(in);
            (void)read_u32(in);
            MethodInfo mi;
            mi.address = addr;
            mi.size = size;
            mod.methods[fullName] = mi;
            if (fullName == "main")
                hasMain = true;
        }
        if (hasMain)
            mod.isEntryModule = true;

        // Classes
        uint32_t classCount = read_u32(in);
        for (uint32_t i = 0; i < classCount; ++i)
        {
            ClassInfo cls;
            cls.name = read_string_with_len(in);
            //std::cout << "[DEBUG] Reading class: " << cls.name << std::endl;
            cls.super_name = read_string_with_len(in);
            //std::cout << "[DEBUG] Super class: " << cls.super_name << std::endl;
            uint32_t methodCount = read_u32(in);
            for (uint32_t j = 0; j < methodCount; ++j)
            {
                std::string methodName = read_string_with_len(in);
                uint32_t addr = read_u32(in);
                cls.methods.push_back({methodName,addr});
            }

            uint32_t fieldCount = read_u32(in);
            cls.field_count = static_cast<int>(fieldCount);
            for (uint32_t j = 0; j < fieldCount; ++j)
            {
                std::string fieldName = read_string_with_len(in);
                std::string ownerClass = read_string_with_len(in);
                std::string descriptor = read_string_with_len(in);
                uint32_t index = read_u32(in);
                FieldInfo fi;
                fi.name = fieldName;
                fi.owner_class = ownerClass;
                fi.descriptor = descriptor;
                fi.index = index;
                cls.fields.push_back({fieldName ,fi});
            }

            mod.classes[cls.name] = cls;
        }
    }

    // --- Read relocations ---
    if (relocOffset)
    {
        in.seekg(relocOffset);
        uint32_t reloCountFromFile = read_u32(in);
        for (uint32_t i = 0; i < reloCountFromFile; ++i)
        {
            RelocationEntry r;
            r.offset = read_u32(in) + 1;
            r.symbol_name = read_string_with_len(in);
            r.is_method_ref = read_u32(in);
            (void)read_u32(in);
            mod.relocs.push_back(r);
        }
    }

    // --- build exports/imports for DCE ---
    for (const auto &kv : mod.labels)
        mod.exports.insert(kv.first);
    for (const auto &kv : mod.methods)
        mod.exports.insert(kv.first);
    for (const auto &ckv : mod.classes)
    {
        mod.exports.insert(ckv.first);
        for (const auto &mth : ckv.second.methods)
            mod.exports.insert(mth.first);
    }
    for (const auto &r : mod.relocs)
    {
        if (!r.symbol_name.empty())
            mod.imports.insert(r.symbol_name);
    }

    mod.filename = path;
    modules_.push_back(std::move(mod));

    std::cout << "[Linker] Loaded module " << path
              << " code=" << codeSize
              << " labels=" << modules_.back().labels.size()
              << " methods=" << modules_.back().methods.size()
              << " classes=" << modules_.back().classes.size()
              << " relocs=" << modules_.back().relocs.size()
              << (modules_.back().isEntryModule ? " [ENTRY MODULE]" : "")
              << "\n";
}

// --------------------------- detectEntryModule ---------------------------
void Linker::detectEntryModule()
{
    entryModule_ = nullptr;
    for (auto &m : modules_)
    {
        if (m.isEntryModule)
        {
            entryModule_ = &m;
            std::cout << "[Linker] Entry module detected: " << m.filename << "\n";
            break;
        }
    }
    // if (!entryModule_)
    // {
    //     throw std::runtime_error("No entry module with main() found!");
    // }
}

// --------------------------- assignBaseAddressesAndOrder ---------------------------
void Linker::assignBaseAddressesAndOrder()
{
    // If entry module not yet detected, fallback to sequential order
    if (!entryModule_)
    {
        std::cout << "[Linker] No entry module detected — linking modules in input order.\n";

        uint32_t curBase = 0;
        for (auto &m : modules_)
        {
            m.live = true;
            m.base_addr = curBase;
            curBase += static_cast<uint32_t>(m.code.size());
        }

        std::cout << "[Linker] Assigned base addresses sequentially (no dependency reordering):\n";
        for (auto &m : modules_)
        {
            std::cout << "  " << m.filename << " base=" << m.base_addr << " size=" << m.code.size() << "\n";
        }
        return;
    }

    // ----------- Normal dependency-based reordering path -----------
    std::string entryFilename = entryModule_->filename;

    // Build provider map: symbol -> Module*
    std::unordered_map<std::string, Module *> provider;
    for (auto &m : modules_)
    {
        for (const auto &sym : m.exports)
        {
            if (provider.find(sym) == provider.end())
                provider[sym] = &m;
        }
    }

    // Build adjacency: module -> dependencies
    std::unordered_map<Module *, std::set<Module *>> adj;
    for (auto &m : modules_)
    {
        for (const auto &imp : m.imports)
        {
            auto it = provider.find(imp);
            if (it != provider.end() && it->second != &m)
            {
                adj[&m].insert(it->second);
            }
        }
    }

    // DFS topo sort
    std::vector<Module *> ordered;
    std::unordered_map<Module *, int> state; // 0 unvisited, 1 visiting, 2 visited

    std::function<void(Module *)> dfs = [&](Module *mod)
    {
        if (!mod || state[mod] == 2) return;
        if (state[mod] == 1) return; // cycle
        state[mod] = 1;
        auto it = adj.find(mod);
        if (it != adj.end())
        {
            for (Module *dep : it->second)
                dfs(dep);
        }
        state[mod] = 2;
        ordered.push_back(mod);
    };

    dfs(entryModule_);

    // Include any unvisited but needed modules
    for (auto &m : modules_)
    {
        if (state[&m] == 0)
        {
            bool needed = false;
            for (auto &visited : ordered)
            {
                auto it = adj.find(visited);
                if (it != adj.end() && it->second.count(&m))
                {
                    needed = true;
                    break;
                }
            }
            if (needed)
                dfs(&m);
        }
    }

    std::reverse(ordered.begin(), ordered.end());

    // Mark and reorder
    std::set<Module *> liveSet(ordered.begin(), ordered.end());
    std::vector<Module> newOrder;
    for (Module *pm : ordered)
    {
        for (size_t i = 0; i < modules_.size(); ++i)
        {
            if (&modules_[i] == pm)
            {
                newOrder.push_back(std::move(modules_[i]));
                break;
            }
        }
    }
    modules_ = std::move(newOrder);

    // Assign base addresses
    uint32_t curBase = 0;
    for (auto &m : modules_)
    {
        m.base_addr = curBase;
        m.live = true;
        curBase += static_cast<uint32_t>(m.code.size());
    }

    std::cout << "[Linker] Assigned base addresses (dependency order):\n";
    for (auto &m : modules_)
    {
        std::cout << "  " << m.filename << " base=" << m.base_addr << " size=" << m.code.size() << "\n";
    }
}

std::size_t Linker::preparePatch(const Module &m, uint32_t relOffset)
{
    if (relOffset >= m.code.size())
        throw std::runtime_error("Invalid patch offset in " + m.filename);
    uint8_t opcode = (relOffset > 0) ? m.code[relOffset - 1] : 0;

    std::cout << "[Linker] Preparing patch at offset " << relOffset
              << " opcode 0x" << std::hex << static_cast<int>(opcode)
              << std::dec << "\n";
    auto op = static_cast<OpCode>(opcode);
    std::size_t instrSize = instruction_size(op);
    if (instrSize <= 1)
        throw std::runtime_error(
            "Opcode at patch site has no operand to patch in " + m.filename);
    std::size_t patchBytes = instrSize - 1;

    if (relOffset + patchBytes > m.code.size())
        throw std::runtime_error("Patch range out of bounds in " + m.filename);

    return patchBytes;
}

// --------------------------- buildGlobalSymbolTable ---------------------------
void Linker::buildGlobalSymbolTable()
{
    class_indices_.clear();
    index_to_class_.clear();
    global_symbols_.clear();

    uint32_t classIndex = 0;

    // Entry module classes first (if entry exists)
    if (entryModule_)
    {
        for (const auto &ckv : entryModule_->classes)
        {
            const auto &clsName = ckv.first;
            if (!class_indices_.count(clsName))
            {
                class_indices_[clsName] = classIndex++;
                index_to_class_.push_back(clsName);
            }
        }
    }

    // referenced_classes_ (compat)
    // for (const auto &clsName : referenced_classes_)
    // {
    //     if (!class_indices_.count(clsName))
    //     {
    //         class_indices_[clsName] = classIndex++;
    //         index_to_class_.push_back(clsName);
    //     }
    // }

    // Classes from remaining (live) modules in modules_ order
    for (const auto &m : modules_)
    {
        // modules_ was already filtered to live-only in assignBaseAddressesAndOrder
        for (const auto &ckv : m.classes)
        {
            const auto &clsName = ckv.first;
            if (!class_indices_.count(clsName))
            {
                class_indices_[clsName] = classIndex++;
                index_to_class_.push_back(clsName);
            }
        }
    }

    // Merge labels & methods into global_symbols_
    for (auto &m : modules_)
    {
        for (const auto &kv : m.labels)
        {
            GlobalSymbol gs{m.base_addr + kv.second, const_cast<Module *>(&m)};
            global_symbols_[kv.first] = gs;
        }
        for (const auto &kv : m.methods)
        {
            std::cout<<"[Linker] Method gotten"<<kv.first<<" BASE addr "<<(kv.second.address)<<"\n";
            GlobalSymbol gs{m.base_addr + kv.second.address, const_cast<Module *>(&m)};
            global_symbols_[kv.first] = gs;
        }
        for (const auto &ckv : m.classes)
        {
            // const auto &cls = ckv.second;
            // for (const auto &meth : cls.methods)
            // {
            //     std::cout<<"[Linker] Class Method gotten "<<meth.first<<" BASE addr "<<(meth.second)<<"\n";
            //     GlobalSymbol gs{m.base_addr + meth.second, const_cast<Module *>(&m)};
            //     global_symbols_[meth.first] = gs;
            // }
        }
    }

    std::cout << "[Linker] Built global symbol table: " << global_symbols_.size() << " symbols\n";
    std::cout << "[Linker] Assigned " << classIndex << " class indices\n";
}

// --------------------------- applyRelocations ---------------------------
void Linker::applyRelocations()
{
    for (auto &m : modules_)
    {
        std::cout << "\n=== Processing relocations for module: " << m.filename << " ===\n";

        for (auto &r : m.relocs)
        {
            uint32_t operandOffset = r.offset;
            std::size_t patchBytes = preparePatch(m, operandOffset);
            uint8_t opcode = m.code[operandOffset - 1];
            // --- Special handling for NEW opcode (class index patch) ---
            if (opcode == 0x50)
            {
                std::string clsName = r.symbol_name;
                std::cout << "[Linker] Relocation for NEW opcode, class name: " << clsName << "\n";
                if (!class_indices_.count(clsName))
                    throw std::runtime_error("Unknown class for NEW: " + clsName);
                uint32_t clsIndex = class_indices_[clsName];
                std::cout << "[Linker] Class " << clsName << " has index " << clsIndex << "\n";
                std::cout << "[Linker] Patching NEW for class " << clsName << " with idx " << clsIndex << "\n";
                patch_bytes(m.code, operandOffset, clsIndex, patchBytes);
                continue;
            }
             if (opcode == 0x33)
            {
                auto it = global_symbols_.find(r.symbol_name);
                if (it == global_symbols_.end())
                    throw std::runtime_error("Unresolved symbol in CALL: " + r.symbol_name + " in module " + m.filename);

                uint32_t target = it->second.address;

                // Force patch only the first 4 bytes (target address)
                std::size_t callPatchBytes = std::min<std::size_t>(4, patchBytes);

                std::cout << "[Linker] Patching CALL target for symbol " << r.symbol_name
                          << " with addr " << target << " (only first 4 bytes)\n";

                patch_bytes(m.code, operandOffset, target, callPatchBytes);

                // Skip normal relocation logic
                continue;
            }
            // --- Normal relocations (symbol references) ---
            auto it = global_symbols_.find(r.symbol_name);
            if (it == global_symbols_.end())
                throw std::runtime_error("Unresolved symbol: " + r.symbol_name + " in module " + m.filename);

            uint32_t target = it->second.address;
            patch_bytes(m.code, operandOffset, target, patchBytes);
        }
    }
}

void Linker::patchModuleSymbols(Module &m)
{
    std::cout << "\n=== Patching intra-module symbols for " << m.filename << " ===\n";

    for (const auto &lbl : m.labels)
    {
        const std::string &label = lbl.first;
        uint32_t offset = lbl.second;
        uint32_t absAddr = m.base_addr + offset;

        std::cout << "[Label] " << label << " -> absolute " << absAddr << "\n";
        std::size_t patchBytes = preparePatch(m, offset);
        patch_bytes(m.code, offset, absAddr, patchBytes);
    }

    for (const auto &meth : m.methods)
    {
        const std::string &name = meth.first;
        const MethodInfo &method = meth.second;
        std::cout<<"[Linker] Method addr "<<method.address<<"\n";
        uint32_t absAddr = m.base_addr + method.address;

        std::cout << "[Method] " << name << " -> absolute " << absAddr << "\n";
        std::size_t patchBytes = preparePatch(m, method.address);
        patch_bytes(m.code, method.address, absAddr, patchBytes);
    }

    for (const auto &clsPair : m.classes)
    {
        const std::string &clsName = clsPair.first;
        const ClassInfo &cls = clsPair.second;
        auto trim_method_name = [](const std::string &full) -> std::string {
            size_t dot = full.find('.');
            if (dot != std::string::npos && dot + 1 < full.size()) {
                return full.substr(dot + 1);
            }
            return full;
        };
        for (const auto &methPair : cls.methods)
        {
            const std::string &methName = methPair.first;
            std::string mName =trim_method_name(methName); 
            uint32_t methOffset = methPair.second;
            uint32_t absAddr = m.base_addr + methOffset;

            std::cout << "[ClassMethod] " << clsName << "::" << methName
                      << " -> absolute " << absAddr << "\n";

            std::size_t patchBytes = preparePatch(m, methOffset);
            patch_bytes(m.code, methOffset, absAddr, patchBytes);
        }
    }
}


// --------------------------- mergeModules ---------------------------
void Linker::mergeModules(std::vector<uint8_t> &outcode)
{
    outcode.clear();
    for (auto &m : modules_)
    {
        // if(!m.isEntryModule)
        // patchModuleSymbols(m);

    if (!m.code.empty())
    {
        outcode.insert(outcode.end(), m.code.begin(), m.code.end());
    }
}
std::cout << "[Linker] Merged " << outcode.size() << " bytes of code from live modules\n";
}

// --------------------------- writeFinalVM ---------------------------
static void write_u32(std::vector<uint8_t> &out, uint32_t val)
{
    out.push_back(val & 0xFF);
    out.push_back((val >> 8) & 0xFF);
    out.push_back((val >> 16) & 0xFF);
    out.push_back((val >> 24) & 0xFF);
}
uint32_t get_type_code(const std::string &descriptor)
{

    if (descriptor == "I")
       return (uint32_t) 1;
    else if (descriptor == "F")
        return (uint32_t) 3;
    else if (descriptor == "C")
       return (uint32_t) 4;
    else
        return (uint32_t) 2;
}
void Linker::writeFinalVM(const std::string &outPath,
                          const std::vector<uint8_t> &finalCode,
                          uint32_t entryPoint)
{
    std::vector<uint8_t> classMetaSection;

    uint32_t classCount = static_cast<uint32_t>(index_to_class_.size());
    write_u32(classMetaSection, classCount);

    for (uint32_t idx = 0; idx < classCount; ++idx)
    {
        const std::string &clsName = index_to_class_[idx];

        ClassInfo *clsPtr = nullptr;
        for (auto &m : modules_)
        {
            auto it = m.classes.find(clsName);
            if (it != m.classes.end())
            {
                clsPtr = &it->second;
                break;
            }
        }
        if (!clsPtr)
            throw std::runtime_error("Class not found for metadata: " + clsName);

        ClassInfo &cls = *clsPtr;

        // Class name
        if (cls.name.size() > 255)
            throw std::runtime_error("Class name too long for metadata");
        classMetaSection.push_back(static_cast<uint8_t>(cls.name.size()));
        classMetaSection.insert(classMetaSection.end(), cls.name.begin(), cls.name.end());

        // Superclass index
        int32_t superIdx = -1;
        if (!cls.super_name.empty())
        {
            auto it = class_indices_.find(cls.super_name);
            if (it != class_indices_.end())
                superIdx = static_cast<int32_t>(it->second);
        }
        write_u32(classMetaSection, static_cast<uint32_t>(superIdx));

        // Fields
        write_u32(classMetaSection, static_cast<uint32_t>(cls.field_count));
        //reverse(cls.fields.begin(), cls.fields.end());
     

        for (const auto &fieldPair : cls.fields)
        {
            const std::string &fName = fieldPair.first;
            const FieldInfo &fInfo = fieldPair.second;
            uint8_t ftype = static_cast<uint8_t>(get_type_code(fInfo.descriptor));

            std::cout << "[Linker] Class Field " << fName
                    << " idx " << fInfo.index
                    << " type " << static_cast<int>(ftype) << "\n";

            if (fName.size() > 255)
                throw std::runtime_error("Field name too long for metadata");

            classMetaSection.push_back(static_cast<uint8_t>(fName.size()));
            classMetaSection.insert(classMetaSection.end(), fName.begin(), fName.end());

            // ✅ write type as single byte instead of u32
            classMetaSection.push_back(ftype);
        }

        
        // Methods
        write_u32(classMetaSection, static_cast<uint32_t>(cls.methods.size()));
        //reverse(cls.methods.begin(), cls.methods.end());
        // for (const auto &methPair : cls.methods)
        // {
        //     const std::string &mName = methPair.first;
        //     auto it = global_symbols_.find(mName);
        //     if (it == global_symbols_.end())
        //         throw std::runtime_error("Method not found for metadata: " + mName);
        //     uint32_t absOffset = global_symbols_[mName].address;

        //     std::cout << "[Linker] Class Method " << mName << " addr " << absOffset << "\n";

        //     if (mName.size() > 255)
        //         throw std::runtime_error("Method name too long for metadata");
        //     classMetaSection.push_back(static_cast<uint8_t>(mName.size()));
        //     classMetaSection.insert(classMetaSection.end(), mName.begin(), mName.end());
        //     write_u32(classMetaSection, absOffset);
        // }
        // Copy unordered_map elements into a vector so we can control order

    for (const auto &methPair : cls.methods)
    {
        const std::string &mName = methPair.first;
        auto it = global_symbols_.find(mName);
        if (it == global_symbols_.end())
            throw std::runtime_error("Method not found for metadata: " + mName);

        uint32_t absOffset = it->second.address;

        std::cout << "[Linker] Class Method " << mName << " addr " << absOffset << "\n";

        if (mName.size() > 255)
            throw std::runtime_error("Method name too long for metadata");

        classMetaSection.push_back(static_cast<uint8_t>(mName.size()));
        classMetaSection.insert(classMetaSection.end(), mName.begin(), mName.end());
        write_u32(classMetaSection, absOffset);
    }
    }

    // ---------------- Header and file write ----------------
    std::ofstream out(outPath, std::ios::binary);
    if (!out)
        throw std::runtime_error("Cannot open final VM file");

    uint32_t codeSize = static_cast<uint32_t>(finalCode.size());
    uint32_t classMetaSize = static_cast<uint32_t>(classMetaSection.size());
    uint32_t globalsOffset = 44 + codeSize;
    uint32_t classMetaOffset = globalsOffset;

    uint8_t header[44] = {0};
    header[0] = 0x56;
    header[1] = 0x4D;
    header[2] = 0x00;
    header[3] = 0x01;

    uint32_t version = 1;
    std::memcpy(header + 4, &version, 4);
    std::memcpy(header + 8, &entryPoint, 4);

    uint32_t constPoolOffset = 44;
    uint32_t constPoolSize = 0;
    std::memcpy(header + 12, &constPoolOffset, 4);
    std::memcpy(header + 16, &constPoolSize, 4);

    uint32_t codeOffset = 44;
    std::memcpy(header + 20, &codeOffset, 4);
    std::memcpy(header + 24, &codeSize, 4);

    uint32_t globalsSize = 0;
    std::memcpy(header + 28, &globalsOffset, 4);
    std::memcpy(header + 32, &globalsSize, 4);

    std::memcpy(header + 36, &classMetaOffset, 4);
    std::memcpy(header + 40, &classMetaSize, 4);

    out.write(reinterpret_cast<char *>(header), sizeof(header));
    if (!finalCode.empty())
        out.write(reinterpret_cast<const char *>(finalCode.data()), finalCode.size());
    if (!classMetaSection.empty())
        out.write(reinterpret_cast<const char *>(classMetaSection.data()), classMetaSection.size());
    out.close();

    std::cout << "[Linker] Wrote final VM file: " << outPath
              << " | entryPoint=" << entryPoint
              << " | code=" << codeSize
              << " | classMeta=" << classMetaSize << "\n";
}


// --------------------------- link (orchestration) ---------------------------
void Linker::link(const std::string &outputVmPath)
{
    if (modules_.empty())
        throw std::runtime_error("No input modules");

    detectEntryModule();
    assignBaseAddressesAndOrder();
    buildGlobalSymbolTable();
    applyRelocations();

    std::vector<uint8_t> finalCode;
    mergeModules(finalCode);

    uint32_t entryPoint = 0;
    auto it = global_symbols_.find("main");
    if (it != global_symbols_.end())
    {
        entryPoint = it->second.address;
        std::cout << "[Linker] Found 'main' at absolute address " << entryPoint << "\n";
    }
    else
    {
        std::cerr << "[Linker] Warning: 'main' not found in global symbols; default entryPoint=0\n";
    }

    writeFinalVM(outputVmPath, finalCode, entryPoint);
}