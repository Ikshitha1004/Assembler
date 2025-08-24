; Test 1: Core ISA functionality (arithmetic, stack, memory, branching)

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

; --- Stack ops ---
DUP             ; duplicate top
POP             ; remove one

; --- Memory ---
STORE 0         ; store -5 into local[0]
LOAD 0          ; load back

; --- Comparison ---
PUSH 10
ICMP_EQ         ; compare local[0] with 10
PUSH 20
ICMP_LT
PUSH 1
ICMP_GT

; --- Control flow ---
JMP check
PUSH 111        ; skipped
check:
JZ zero_case
JNZ nonzero_case

zero_case:
PUSH 999
JMP end

nonzero_case:
PUSH 123
CALL func

func:
PUSH 7
RET

end:
PUSH 42
