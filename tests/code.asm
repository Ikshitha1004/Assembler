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




