;No function reference-unrelated files-gives 
.class_metadata
class_count 0
.end_metadata

.code
.method main
.limit stack 3
.limit locals 2

PUSH 10
STORE 0        ; locals[0] = 10
LOAD 0
PUSH 5
IADD
STORE 1        ; locals[1] = locals[0] + 5 = 15

LOAD 1
RET
.endmethod
