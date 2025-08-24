; Error set 2: Duplicate labels, undefined labels, and more mnemonics

start:
PUSH 1
JMP end

start:          ; Duplicate label

PUSH 2
JNZ missing     ; Jumps to an undefined label "missing"

DONEZ 3         ; Invalid mnemonic (should be JNZ)
