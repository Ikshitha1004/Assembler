# Module 1 — Assembler Frontend (Lexer + Parser)

## Overview

Module 1 implements the **frontend of the assembler**, responsible for converting human-readable assembly source into an internal representation suitable for later linking and loading.

It consists of:

* **Tokenizer (Lexer):** Breaks the raw text into tokens such as mnemonics, numbers, identifiers, and labels.
* **Parser:** Validates tokens against the VM ISA, constructs `Instruction` objects, and records labels for future resolution.
* **Error Handler:** Detects invalid syntax, operands, or mnemonics and reports them gracefully without crashing.

---

## ISA Implemented

This assembler currently supports the **full stack-based VM ISA**:

### Arithmetic

* `IADD`, `ISUB`, `IMUL`, `IDIV`, `INEG`

### Stack

* `PUSH <val>`, `POP`, `DUP`

### Memory

* `LOAD <idx>`, `STORE <idx>`

### Control Flow

* `JMP <label>`, `JZ <label>`, `JNZ <label>`, `CALL <label>`, `RET`

### Comparison

* `ICMP_EQ`, `ICMP_LT`, `ICMP_GT`

### Object-Oriented

* `NEW <class_ref>`, `GETFIELD <field_ref>`, `PUTFIELD <field_ref>`, `INVOKEVIRTUAL <meth_ref>`, `INVOKESPECIAL <meth_ref>`

---

## Operand Rules

Each instruction has strict operand rules that the parser enforces:

* **0 operands**
  `IADD, ISUB, IMUL, IDIV, INEG, POP, DUP, RET, ICMP_EQ, ICMP_LT, ICMP_GT`

* **1 operand (immediate or index)**
  `PUSH <val>, LOAD <idx>, STORE <idx>, JMP <label>, JZ <label>, JNZ <label>, CALL <label>, NEW <class_ref>, GETFIELD <field_ref>, PUTFIELD <field_ref>, INVOKEVIRTUAL <meth_ref>, INVOKESPECIAL <meth_ref>`

If an instruction receives the wrong number of operands, the parser reports an **Unexpected Operand Error**.

---

## Error Handling

Error detection happens in two stages:

### 1. Tokenizer Errors

* **Invalid Numbers**: Non-numeric literals where numbers are expected (e.g., `PUSH xyz`).
* **Unknown Tokens**: Characters or tokens that don’t fit any category.

### 2. Parser Errors

* **Invalid Mnemonics**: Tokens not matching any known instruction (`ADD` instead of `IADD`).
* **Unexpected Operands**: Too many or too few operands (`POP 1`, `IADD 2`).
* **Duplicate Labels**: Redefinition of the same label (`loop:` defined twice).
* **Undefined Mnemonics**: Any mnemonic not in the ISA maps to `OpCode::INVALID`.
* **Graceful Recovery**: Errors are reported with **line/column number**, but parsing continues so that multiple errors can be shown in one run.

---

## Example Error Cases

 Invalid Mnemonics

```asm
ADD 2, 3   ; not valid, should be IADD
```

 Unexpected Operands

```asm
POP 1      ; POP takes no operands
```

 Invalid Numbers

```asm
PUSH xyz   ; operand must be a number
```

 Duplicate Labels

```asm
loop:
  IADD
loop:       ; duplicate definition
```

 Undefined Mnemonics

```asm
FOO 10     ; not in ISA
```

---

## Test Files

###  Valid Programs

| File                  | Description                               |
| --------------------- | ----------------------------------------- |
| `tests/demo1.asm` | Covers arithmetic, memory ops, comparisons,branching, and function calls. |
| `tests/demo2.asm` | Exercises object-oriented ops  , control flow        |
| `tests/demo3.asm` | Comprehensive test of the core ISA        |

###  Error Programs

| File                      | Error Type                                                    |
| ------------------------- | ------------------------------------------------------------- |
| `tests/error_set1.asm`  | Invalid mnemonic, wrong operands, bad number                 |
| `tests/error_set2.asm` |  Duplicate labels, undefined labels, and more mnemonics                     |
| `tests/error_set3.asm`  | Collects all errors|

---

## Output Behavior

Running the assembler on any file prints:

1. **Token Stream** (for debugging).
2. **Parsed Instruction List** (for valid programs).
3. **Error Messages** (for invalid programs).

Example:

```bash
./bin/assembler tests/errors_mixed.asm
Error [line 2, col 1]: Invalid mnemonic 'ADD'
Error [line 4, col 5]: Unexpected operand for POP
Error [line 6, col 8]: Invalid number 'xyz'
```

---

## Summary

* Module 1 implements **Lexer + Parser** for the stack-based ISA.
* Enforces strict operand rules per instruction.
* Provides **robust error handling** with clear diagnostics.
* Enables **incremental development** for Module 2 (Linker) and Module 3 (Loader).

##Contributions

- Ikshitha : Parser
- sahiti : Tokenizer
- Dakshayini : Error validation 