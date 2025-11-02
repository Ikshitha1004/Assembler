.code
.method main
.limit stack 4
.limit locals 8
NEW TestClass ; Create new object of class TestClass
PUSH 10
INVOKESPECIAL 2 ; Call constructor for TestClass
STORE 2 ; Store new object to 'obj'
NEW AnotherClass ; Create new object of class AnotherClass
INVOKESPECIAL 3 ; Call constructor for AnotherClass
STORE 3 ; Store new object to 'obj2'
LOAD 2  ; Load local var obj
INVOKEVIRTUAL 0 ; Call TestClass.publicMethod
LOAD 2  ; Load local var obj
PUSH 10
PUTFIELD 0 ; Set field 'publicVar'
LOAD 3  ; Load local var obj2
INVOKEVIRTUAL 1 ; Call AnotherClass.publicMethod
LOAD 3  ; Load local var obj2
PUSH 30
PUSH 20
INVOKEVIRTUAL 2 ; Call AnotherClass.publicMethod@I@I
PUSH 0
STORE 5 ; Init d
PUSH 0
STORE 4 ; Init c
PUSH 0
STORE 6 ; Init k
PUSH 0
STORE 6 ; Store to local 'k'
JMP L8
L9:
LOAD 6  ; Load local var k
PUSH 2
ICMP_EQ
JNZ L12
JMP L13
L12:
JMP L11 ; BREAK
JMP L14
L13:
JMP L10 ; CONTINUE
L14:
PUSH 0
STORE 7 ; Init c1
L10:
LOAD 6 ; Load current value of k for post increment
DUP
PUSH 1
IADD
STORE 6 ; Post increment
L8:
LOAD 6  ; Load local var k
PUSH 3
ICMP_LT
JNZ L9
JMP L11
L11:
PUSH 0
RET
.endmethod
