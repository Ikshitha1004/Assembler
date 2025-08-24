; Error set 1: Invalid mnemonic, wrong operands, bad number

PUHS 5          ; Invalid mnemonic (should be PUSH)
POP 1           ; POP should not have operands
PUSH xyz        ; Invalid number (not numeric)

main:
PUSH 10
LOADR 2         ; Undefined mnemonic (should be LOAD)
