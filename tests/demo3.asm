; Test 3: Array handling

.class_metadata
class_count 0
.end_metadata
.code
.method test_newarray
.limit stack 10
.limit locals 10

; Create INT array of size 3, store in local 0
; local 0 = reference to new array of size 5
PUSH 5       ; size of array
PUSH 0       ; local index to store array reference
NEWARRAY INT ; VM will pop size & local index, type is encoded

; Create FLOAT array of size 2, store in local 1
PUSH 1       ; local index
PUSH 2       ; size
NEWARRAY FLOAT

; Create OBJECT array of size 1, store in local 2
PUSH 2       ; local index
PUSH 1       ; size
NEWARRAY OBJECT

; Test: push sizes to stack and return
LOAD 0       ; load INT array ref
LOAD 1       ; load FLOAT array ref
LOAD 2       ; load OBJECT array ref
RET
.endmethod
