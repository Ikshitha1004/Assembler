.class_metadata
class_count 1
class_begin TestClass -1
field_count 1
field publicVar int 0
method_count 2
method TestClass TestClass.TestClass
method TestClass TestClass.TestClass@int
class_end
.end_metadata

.code 
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




