# Stack-Based Assembler

This project implements the **core tokenizer and parser** for a **stack-based virtual machine (VM) assembler**.  
The assembler processes **23 stack-based VM instructions**, converting human-readable assembly into a structured intermediate representation.  

It lays the foundation for future passes such as label resolution and bytecode generation.

---

##  Features

### 1. Tokenizer (Lexer)
- Recognizes:
  - **Mnemonics**:  
    `PUSH, POP, DUP, IADD, ISUB, IMUL, IDIV, INEG, LOAD, STORE, JMP, JZ, JNZ, CALL, RET, ICMP_EQ, ICMP_LT, ICMP_GT, NEW, GETFIELD, PUTFIELD, INVOKEVIRTUAL, INVOKESPECIAL`
  - **Numbers, identifiers, labels, and comments** (`;` or `//`)
- Produces a **stream of tokens** with type and value.
- Modular and reusable for further passes.

### 2. Parser
- Converts the token stream into **instructions (opcode + operands)**.
- Validates mnemonics against the **23-instruction ISA**.
- Supports **operand parsing** for all instructions.
- Maintains **labels** for jumps and function calls.
- Error handling (currently commented out for dev testing).

### 3. Supported Instructions
- **Arithmetic**: `IADD, ISUB, IMUL, IDIV, INEG`  
- **Stack**: `PUSH, POP, DUP`  
- **Memory**: `LOAD, STORE`  
- **Control Flow**: `JMP, JZ, JNZ, CALL, RET`  
- **Comparison**: `ICMP_EQ, ICMP_LT, ICMP_GT`  
- **Object-Oriented**: `NEW, GETFIELD, PUTFIELD, INVOKEVIRTUAL, INVOKESPECIAL`

### 4. Demo Program
```asm
; Computes (2 + 3) * 4, stores to local[0], compares, and simple branch

        PUSH 2
        PUSH 3
        IADD
        PUSH 4
        IMUL
        STORE 0

        LOAD 0
        PUSH 20
        ICMP_LT         ; local[0] < 20 ? 1 : 0
        JZ  end

        ; object-y ops with dummy constant pool refs
        NEW  1
        DUP
        PUTFIELD 2
        POP

        JMP done

end:
        LOAD 0
        PUSH 20
        ICMP_GT
        JNZ alt

        ; call fallthrough
        CALL func
        JMP done

alt:
        CALL func

func:
        ; trivial function body + return
        RET

done:
        ; loop forever (just to demonstrate label resolution)
        JMP done
