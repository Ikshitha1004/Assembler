
# Assembler and Linker
## CSD LAB — CS401P

This repository contains  **Assembler and Linker** in C++ for a custom **stack-based Virtual Machine (VM)** instruction set used in CSD Lab.


## Project Overview

### **Assembler (with Integrated Linker)**

* Converts multiple `.asm` source files into a single **linked executable** (`.vm`).
* Internally performs:

  * **Assembly:** Tokenization, parsing, and opcode emission.
  * **Symbol management:** Builds local and global symbol tables.
  * **Relocation & Linking:** Resolves inter-file label and function references before generating the final binary.

---

## Key Features

* **Multi-file support:**
  Directly accepts multiple `.asm` files and links them into one `.vm`.

* **Assembler:**

  * Tokenizes **mnemonics**, **numbers**, **identifiers**, and **labels** from the input source.
  * Validates **instruction syntax** and **operand counts** for each mnemonic.
  * Builds an **Intermediate Representation (IR)** containing instruction metadata and symbol references.
  * Emits **relocatable object files** (`.vmobj`) with symbol and relocation tables for the linker.

* **Linker:**

  * Resolves **labels** and **method references** (`CALL`, `JMP`, `JZ`, `JNZ`) across multiple files.
  * Merges **code**, **symbol**, and **relocation** sections into one unified executable.
  * Performs **relocation patching**, automatically updating addresses after merging code sections.
  * Produces a final `.vm` file ready for execution on the Virtual Machine.

* **Error handling:**
  Detects invalid mnemonics, undefined labels, and duplicate symbols.
  Here’s your corrected and complete **Key Features** section — rewritten for clarity, completeness, and correctness, focusing only on **Assembler + Linker** integration (since Loader is not part of this stage):

---

## **Instruction Set (Subset)**

| **Category**                  | **Instructions**                                                                                                                         |
| ----------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------- |
| **Arithmetic (Int & Float)**  | `IADD`, `ISUB`, `IMUL`, `IDIV`, `INEG`, `FADD`, `FSUB`, `FMUL`, `FDIV`, `FNEG`                                                           |
| **Stack Operations**          | `PUSH`, `POP`, `DUP`, `FPUSH`, `FPOP`                                                                                                    |
| **Memory Access**             | `LOAD`, `STORE`, `LOAD_ARG`                                                                                                              |
| **Control Flow**              | `JMP`, `JZ`, `JNZ`, `CALL`, `RET`                                                                                                        |
| **Comparisons (Int & Float)** | `ICMP_EQ`, `ICMP_LT`, `ICMP_GT`, `ICMP_GEQ`, `ICMP_NEQ`, `ICMP_LEQ`, `FCMP_EQ`, `FCMP_LT`, `FCMP_GT`, `FCMP_GEQ`, `FCMP_NEQ`, `FCMP_LEQ` |
| **Object-Oriented**           | `NEW`, `GETFIELD`, `PUTFIELD`, `INVOKEVIRTUAL`, `INVOKESPECIAL`, `NEWARRAY`                                                              |
| **System Interface**          | `SYS_CALL <id>` — performs a VM-level system call (e.g., `OPEN`, `READ`, `WRITE`, `EXIT`, etc.)                                          |


---
## **Build and Run Instructions**

### **Build (CMake-based):**

```bash
# On Linux
./build.sh

# On Windows
.\build.bat
```

### **Usage**

```bash
# Assemble and link multiple files into a single VM executable
./bin/assembler output.vm input1.asm input2.asm input3.asm
# Disassemble .vmobj or .vm files for debugging
./bin/disassembler <file>.vmobj
./bin/disassembler <file>.vm
```

The assembler automatically:

1. Assembles all `.asm` files.
2. Merges symbol tables and relocations.
3. Outputs the final linked executable (`output.vm`).

---

## Team Members

| Name       | Roll Number |
| ---------- | ----------- |
| Ikshitha | (CS22B027)  |
| Sahiti | (CS22B056)  |
| Dakshayini | (CS21B016)  |

Detailed module-wise documentation and contributions can be found in the [MODULE_WISE_DOCS](MODULE_WISE_DOCS/) folder.

---
## Module 3 – Contributions


### Sahiti – Assembler File Formats

#### 1st September

* Drafted *input file format* for .asm supporting .text, .data, .bss, .const.
* Defined macros: .word, .byte, .string, .space, .align, .global.

#### 2nd September

* Designed *output object file (.obj) format* (header, section table, symbol table).
* Documented endianess flags, section flags, and symbol resolution.

#### 3rd September

* Updated spec for label resolution in data section.

#### 4th September

* Integrated *error reporting* for malformed macros (e.g., .word text).
* Tested assembler formats with demo1.asm.

#### 5th September

* Prepared documentation of *assembler I/O formats* and parser flow.


---

### Ikshita – Static & Shared Library Formats

#### 1st September

* Surveyed *static library formats* (.a, .lib) and dynamic ones (.so, .dll).
* Proposed using .lib for our VM toolchain.

#### 2nd September

* Designed *.lib format* (archive of .obj files with headers and offsets).
* Documented format fields: magic, count, per-object entries.

#### 3rd September

* Drafted *.sobj shared object format* (dynamic symbol table + relocations).

#### 4th September

* Wrote test libmath.lib with add.obj and mul.obj.

#### 5th September

* Read *static vs dynamic linking behavior* (when symbols resolved).


---

### Dakshayini – Linking Pipeline & Shared Library Example

#### 1st September
* Studied *linking pipeline*: assembler → linker → loader.

#### 2nd September
* Understood flow: Assembler → Linker → Loader → VM runtime.
* Studied how relocations will be applied during linking.

#### 3rd September
* Read about *static vs shared libraries* and their role in linking.
* Explored how symbol resolution differs between static and dynamic linking.

#### 4th September
* Built *example shared library* math.sobj exporting add2 function.

#### 5th September
* Wrote *Module 3.md* documentation.

---

 Module 3 submission : [MODULE_WISE_DOCS/Module3.md](MODULE_WISE_DOCS/Module3.md).


