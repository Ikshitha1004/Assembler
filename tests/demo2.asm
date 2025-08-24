; Test 2: Object-oriented instructions + control flow

start:
; --- Object creation ---
NEW 1           ; allocate object (class_ref = 1)
DUP
PUSH 77
PUTFIELD 2      ; set field #2
GETFIELD 2      ; get field #2
POP

; --- Virtual calls ---
NEW 1
DUP
INVOKEVIRTUAL 5 ; call virtual method (meth_ref = 5)

NEW 1
DUP
INVOKESPECIAL 6 ; constructor/private method

; --- Control flow with labels ---
PUSH 0
JZ branch_taken
PUSH 100        ; skipped if zero
JMP done

branch_taken:
PUSH 200

done:
CALL helper
JMP done        ; infinite loop

helper:
PUSH 55
RET
