; Error set 3: Collects everything for robustness testing

PUH 10          ; Invalid mnemonic
PUSH abc        ; Invalid number
POP 2           ; Extra operand
main:
main:           ; Duplicate label
LOADR 3         ; Undefined mnemonic
JMP nowhere     ; Undefined label
RET 5           ; RET should not take operands
