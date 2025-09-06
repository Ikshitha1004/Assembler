
# Assembler, Linker, and Loader 
## CSD LAB — CS401P
This repository contains **Assembler, Linker, and Loader** in C++ that work together to process a custom VM instruction set architecture (ISA) for CSD Lab.

---

## Project Overview


* **Assembler**
  Converts human-readable assembly source code into a stream of machine-readable instructions.
* **Linker**
  Resolves labels, symbols, and combines multiple object files into a single executable image.
* **Loader**
  Prepares the executable for execution by resolving addresses, initializing memory, and passing control to the virtual machine.


---

## Key Features

* **23-Instruction Stack-Based ISA**
  Supports arithmetic, stack operations, memory access, control flow, comparisons, and basic object-oriented instructions.
* **Assembler**

  * Tokenizes mnemonics, numbers, identifiers, and labels.
  * Validates instruction syntax.
  * Builds an intermediate representation (`Instruction` objects).
* **Linker**

  * Resolves labels for jumps and calls.
  * Handles multiple assembly source files.
* **Loader**

  * Loads the linked program into memory.
  * Initializes stack, constant pool, and instruction pointer.
* **Error Handling**
  Detects invalid mnemonics, missing labels, and malformed operands.
* **Demo Program**
  Included sample assembly file (`demo.asm`) demonstrates stack arithmetic, branching, and object operations.

---



## Planned Instruction Set (Subset)

* **Arithmetic**: IADD, ISUB, IMUL, IDIV, INEG
* **Stack**: PUSH, POP, DUP
* **Memory**: LOAD, STORE
* **Control Flow**: JMP, JZ, JNZ, CALL, RET
* **Comparison**: ICMP\_EQ, ICMP\_LT, ICMP\_GT
* **Object-Oriented**: NEW, GETFIELD, PUTFIELD, INVOKEVIRTUAL, INVOKESPECIAL

---

## Usage

1. **Build**:

   ```bash
   make clean
   make
   ```

   Produces the assembler binary in `bin/assembler`.

2. **Run**:

   ```bash
   ./bin/assembler demo.asm
   ```

3. **Output**:
   Prints tokens and parsed instruction list.

---

## Team Members

| Name       | Roll Number |
| ---------- | ----------- |
| Ikshitha | (CS22B027)  |
| Sahiti | (CS22B056)  |
| Dakshayini | (CS21B016)  |

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


