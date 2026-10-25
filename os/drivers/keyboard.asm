.text
public keyboard_interrupt
public keyboard_init
public keyboard_getchar

keyboard_init:
    di
    mvi a, 0
    sta keyboard_head
    sta keyboard_tail
    ei
    ret

keyboard_interrupt:
    push psw
    push b
    push d
    push h
    in 0x01
    ani 1
    jz keyboard_done
    in 0x00
    mov c, a
    lda keyboard_head
    mov l, a
    mvi h, 0
    inr a
    ani 255
    mov b, a
    lda keyboard_tail
    cmp b
    jz keyboard_done
    lxi d, keyboard_buffer
    dad d
    mov m, c
    mov a, b
    sta keyboard_head
keyboard_done:
    pop h
    pop d
    pop b
    pop psw
    ei
    ret

keyboard_getchar:
    push b
    push d
    push h
    di
    lda keyboard_tail
    mov b, a
    lda keyboard_head
    cmp b
    jz keyboard_empty
    mvi h, 0
    mov l, b
    lxi d, keyboard_buffer
    dad d
    mov c, m
    mov a, b
    inr a
    ani 255
    sta keyboard_tail
    mov a, c
    jmp keyboard_getchar_done
keyboard_empty:
    mvi a, -1
keyboard_getchar_done:
    pop h
    pop d
    pop b
    mvi h, 0
    mov l, a
    ei
    ret

.data
keyboard_head: db 0
keyboard_tail: db 0
keyboard_buffer: ds 256
