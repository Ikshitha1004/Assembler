.class_metadata
class_count 1
class_begin Logger -1
field_count 1
field logCount int 0
method_count 2
method logMessage Logger.logMessage
method Logger Logger.Logger
class_end
.end_metadata

.code

.method Logger.logMessage
.limit stack 3
.limit locals 1

PUSH 1
LOAD_ARG 0        ; 'this'
GETFIELD 0
IADD
LOAD_ARG 0
PUTFIELD 0        ; logCount++
RET
.endmethod

.method Logger.Logger
.limit stack 1
.limit locals 0
RET
.endmethod
