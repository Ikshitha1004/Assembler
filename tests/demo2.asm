; Test 2: Memory,comparision, Nested control flow ,syscalls
.class_metadata
class_count 0
.end_metadata

.code

.method read_and_compute
.limit stack 8
.limit locals 5

; --- Syscall: READ ---
PUSH 0          ; buffer address
PUSH 16         ; size = 16 bytes
PUSH 0          ; fd = stdin
SYS_CALL READ   ; result (bytes read)
STORE 0         ; locals[0] = bytesRead

; --- Initialize locals ---
PUSH 0
STORE 1         ; locals[1] = sum
PUSH 0
STORE 2         ; locals[2] = i

; --- Outer loop ---
L_outer_start:
LOAD 2          ; i
PUSH 5
ICMP_LT
JZ L_outer_end

; Inner conditional based on bytesRead
LOAD 0
PUSH 10
ICMP_GT
JZ L_skip_inner  ; if bytesRead <= 10, skip

    ; --- Inner loop ---
    PUSH 0
    STORE 3      ; locals[3] = j = 0
    L_inner_start:
    LOAD 3
    PUSH 3
    ICMP_LT
    JZ L_inner_end

        LOAD 1
        LOAD 2
        IADD
        STORE 1  ; sum += i

        LOAD 3
        PUSH 1
        IADD
        STORE 3  ; j++

        JMP L_inner_start
    L_inner_end:

L_skip_inner:

; After inner block
LOAD 2
PUSH 1
IADD
STORE 2          ; i++

JMP L_outer_start
L_outer_end:

; --- Comparison result ---
LOAD 1
PUSH 20
ICMP_GT
JZ less_equal_case
JNZ greater_case

less_equal_case:
PUSH 100
STORE 4
JMP L_end

greater_case:
PUSH 200
STORE 4

L_end:

; --- Another syscall to WRITE result ---
LOAD 4
SYS_CALL WRITE

RET
.endmethod
