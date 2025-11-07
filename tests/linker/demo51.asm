;Library linking

.class_metadata
class_count 0
.end_metadata

.code
#include "io"
.method main
.limit stack 10
.limit locals 5

; Create IOHandler instance
NEW IOHandler
STORE 0

; Call IOHandler.readChar()
LOAD 0
CALL IOHandler.readChar
POP

RET
.endmethod
