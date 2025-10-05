.text
.method add
.limit stack 2
.limit locals 3
L0:
PUSH 1
STORE 0 ; Store to a
JMP L2
LOAD 0  ; Load a
LOAD 1  ; Load b
IADD
STORE 0 ; Store to a
PUSH 2
STORE 2 ; Init c
LOAD 0  ; Load a
LOAD 1  ; Load b
IADD
LOAD 2  ; Load c
IADD
RET
.endmethod



