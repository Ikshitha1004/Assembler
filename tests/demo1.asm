.class Factorial
.method main ()V
    .limit stack 6
    .limit locals 3   ; 0=n, 1=result, 2=counter

    ; result = 1
    PUSH 1
    STORE 1

    ; counter = n
    LOAD 0
    STORE 2

Lloop:
    ; if counter == 0 → jump to Lend
    LOAD 2
    JZ Lend

    ; result = result * counter
    LOAD 1
    LOAD 2
    IMUL
    STORE 1

    ; counter = counter - 1
    LOAD 2
    PUSH 1
    ISUB
    STORE 2

    JMP Lloop

Lend:
    RET
.end
