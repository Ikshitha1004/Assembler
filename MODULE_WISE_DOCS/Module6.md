
# Module 6 : Assembler Enhancements and Linker Implementation

## 1. Overview

Module 6 extends the assembler and introduces basic **linker functionality**. Key goals were:

* Updating the tokenizer and parser to handle all VM instructions, including floating-point operations.
* Implementation of a symbol table, symbol resolution, and relocation table.
* Partial code generation and linking multiple input files and libraries.

The module bridges the gap between single-file assembly and full multi-file linking for the VM environment.

---

## 2. Implementation Details

### 2.1 Sample Input and Output Formats

Got it! Here’s the revised **Section 2.1** for Module 6, updated to reflect that you specifically decided on the `.vmobj` format for linker input:

---

### 2.1 Sample Input and Output Formats

* Decided on the **`.vmobj` file format** to serve as the **linker input**, containing:

  * Encoded instructions and code metadata.
  * Symbol table entries and relocation information.
  * Constant pool references and method/class metadata.
* Generated `.vmobj` output files from assembler source, which are fully compatible with the linker for multi-file resolution and linking.

### 2.2 Parser and Tokenizer Updates

* Updated tokenizer to recognize floating-point instructions.

* Parser was extended to:
  * Build symbol tables for methods, variables, and library symbols.
  * Resolve references within the same file and across linked libraries.

### 2.4 Relocation and Symbol Resolution

* Added relocation tables to track addresses of symbols that need adjustment during linking.
* Symbol resolution logic implemented to:
  * Map references to correct memory locations across files.
  * Handle linking of multiple input files with overlapping symbols.

---

## 3. Contributions (Last 5 Days)

| Contributor    | Date       | Work / Reading / Coding                                                                                 |
| -------------- | ---------- | ------------------------------------------------------------------------------------------------------- |
| **Dakshayini** | 2025-10-06 | Updated tokenizer and parser to handle floating-point instructions and other VM-supported instructions. |
| **Sahiti**     | 2025-10-06 | Added relocation table support and partially completed code generation for assembler output.            |
| **Ikshitha**   | 2025-10-06 | Implemented linking logic for multiple input files; wrote resolution logic for symbols and libraries.   |

---

## 4. Key Takeaways

* Assembler and linker now support **multi-file projects** with partial code generation.
* Parser and tokenizer are fully updated for VM instruction set, including floating-point operations.
* Symbol tables, relocation, and linking logic allow correct resolution across multiple inputs and libraries.

