
# Module 4 Documentation: Enhanced Directive Handling and Method/Class Resolution

## 1. Overview

This module extends the current intermediate representation (IR) and bytecode emitter to support:

* Additional directives such as `.text`, `.endmethod`, `.class`, `.method`, and `.endclass`.
* Improved handling and resolution of methods and classes.
* Constant pool referencing and management.

The goal is to make the stack-based virtual machine (VM) more expressive and closer to  our VM style bytecode semantics.

---

## 2. New Directives

### 2.1 `.text`

* Marks the beginning of the executable code section.
* Separates code from other data sections like constants or class metadata.

### 2.2 `.method` / `.endmethod`

* Defines the start and end of a method.
* Tracks method signature, stack limits, and local variables.

### 2.3 `.class` / `.endclass`

* Defines the start and end of a class.
* Tracks class name, fields, and method references.

---

## 3. Method and Class Resolution

### 3.1 Method Resolution


1. **Parsing Phase**: Collect method signatures and starting addresses into a symbol table.


### 3.2 Class Resolution

* Classes are assigned unique identifiers in the constant pool.
* During parsing:

  * If a method call references a class, the class must already exist in the constant pool.
  * Tracks class hierarchy and enables method lookup in parent classes.

---

## 4. Constant Pool Referencing

* All strings, class names, and method signatures are stored in a constant pool.
* Each entry has a unique index for efficient reference.
* During instruction parsing, references to constants (e.g., method calls, class names) are replaced with constant pool indices.
* Ensures deduplication: repeated constants are stored only once.

---

## 5. Bytecode Emitter

* Translates resolved instructions into bytecode.
* Steps:

  1. Write `.class` and `.method` metadata.
  2. Emit code segment starting with `.text`.
  3. Replace symbolic references with constant pool indices.

---

## 6. Example Workflow

**Output from the compiler:**

```asm
.class TestClass

.method publicMethod
.limit stack 2
.limit locals 0
PUSH 10
STORE 0 ; Store to publicVar
RET
.endmethod
.endclass

.method compute_sum
.limit stack 2
.limit locals 3
LOAD 0  ; Load a
LOAD 1  ; Load b
IADD
STORE 2 ; Init sum
LOAD 2  ; Load sum
RET
.endmethod

.method main
.limit stack 2
.limit locals 3
PUSH 15
STORE 1 ; Init var2
LOAD 2  ; Load gh
CALL compute_sum
STORE 0 ; Store to var1
RET
.endmethod
```

**Assembler Output:**
*To be put*

---

## 7. Future Enhancements

* Support for `.data` sections.
* Increase capacity for constant pool, method references, object references, and class references.
* Handle access modifiers for methods and classes.
* Improve linking for forward-referenced methods and classes.

---

## 8. Contributions (Last 5 Days)

| Contributor   | Date       | Work / Reading / Coding                                                                                                 |
| ------------- | ---------- | ----------------------------------------------------------------------------------------------------------------------- |
| **Ikshitha**  | 2025-09-15 | Added parsing for new directives `.text`, `.endmethod`, `.class`, `.method`. |
|               | 2025-09-16 | Tested method resolution and linking for simple method calls.                                                           |
|               | 2025-09-17 |  Bug fixes for `CALL` and `INVOKE` opcodes.                                                                             |
|               | 2025-09-18 | Learnt about the entries in constnat pool and Verified symbolic references constant pool indices during emission.       |                           |
|               | 2025-09-19 | End-to-end testing of classes and methods with the emitter.                                                             |
| **Sahiti**    | 2025-09-15 | Implemented constant pool resolution for normal instructions and OOP support.                                           |
|               | 2025-09-16 | Built symbol table for classes and method lookup.                                                                       |
|               | 2025-09-17 | Integrated method and class linking during parsing.                                                                     |
|               | 2025-09-18 | Tested constant pool references for string literals, class names, and methods.                                          |
|               | 2025-09-19 | Debugged constant pool references in nested method calls.                                                               |
| **Dakshayini** | 2025-09-15 | Modified emitter to write class and method metadata correctly.                                                          |
|               | 2025-09-16 | Updated emitter to resolve symbolic references with constant pool indices.                                              |
|               | 2025-09-17 | Verified instruction encoding and stack layout.                                                                         |
|               | 2025-09-18 | Tested bytecode emission for multiple methods.                                                                          |
|               | 2025-09-19 | Finalized integration of emitter with constant pool and directives.                                                     |

---

