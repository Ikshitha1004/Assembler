.class_metadata
class_count 2
class_begin TestClass None
field_count 1
field publicVar I 0
method_count 3
method publicMethod TestClass.publicMethod
method TestClass TestClass.TestClass
method TestClass TestClass.TestClass@I
class_end
class_begin AnotherClass TestClass 
field_count 2
field publicVar I 0
field anotherVar I 0
method_count 4
method publicMethod AnotherClass.publicMethod
method publicMethod AnotherClass.publicMethod
method publicMethod AnotherClass.publicMethod@I@I
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

.method TestClass.TestClass
.limit stack 10
.limit locals 183
LOAD_ARG 0      ; Push 'this' reference for field 'publicVar'
PUSH 0      ; Default value for 'publicVar'
PUTFIELD 0 ; Store 0 to 'publicVar'
PUSH 0
STORE 180 ; Init i
PUSH 7
STORE 180 ; Store to local 'i'
JMP L0
L1:
LOAD_ARG 0 ; 'this' for assignment to member 'publicVar'
LOAD_ARG 0 ; Load 'this' to access member 'publicVar'
GETFIELD 0
LOAD 180  ; Load local var i
IADD
PUTFIELD 0
L2:
LOAD 180 ; Load local 'i'
DUP
PUSH 1
IADD ; ++
STORE 180 ; Store local 'i'
L0:
LOAD 180  ; Load local var i
PUSH 10
ICMP_LT
JNZ L1
JMP L3
L3:
RET
.endmethod

.method TestClass.TestClass@I
.limit stack 10
.limit locals 184
LOAD_ARG 0      ; Push 'this' reference for field 'publicVar'
PUSH 0      ; Default value for 'publicVar'
PUTFIELD 0 ; Store 0 to 'publicVar'
LOAD_ARG 1 ; Copy arg 'x' to local
STORE 181
LOAD_ARG 0 ; 'this' for assignment to member 'publicVar'
LOAD 181  ; Load parameter 'x'
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

.method AnotherClass.publicMethod@I@I
.limit stack 4
.limit locals 184
LOAD_ARG 0 ; Copy arg 'x' to local
STORE 182
LOAD_ARG 1 ; Copy arg 'y' to local
STORE 183
LOAD_ARG 0 ; 'this' for assignment to member 'anotherVar'
LOAD 182  ; Load parameter 'x'
LOAD 183  ; Load parameter 'y'
IADD
PUTFIELD 1
RET
.endmethod

.method AnotherClass.AnotherClass
.limit stack 10
.limit locals 188
LOAD_ARG 0      ; Push 'this' reference for field 'publicVar'
PUSH 0      ; Default value for 'publicVar'
PUTFIELD 0 ; Store 0 to 'publicVar'
LOAD_ARG 0      ; Push 'this' reference for field 'anotherVar'
PUSH 0      ; Default value for 'anotherVar'
PUTFIELD 1 ; Store 0 to 'anotherVar'
PUSH 0
STORE 184 ; Init j
PUSH 0
STORE 184 ; Store to local 'j'
JMP L4
L5:
LOAD_ARG 0 ; 'this' for assignment to member 'anotherVar'
LOAD_ARG 0 ; Load 'this' to access member 'anotherVar'
GETFIELD 1
LOAD 184  ; Load local var j
IADD
PUTFIELD 1
L6:
LOAD 184 ; Load local 'j'
DUP
PUSH 1
IADD ; ++
STORE 184 ; Store local 'j'
L4:
LOAD 184  ; Load local var j
PUSH 5
ICMP_LT
JNZ L5
JMP L7
L7:
RET
.endmethod

