.text
public getchar
getchar:
    IN 1
    ANI 1
    JZ getchar
    IN 0
    LXI H, 0
    MOV L, A
    RET
