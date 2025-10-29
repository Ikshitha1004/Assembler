; Test 1: Core ISA functionality (arithmetic,float, stack, memory)

.class_metadata
class_count 0
.end_metadata
; --- Arithmetic ---
PUSH 2
PUSH 3
IADD            ; stack: 5
PUSH 4
IMUL            ; stack: 20
PUSH 5
ISUB            ; stack: 15
INEG            ; stack: -15
PUSH 3
IDIV            ; stack: -5

; Floating arithmetic
FPUSH 3.14
FPUSH 2.0
FMUL          ; 6.28
FPUSH 1.14
FADD          ; 7.42
FPUSH 2.0
FDIV          ; 3.71
FNEG          ; -3.71
FPOP          ; clear stack

; --- Stack ops ---
DUP             ; duplicate top
POP             ; remove one

; --- Memory ---
STORE 0         ; store -5 into local[0]
LOAD 0          ; load back


