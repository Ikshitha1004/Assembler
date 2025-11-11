.code


.method main
.limit stack 4
.limit locals 191
NEW TestClass ; Create new object of class TestClass
STORE 185 ; Store new Object obj
PUSH 10
LOAD 185 ; Store new Object obj
DUP ; Duplicate object ref for constructor call
INVOKEVIRTUAL 2 1; Call constructor for TestClass
POP ; discard fp
POP ; discard extra reference
LOAD 185  ; Load local var obj
LOAD 185  ; Load local var obj
INVOKEVIRTUAL 0 0; Call TestClass.publicMethod
POP ; discard extra reference 
POP ; discard fp
LOAD 185  ; Load local var obj
PUSH 10
PUTFIELD 0 ; Set field 'publicVar'
NEW AnotherClass ; Create new object of class AnotherClass
STORE 186 ; Store new Object obj2
LOAD 186 ; Store new Object obj2
DUP ; Duplicate object ref for constructor call
INVOKEVIRTUAL 5 0; Call constructor for AnotherClass
POP ; discard fp
POP ; discard extra reference
LOAD 186  ; Load local var obj2
LOAD 186  ; Load local var obj2
INVOKEVIRTUAL 3 0; Call AnotherClass.publicMethod
POP ; discard extra reference 
POP ; discard fp
PUSH 20
PUSH 30
LOAD 186  ; Load local var obj2
LOAD 186  ; Load local var obj2
INVOKEVIRTUAL 4 2; Call AnotherClass.publicMethod@I@I
POP ; discard extra reference 
POP ; discard fp
PUSH 0
STORE 188 ; Init d
PUSH 0
STORE 187 ; Init c
PUSH 0
STORE 189 ; Init k
PUSH 0
STORE 189 ; Store to local 'k'
JMP L8
L0:
LOAD 189  ; Load local var k
PUSH 2
ICMP_EQ
JNZ L12
JMP L3
L12:
JMP L2 ; BREAK
JMP L4
L3:
JMP L1 ; CONTINUE
L4:
PUSH 0
STORE 190 ; Init c1
L1:
LOAD 189 ; Load local 'k'
DUP
PUSH 1
IADD ; ++
STORE 189 ; Store local 'k'
L8:
LOAD 189  ; Load local var k
PUSH 3
ICMP_LT
JNZ L0
JMP L2
L2:
PUSH 0
RET
.endmethod
