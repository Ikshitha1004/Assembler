

---

#  Assembler & Linker – File Formats and Libraries

This project implements a **stack-based VM assembler** with support for labels, constants, symbol table, IR generation, and a basic linking model.  

---

##  1. Assembler Input Format

The assembler accepts `.asm` source files with support for **macros**:

```asm
; Example program: main.asm

.class Factorial
.method main
    .limit stack 6
    .limit locals 3   ; locals[0]=n, locals[1]=result, locals[2]=counter

    ; result = 1
    LOAD 1
    STORE 1

    ; counter = n
    LOAD 0
    STORE 2

Lloop:
    LOAD 2            ; counter
    JZ Lend           ; if counter == 0 → end

    ; result = result * counter
    LOAD 1
    LOAD 2
    IMUL
    STORE 1

    ; counter = counter - 1
    LOAD 2
    LOAD 3            ; push 1
    ISUB
    STORE 2

    JMP Lloop

Lend:
    RET
.end
````

---

## 🔹 2. Assembler Output Format (`.o`)

The assembler produces a **relocatable object file** (`.o`), similar to C/C++ object files.

### File Layout
```
+----------------+
| HEADER         |
+----------------+
| SECTION TABLE  |
+----------------+
| .code          |  (bytecode instructions)
+----------------+
| .constpool     |  (constants, strings, method refs)
+----------------+
| .locals        |  (metadata: #locals, #stack slots)
+----------------+
| SYMBOL TABLE   |  (methods, fields)
+----------------+
| RELOCATION     |  (unresolved constpool refs)
+----------------+

```

---

## 🔹 3. Static Library Format (`.vmlib`)

Static libraries are packaged archives of `.o` files, similar to **C++ `.a` archives** or **Java `.jar`** files.

### Layout

```text
+-----------------+
| LIB HEADER      |
+-----------------+
| INDEX ENTRIES   |
+-----------------+
| OBJECT DATA     |
+-----------------+
```

### Example Header

```c
struct VMLibHeader {
    char magic[6];     // "VMLIB\0"
    uint32_t version;  // =1
    uint32_t count;    // number of objects
};
```

### Example Index Entry

```c
struct VMLibEntry {
    char name[64];     // "add.o"
    uint32_t offset;   // offset of object file
    uint32_t size;     // size of object file
};
```

---

## 🔹 4. Example Library (`libmath.vmlib`)
```
Header:
  magic   = "VMOBJ"
  version = 1

Section Table:
  .code       offset=128 size=64
  .constpool  offset=192 size=32
  .locals     offset=224 size=12
  .symtab     offset=236 size=48

.code:
  Bytecode for factorial (LOAD, STORE, JZ, IMUL, ...)

.constpool:
  [0] int 1
  [1] field "n"
  [2] methodref "IMUL"
  [3] labelref "Lend"

.locals:
  count=3 → [0]=n, [1]=result, [2]=counter

.symbol table:
  main() → entry point, references .code
```