.text

; -------------------
; Class definition
; -------------------
.class MyClass
.method setValues
    .limit stack 2
    .limit locals 2   ; 0=x, 1=y
    LOAD 0
    STORE 0
    LOAD 1
    STORE 1
    RET
.endmethod

.method sum
    .limit stack 2
    .limit locals 3   ; 0=x, 1=y, 2=result
    LOAD 0
    LOAD 1
    IADD
    STORE 2
    LOAD 2
    RET
.endmethod
.endclass

; -------------------
; Main program
; -------------------
.method main
    .limit stack 6
    .limit locals 5   ; 0=x, 1=y, 2=total, 3=i, 4=temp

    ; Initialize object values (x=5, y=10)
    PUSH 5
    STORE 0
    PUSH 10
    STORE 1
    CALL MyClass.setValues
    
    ; Loop over array NUMS and add each value to x
    PUSH 0
    STORE 3          ; i = 0
set:
LOOP_START:
    LOAD 3
    PUSH 5           ; SIZE = 5
    ISUB
    JZ LOOP_END      ; exit loop if i == SIZE

    ; temp = NUMS[i]
    LOAD 3
    ; Here we simulate getting NUMS[i] (NUMS + i)
    ; For simplicity, just push a value from NUMS
    ; e.g., NUMS[0]=2, NUMS[1]=4, etc.
    ; We'll simulate as:
    LOAD 3
    PUSH 2
    IMUL
    STORE 4          ; temp = NUMS[i] (simulated)

    ; x = x + temp
    LOAD 0
    LOAD 4
    IADD
    STORE 0

    ; i = i + 1
    LOAD 3
    PUSH 1
    IADD
    STORE 3

    JMP LOOP_START

LOOP_END:

    ; Compute total = x + y
    CALL MyClass.sum
    STORE 2          ; total = sum result

    RET
.endmethod