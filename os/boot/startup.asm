.text
extern kernel_main
extern keyboard_interrupt
extern keyboard_init
extern mouse_interrupt
extern mouse_init

org 0x00
rst0_vector:
    jmp kernel_start

org 0x10
rst1_vector:
    jmp keyboard_interrupt

org 0x20
rst2_vector:
    jmp mouse_interrupt

org 0x100
global kernel_start
kernel_start:
    di
    lxi sp, 0x100000000
    call mouse_init
    call keyboard_init
    call kernel_main
    stop
