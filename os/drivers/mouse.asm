.text
public mouse_interrupt
public mouse_init
public mouse_getx
public mouse_gety
public mouse_getbutton
public mouse_read

mouse_init:
    push psw
    push b
    push h
    lxi h, mouse_x
    mvi b, 10
    mvi a, 0
mouse_clear:
    mov m, a
    inx h
    dcr b
    jnz mouse_clear
    pop h
    pop b
    pop psw
    ret

mouse_interrupt:
    push psw
    push b
    push h
    mvi a, 0
    out 0x22
    in 0x24
    sta mouse_button
    lxi h, mouse_x
    mvi b, 0
mouse_snapshot_read:
    in 0x23
    mov m, a
    inx h
    inr b
    mov a, b
    cpi 8
    jz mouse_done
    out 0x22
    jmp mouse_snapshot_read
mouse_done:
    mvi a, 1
    sta mouse_pending
    pop h
    pop b
    pop psw
    ei
    ret

mouse_getx:
    lxi h, mouse_x + 3
    jmp mouse_getfloat
mouse_gety:
    lxi h, mouse_y + 3
mouse_getfloat:
    di
    mov a, m
    dcx h
    rlc 8
    ora m
    dcx h
    rlc 8
    ora m
    dcx h
    rlc 8
    ora m
    mvi h, 0
    mov l, a
    ei
    ret

mouse_getbutton:
    lda mouse_button
    mvi h, 0
    mov l, a
    ret

mouse_read:
    push b
    lxi h, 16
    dad sp
    mov b, h
    mov c, l
    push d
    di
    lda mouse_pending
    ora a
    jz mouse_no_event
    ldbc32 0
    mov e, a
    ldbc32 4
    mov d, a
    push d
    pop h
    lda mouse_x
    mov m, a
    inx h
    lda mouse_x + 1
    mov m, a
    inx h
    lda mouse_x + 2
    mov m, a
    inx h
    lda mouse_x + 3
    mov m, a
    ldbc32 8
    mov e, a
    ldbc32 12
    mov d, a
    push d
    pop h
    lda mouse_y
    mov m, a
    inx h
    lda mouse_y + 1
    mov m, a
    inx h
    lda mouse_y + 2
    mov m, a
    inx h
    lda mouse_y + 3
    mov m, a
    lda mouse_button
    mov e, a
    mvi a, 0
    sta mouse_pending
    mov a, e
    jmp mouse_read_done
mouse_no_event:
    mvi a, 0
mouse_read_done:
    pop d
    pop b
    mvi h, 0
    mov l, a
    ei
    ret
.data
mouse_x: ds 4
mouse_y: ds 4
mouse_button: db 0
mouse_pending: db 0
