.text
public PUTCHAR
PUTCHAR:
    LXI H, 8
    DAD SP
    MOV A, M
    OUT 0
    LXI H, 0
    MOV L, A
    RET
