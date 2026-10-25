.text
public time
time:
    PUSH B
    XRA A
    OUT 32
    IN 33
    MOV E, A
    LXI H, 0
    MVI B, 7
time_next_byte:
    MOV A, B
    OUT 32
    CALL time_shift_byte
    IN 33
    ORA L
    MOV L, A
    DCR B
    JNZ time_next_byte
    CALL time_shift_byte
    MOV A, E
    ORA L
    MOV L, A
    POP B
    RET
time_shift_byte:
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    RET
