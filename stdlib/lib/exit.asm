.text
public exit
exit:
    LXI H, 8
    DAD SP
    MOV A, M
    STOP
