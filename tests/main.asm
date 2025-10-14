; main.asm - simple program calling a function

.method main
.limit stack 2
.limit locals 1

PUSH 10       ; Push constant 10
STORE 1
JMP L0
STORE 0       ; Store result in local 0
RET
.endmethod
