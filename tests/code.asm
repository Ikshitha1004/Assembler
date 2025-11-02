.class_metadata
class_count 2
class_begin TestClass None
field_count 1
field publicVar int 0
method_count 3
method publicMethod TestClass.publicMethod
method TestClass TestClass.TestClass
method TestClass TestClass.TestClass@int
class_end
class_begin AnotherClass TestClass
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
RET
.endmethod

.method TestClass@constructor
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
L0:
LOAD 1  ; Load local var i
PUSH 10
ICMP_LT
JNZ L1
JMP L3
L3:
RET
.endmethod

.method TestClass@int@constructor
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
RET
.endmethod

.method AnotherClass.publicMethod
.limit stack 4
.limit locals 0
LOAD_ARG 0 ; 'this' for assignment to member 'anotherVar'
PUSH 15
PUTFIELD 1
RET
.endmethod

.method AnotherClass.publicMethod@int@int
.limit stack 4
.limit locals 3
LOAD_ARG 0 ; 'this' for assignment to member 'anotherVar'
LOAD_ARG 1  ; Load parameter 'x'
LOAD_ARG 2  ; Load parameter 'y'
IADD
PUTFIELD 1
RET
.endmethod

.method AnotherClass@constructor
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
L4:
LOAD 2  ; Load local var j
PUSH 5
ICMP_LT
JNZ L5
JMP L7
L7:
RET
.endmethod
