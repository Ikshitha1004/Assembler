# ARM_ALL

# ARM Assembler Core Module

This module implements the **core tokenizer, parser, and section handling** for an ARM assembler. It is designed to be the **central file** of the assembler project and will be extended with Pass 1/Pass 2 functionality, symbol tables, and instruction encoding.

## Features

### Tokenizer
- Recognizes labels, mnemonics, directives, registers, immediates, and comments.
- Supports standard ARM syntax, including:
  - Registers: `r0`–`r15`, `sp`, `lr`, `pc`
  - Directives: `.text`, `.data`, `.bss`, `.word`, `.asciz`, `.skip`, `.align`
  - Comments: `@`, `;`, `//`

### Parser Skeleton
- Converts tokens into an **AST** (Abstract Syntax Tree).
- Handles:
  - Labels
  - Directives
  - Instructions
- Supports operand parsing for instructions and directives.

### Section Handling & Location Counters
- Maintains current section (`.text`, `.data`, `.bss`).
- Tracks location counter (`LC`) per section.
- Updates LC for instructions and directives like `.word` and `.asciz`.

### Instructions Currently Recognized
- Data processing: `MOV`, `ADD`, `SUB`, `RSB`, `AND`, `ORR`, `EOR`, `MVN`
- Compare: `CMP`, `CMN`, `TST`, `TEQ`
- Memory: `LDR`, `STR`, `LDM`, `STM`
- Branching: `B`, `BL`, `BX`
- Misc: `SWI`, `NOP`
- Shift operations: `LSL`, `LSR`, `ASR`, `ROR`

> **Note:** This module does not encode instructions into binary yet. It is meant to provide a **foundation** for Pass 1 (symbol collection) and Pass 2 (instruction encoding).

## How to Use
```bash
g++ -std=c++17 assembler.cpp -o assembler
./assembler example.asm
