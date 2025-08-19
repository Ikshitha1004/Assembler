        .text
        .global _start
_start:
        MOV     R0, #1          @ set exit code
        ADD   R1, R2, R3      @ conditional add with suffix
        LDR     R4, [R5, #8]!
        STR     R0, [SP, #-4]!
        B       done

        .data
msg:    .asciz  "Hello, ARM!"
arr:    .word   1, 2, 3

        .bss
buffer: .skip   16

        .section .rodata
pi:     .word   0x40490fdb

done:
        NOP
