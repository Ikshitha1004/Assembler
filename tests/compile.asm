.code
.method add@int@int
.limit stack 4
.limit locals 8

RET
.endmethod

.method main
.limit stack 4
.limit locals 8
    PUSH 10
    PUSH 20
    CALL add@int@int
    RET
.endmethod