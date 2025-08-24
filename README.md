
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


