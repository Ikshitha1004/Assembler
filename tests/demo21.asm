; Simple test with labels and arithmetic
start:
    PUSH 5
    PUSH 10
    IADD
    STORE 0

    JMP end

mid:
    PUSH 99

end:
    LOAD 0
    RET
