.code
.method read_example
.limit stack 3
.limit locals 3

; Assume locals[0..2] are available

; --- Prepare arguments for SYS_CALL READ ---
PUSH 0       ; localsIdx = 0 (store buffer reference here)
PUSH 16      ; size = 16 bytes to read
PUSH 0       ; fd = 0 (file descriptor in fileData[0])

SYS_CALL READ ; Perform read, bytesRead pushed onto stack

STORE 2      ; Store bytesRead into locals[2]

; Example arithmetic after read
LOAD 2       ; Load bytesRead
PUSH 5
IADD         ; Add 5
STORE 1      ; Store result in locals[1]

RET
.endmethod
