

# **Module 7 — Linker Integration and ISA Extension**


## **1. Overview**

In **Module 7**, we extended our assembler–linker–loader toolchain to support **multi-file linking**, **symbol resolution**, and **system call integration** with the VM.

This module focused on:

* Implementing a full **Linker**.
* Modifying the **Assembler** to emit relocatable object files.
* Extending the **ISA** to include new **Syscall-based instructions** for VM-level integration.

---

## **2. Implementation Details**

### **2.1 Linker Implementation**

* Implemented a **Linker** to merge multiple `.vmobj` files into one executable (`.vmx`).
* Resolved:

  * **Label references** for jump instructions (`JMP`, `JZ`, `JNZ`).
  * **External method calls** in `CALL` instructions.

* Ensured correct merging of:

  * Code segments
  * Symbol tables
  * Relocation entries

---

### **2.2 Assembler Modifications for Linking**

* Modified the **Parser**:

  * When encountering `CALL` or jump targets not found in the local symbol table, the assembler now:

    * Marks them as **external references**.
    * Adds them to the **symbol table** for the linker to resolve later.
* Enhanced **Emitter**:

  * Emits **placeholder values** for unresolved symbols.
  * Writes **16-bit operands** for jumps and **32-bit operands** for calls.
* Added relocation entry emission for all external symbol references.
* Updated **instruction validation** to handle new opcodes consistently.

---

### **2.3 ISA Extension — System Calls and New Opcodes**

* Added new **opcodes** and a dedicated **`SYS_CALL`** instruction to interface with VM-level system calls.
* Introduced:

  ```cpp
  enum class Syscall : uint8_t {
      OPEN, READ, WRITE, CLOSE, EXIT,
      GETPID, TIME, CHDIR, MKDIR,
      UNLINK, RENAME, ISATTY
  };
  ```
* Updated:

  * **Mnemonic table** — to recognize `SYS_CALL` and syscall names.
  * **Instruction size table** — to handle 2-byte syscall format (`opcode + syscall_id`).
  * **Disassembler** — to print syscall mnemonics with identifiers for readability.

---

### **2.4 Testing and Validation**

* Verified linking correctness across multiple `.vmobj` files containing inter-method `CALL`s and label jumps.
* Confirmed assembler-generated relocation tables are correctly interpreted by the linker.
* Validated extended ISA and syscall support through sample assembly programs.
* Disassembler verified by decoding `.vmobj` and `.vm` outputs.

---

## **3. Build and Run Instructions**

### **Build (CMake-based):**

```bash
# On Linux
./build.sh

# On Windows
.\build.bat
```

### **Usage:**

```bash
# Assemble individual source files
./bin/assembler <file>.asm

# Disassemble .vmobj or .vm files for debugging
./bin/disassembler <file>.vmobj
./bin/disassembler <file>.vm


```

---

## **4. Contributions**

| **Contributor** | **Date**   | **Work / Reading / Coding**                                                                                                                  |
| --------------- | ---------- | -------------------------------------------------------------------------------------------------------------------------------------------- |
| **Ikshitha**    | 2025-10-15 | Implemented complete **Linker** including global symbol table and relocation patching.                                                        |
|                 | 2025-10-16 | Verified correctness of linked outputs and relocation accuracy across modules.                                                               |
| **Sahiti**      | 2025-10-15 | Developed **Disassembler** for `.vmobj` and `.vm` formats.                                                                                  |
|                 | 2025-10-16 | Integrated existing assembler with linker, fixing symbol table and .vmobj issues.                                                             |                      |
| **Dakshayini**  | 2025-10-16 | Extended ISA with new **opcodes** and **Syscalls** for VM integration.                                                                       |
|                 | 2025-10-17 | Updated assembler’s opcode tables, instruction sizing, and disassembler logic for system calls.                                              |

---

## **5. Key Takeaways**

* Successfully implemented **multi-file linking** with relocation and symbol resolution.
* Integrated **system calls** into the instruction set for richer VM functionality.
* Established a **complete end-to-end toolchain**:
  **Assembler → Linker → VM Execution.**
