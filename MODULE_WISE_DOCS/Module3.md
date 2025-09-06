

---

#  Assembler & Linker – File Formats and Libraries

This project implements a **stack-based VM assembler** with support for labels, constants, symbol table, IR generation, and a basic linking model.  

---

##  1. Assembler Input Format

The assembler accepts `.asm` source files with support for **macros**:

```asm
; Example program: main.asm

.class Main
.super java/lang/Object

.const TEN 10          ; constant macro
.entry main            ; mark entry point

.method public static main
    .limit stack 4
    .limit locals 2

    PUSH TEN           ; load constant
    PUSH 20
    IADD
    STORE 0
    RET
.end
````

---

## 🔹 2. Assembler Output Format (`.o`)

The assembler produces a **relocatable object file** (`.o`), similar to C/C++ object files.

### File Layout

```text
+----------------+
| HEADER         |
+----------------+
| SECTION TABLE  |
+----------------+
| .text          |
+----------------+
| .data          |
+----------------+
| SYMBOL TABLE   |
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

We define a math library with **two methods: `add` and `mul`**.

### File Layout of `libmath.vmlib`

```text
Header:
  magic   = "VMLIB"
  version = 1
  count   = 2

Index:
  [0] name="add.o" offset=128 size=96
  [1] name="mul.o" offset=224 size=104

Object Data:
  (binary contents of add.o)
  (binary contents of mul.o)
```

---



