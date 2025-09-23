
.method main
.limit stack 3
.limit locals 5
PUSH 3
STORE 3 ; Init d
PUSH 2
STORE 2 ; Init c
PUSH 10
STORE 1 ; Init b
PUSH 5
STORE 0 ; Init a
LOAD 0  ; Load a
INEG
LOAD 1  ; Load b
LOAD 2  ; Post-Increment: Load original value of c
DUP
PUSH 1
IADD
STORE 2 ; Post-Increment: Store new value to c
IMUL
IADD
STORE 4 ; Init result1
LOAD 0  ; Load a
LOAD 1  ; Load b
PUSH 2
IMUL
IADD
PUSH 25
ICMP_EQ
JNZ L4
JMP L3
L4:
LOAD 2  ; Load c
LOAD 3  ; Load d
ICMP_EQ
JNZ L0
JMP L3
L3:
L0:
PUSH 1
STORE 0 ; Store to a
JMP L2
L1:
PUSH 0
STORE 0 ; Store to a
L2:
LOAD 0  ; Load a
LOAD 1  ; Load b
IADD
POP
RET
.endmethod
