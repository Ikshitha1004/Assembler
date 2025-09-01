.class Factorial
.method main
    .limit stack 6
    .limit locals 3   ; locals[0]=n, locals[1]=result, locals[2]=counter

    ; result = 1
    LOAD 1
    STORE 1

    ; counter = n
    LOAD 0
    STORE 2

Lloop:
    LOAD 2            ; counter
    JZ Lend           ; if counter == 0 → end

    ; result = result * counter
    LOAD 1
    LOAD 2
    IMUL
    STORE 1

    ; counter = counter - 1
    LOAD 2
    LOAD 3            ; push 1
    ISUB
    STORE 2

    JMP Lloop

Lend:
    RET
.end