.text
.method add
.limit stack 2
.limit locals 3
L23:
LOAD 2
IADD
POP
SYS_CALL READ
L0:
PUSH 1
STORE 0 ; Store to a
LOAD 0  ; Load a
LOAD 1  ; Load b
IADD
STORE 0 ; Store to a
PUSH 2
STORE 2 ; Init c
LOAD 0  ; Load a
LOAD 1  ; Load b
JMP L3
IADD
LOAD 2  ; Load c
L3:
IADD
RET
.endmethod



