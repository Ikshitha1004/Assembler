
.method main
.limit stack 2
.limit locals 2
PUSH 0
STORE 0 ; Init a
PUSH 10
STORE 1 ; Init b
LOAD 0  ; Load a
LOAD 1  ; Load b
ICMP_LT
JNZ L0
JMP L1
L0:
LOAD 0  ; Load a
PUSH 1
IADD
STORE 0 ; Store to a
JMP L2
L1:
LOAD 1  ; Load b
PUSH 1
IADD
STORE 1 ; Store to b
PUSH 11
STORE 0 ; Init c
L2:
L3:
LOAD 0  ; Load a
PUSH 10
ICMP_LT
JNZ L4
JMP L5
L4:
LOAD 0  ; Load a
PUSH 1
IADD
STORE 0 ; Store to a
JMP L3
L5:
PUSH 0
STORE 0 ; Init i
L6:
LOAD 0  ; Load i
PUSH 10
ICMP_LT
JNZ L7
JMP L8
L7:
LOAD 0  ; Post-Increment: Load original value of a
DUP
PUSH 1
IADD
STORE 0 ; Post-Increment: Store new value to a
POP
LOAD 1  ; Post-Increment: Load original value of b
DUP
PUSH 1
IADD
STORE 1 ; Post-Increment: Store new value to b
POP
LOAD 0  ; Post-Increment: Load original value of i
DUP
PUSH 1
IADD
STORE 0 ; Post-Increment: Store new value to i
POP
JMP L6
L8:
RET
.endmethod
