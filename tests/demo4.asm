Test Case 4: Object-Oriented Code (Classes, Fields, Methods)
.class_metadata
class_count 2
class_begin TestClass -1
field_count 1
field publicVar int 0
method_count 3
method publicMethod TestClass.publicMethod
method TestClass TestClass.TestClass
method TestClass TestClass.TestClass@int
class_end
class_begin AnotherClass 0
field_count 1
field anotherVar int 0
method_count 4
method anotherMethod AnotherClass.anotherMethod
method publicMethod AnotherClass.publicMethod
method publicMethod AnotherClass.publicMethod@int@int
method AnotherClass AnotherClass.AnotherClass
class_end
.end_metadata

.code

.method TestClass.publicMethod
.limit stack 4
.limit locals 0
LOAD_ARG 0 ; 'this' for assignment to member 'publicVar'
PUSH 10
PUTFIELD 0
.endmethod

.method TestClass.TestClass
.limit stack 4
.limit locals 2
PUSH 0
STORE 1 ; Init i
PUSH 7
STORE 1 ; Store to local 'i'
JMP L0
L1:
LOAD_ARG 0 ; 'this' for assignment to member 'publicVar'
LOAD 0 ; Load 'this' to access member 'publicVar'
GETFIELD 0
LOAD 1  ; Load local var i
IADD
PUTFIELD 0
L2:
LOAD 1 ; Load current value of i for post increment
DUP
PUSH 1
IADD
STORE 1 ; Post increment
L0:
LOAD 1  ; Load local var i
PUSH 10
ICMP_LT
JNZ L1
JMP L3
L3:
RET
.endmethod

.method TestClass.TestClass@int
.limit stack 4
.limit locals 2
LOAD_ARG 0 ; 'this' for assignment to member 'publicVar'
LOAD_ARG 1  ; Load parameter 'x'
PUTFIELD 0
RET
.endmethod

.method AnotherClass.anotherMethod
.limit stack 4
.limit locals 0
LOAD_ARG 0 ; 'this' for assignment to member 'anotherVar'
PUSH 20
PUTFIELD 1
.endmethod

.method AnotherClass.publicMethod
.limit stack 4
.limit locals 0
LOAD_ARG 0 ; 'this' for assignment to member 'anotherVar'
PUSH 15
PUTFIELD 1
.endmethod

.method AnotherClass.publicMethod@int@int
.limit stack 4
.limit locals 3
LOAD_ARG 0 ; 'this' for assignment to member 'anotherVar'
LOAD_ARG 1  ; Load parameter 'x'
LOAD_ARG 2  ; Load parameter 'y'
IADD
PUTFIELD 1
.endmethod

.method AnotherClass.AnotherClass
.limit stack 4
.limit locals 3
PUSH 0
STORE 2 ; Init j
PUSH 0
STORE 2 ; Store to local 'j'
JMP L4
L5:
LOAD_ARG 0 ; 'this' for assignment to member 'anotherVar'
LOAD 0 ; Load 'this' to access member 'anotherVar'
GETFIELD 1
LOAD 2  ; Load local var j
IADD
PUTFIELD 1
L6:
LOAD 2 ; Load current value of j for post increment
DUP
PUSH 1
IADD
STORE 2 ; Post increment
L4:
LOAD 2  ; Load local var j
PUSH 5
ICMP_LT
JNZ L5
JMP L7
L7:
RET
.endmethod

.method main
.limit stack 4
.limit locals 8
NEW TestClass ; Create new object of class TestClass
PUSH 10
INVOKESPECIAL 2 ; Call constructor for TestClass
STORE 2 ; Store new object to 'obj'
LOAD 2  ; Load local var obj
INVOKEVIRTUAL 0 ; Call TestClass.publicMethod
LOAD 2  ; Load local var obj
PUSH 10
PUTFIELD 0 ; Set field 'publicVar'
NEW AnotherClass ; Create new object of class AnotherClass
INVOKESPECIAL 3 ; Call constructor for AnotherClass
STORE 3 ; Store new object to 'obj2'
LOAD 3  ; Load local var obj2
INVOKEVIRTUAL 1 ; Call AnotherClass.publicMethod
LOAD 3  ; Load local var obj2
PUSH 20
PUSH 30
INVOKEVIRTUAL 2 ; Call AnotherClass.publicMethod@int@int
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
