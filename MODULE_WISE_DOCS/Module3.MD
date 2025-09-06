
---

`markdown
#  Assembler & Linker – File Formats and Libraries

This project implements a **stack-based VM assembler** with support for labels, constants, symbol table, IR generation, and a basic linking model.  

---

## 1. Assembler Input Format

The assembler accepts `.asm` source files with support for **macros**:

asm
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
`

### Supported Directives

* `.class <name>` → declare class
* `.super <name>` → superclass
* `.const NAME VALUE` → constant definition
* `.limit stack <n>` / `.limit locals <n>` → per-method resource limits
* `.entry <method>` → program entry point
* `.method … .end` → method definition
* `.data <type>` (future) → declare global data
* `.endian little|big` → file endianness

---

##  2. Assembler Output Format (`.o`)

The assembler produces a **relocatable object file** (`.o`), similar to C/C++ object files.

### File Layout


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


### Example Header (C-struct)

c
struct ObjHeader {
    char magic[4];     // "VMO\0"
    uint16_t version;  // =1
    uint16_t endian;   // 0=little, 1=big
};


* **.text** → machine code / bytecode
* **.data** → constants & initialized memory
* **.symtab** → labels, methods, constants

---

##  3. Static Library Format (`.vmlib`)

Static libraries are packaged archives of `.o` files, similar to **C++ `.a` archives** or **Java `.jar`** files.

### Layout


+-----------------+
| LIB HEADER      |
+-----------------+
| INDEX ENTRIES   |
+-----------------+
| OBJECT DATA     |
+-----------------+


### Example Header

c
struct VMLibHeader {
    char magic[6];     // "VMLIB\0"
    uint32_t version;  // =1
    uint32_t count;    // number of objects
};


### Example Index Entry

c
struct VMLibEntry {
    char name[64];     // "add.o"
    uint32_t offset;   // offset of object file
    uint32_t size;     // size of object file
};


---

##  4.Reading done:



---

##  5. Example Library (`libmath.vmlib`)

We define a math library with **two methods: `add` and `mul`**.

### `add.asm`

asm
.method public static add
    .limit stack 2
    .limit locals 2
    LOAD 0
    LOAD 1
    IADD
    RET
.end


### `mul.asm`

asm
.method public static mul
    .limit stack 2
    .limit locals 2
    LOAD 0
    LOAD 1
    IMUL
    RET
.end


Both are compiled into `add.o` and `mul.o`.

---

### `libmath.vmlib` Layout


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


---
