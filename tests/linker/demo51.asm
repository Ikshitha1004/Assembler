.class_metadata
class_count 0
.end_metadata

.code
#include "StringHandler"

.method main
.limit stack 5
.limit locals 183
NEW StringHandler ; Create new object of class StringHandler
STORE 180 ; Store new Object handler
LOAD 180 ; Store new Object handler
DUP ; Duplicate object ref for constructor call
INVOKEVIRTUAL 12 0; Call constructor for StringHandler
POP ; discard fp
POP ; discard extra reference
;Generating array initialization for 'str'

PUSH 12
NEWARRAY C ; allocating array
DUP ; duplicate array ref for initialization
DUP ; array_ref for ASTORE
PUSH 0 ; flat index
PUSH 72 ; Push ASCII for char 'H'
ASTORE ; Store value at flat index 0
DUP ; array_ref for ASTORE
PUSH 1 ; flat index
PUSH 101 ; Push ASCII for char 'e'
ASTORE ; Store value at flat index 1
DUP ; array_ref for ASTORE
PUSH 2 ; flat index
PUSH 108 ; Push ASCII for char 'l'
ASTORE ; Store value at flat index 2
DUP ; array_ref for ASTORE
PUSH 3 ; flat index
PUSH 108 ; Push ASCII for char 'l'
ASTORE ; Store value at flat index 3
DUP ; array_ref for ASTORE
PUSH 4 ; flat index
PUSH 111 ; Push ASCII for char 'o'
ASTORE ; Store value at flat index 4
DUP ; array_ref for ASTORE
PUSH 5 ; flat index
PUSH 32 ; Push ASCII for char ' '
ASTORE ; Store value at flat index 5
DUP ; array_ref for ASTORE
PUSH 6 ; flat index
PUSH 87 ; Push ASCII for char 'W'
ASTORE ; Store value at flat index 6
DUP ; array_ref for ASTORE
PUSH 7 ; flat index
PUSH 111 ; Push ASCII for char 'o'
ASTORE ; Store value at flat index 7
DUP ; array_ref for ASTORE
PUSH 8 ; flat index
PUSH 114 ; Push ASCII for char 'r'
ASTORE ; Store value at flat index 8
DUP ; array_ref for ASTORE
PUSH 9 ; flat index
PUSH 108 ; Push ASCII for char 'l'
ASTORE ; Store value at flat index 9
DUP ; array_ref for ASTORE
PUSH 10 ; flat index
PUSH 100 ; Push ASCII for char 'd'
ASTORE ; Store value at flat index 10
DUP ; array_ref for ASTORE
PUSH 11 ; flat index
PUSH 92 ; Push ASCII for char '\0'
ASTORE ; Store value at flat index 11
POP ; pop initialized array reference
STORE 181 ; Store initialized array to 'str'

;Completed array initialization for 'str'

PUSH 6
NEWARRAY C
STORE 182 ; Store new flattened array to 'sstr'
LOAD 182  ; Load local var sstr
PUSH 5
PUSH 0
LOAD 181  ; Load local var str
LOAD 180  ; Load local var handler
LOAD 180  ; Load local var handler
INVOKEVIRTUAL 1 4; Call StringHandler.substr@[C@I@I@[C
POP ; discard extra reference 
POP ; discard fp
PUSH 0
RET
.endmethod
