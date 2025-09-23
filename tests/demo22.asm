.const TEN, 10
.const TWENTY, 20

main:
    PUSH TEN
    PUSH TWENTY
    ICMP_LT
    JZ Lfail
    RET

Lfail:
    PUSH 0
    RET
