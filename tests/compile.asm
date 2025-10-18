
.code

.method get_value
.limit stack 4
.limit locals 1
PUSH 10
STORE 0 ; Init x
LOAD 0  ; Load local var x
LOAD_ARG 0  ; Load parameter 'y'
IADD
RET
.endmethod

.method set_value
.limit stack 4
.limit locals 2
LOAD_ARG 0  ; Load parameter 'val'
STORE 1 ; Init local
RET
.endmethod

.method main
.limit stack 4
.limit locals 7
PUSH 10
STORE 3 ; Init a_float
FPUSH 5.5
STORE 4 ; Init a_float2
PUSH 3
STORE 5 ; Init z
PUSH 5
NEWARRAY int
STORE 6 ; Store new flattened array to 'my_array'
LOAD 5  ; Load local var z
CALL get_value
STORE 2 ; Store to local 'an_int'
LOAD 3  ; Load local var a_float
STORE 2 ; Store to local 'an_int'
LOAD 3  ; Load local var a_float
PUSH 2
STORE 2 ; Store to local 'an_int'
LOAD 5  ; Load local var z
PUSH 1
IADD
STORE 2 ; Store to local 'an_int'
LOAD 6 ; Load base array ref 'my_array'
PUSH 2
PUSH 100
ASTORE ; Store to array element
RET
.endmethod
