; Demo: computes (2 + 3) * 4, stores to local[0], compares, and branches
PUSH 2
PUSH 3
IADD
PUSH 4
IMUL
STORE 0

LOAD 0
PUSH 20
ICMP_LT
JZ end

NEW 1
DUP
PUTFIELD 2
POP

JMP done

end:
LOAD 0
PUSH 20
ICMP_GT
JNZ alt

CALL func
JMP done

alt:
CALL func

func:
RET

done:
JMP done