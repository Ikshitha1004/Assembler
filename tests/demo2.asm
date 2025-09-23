; Test 2: Object-oriented instructions + control flow

start:
; --- Object creation ---
NEW MyClass           ; allocate object (class_ref = MyClass)
DUP
PUSH 77
PUTFIELD MyClass.myField      ; set field 'myField'
GETFIELD MyClass.myField      ; get field 'myField'
POP

; --- Virtual calls ---
NEW MyClass
DUP
INVOKEVIRTUAL MyClass.myMethod ; call virtual method 'myMethod'

NEW MyClass
DUP
INVOKESPECIAL MyClass.init     ; constructor/private method

; --- Control flow with labels ---
PUSH 0
JZ branch_taken
PUSH 100        ; skipped if zero

branch_taken:
PUSH 200

; Call helper function
CALL helper

; End of program: VM stops naturally after last instruction

helper:
PUSH 55
RET