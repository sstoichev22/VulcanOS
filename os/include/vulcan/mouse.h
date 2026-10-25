#ifndef VULCANOS_MOUSE_H
#define VULCANOS_MOUSE_H

/** Mouse ports: OUT selects snapshot byte 0..7 (0 also captures X then Y as little-endian binary32),
    IN data reads the selected byte, IN button reads the captured button. Presses request RST 2. */
#define VULCAN_MOUSE_COMMAND 0x22
#define VULCAN_MOUSE_DATA 0x23
#define VULCAN_MOUSE_BUTTON 0x24
#define VULCAN_MOUSE_INTERRUPT_VECTOR 0x20

#define VULCAN_MOUSE_BUTTON_NONE 0
#define VULCAN_MOUSE_BUTTON_LEFT 1
#define VULCAN_MOUSE_BUTTON_MIDDLE 2
#define VULCAN_MOUSE_BUTTON_RIGHT 3

/** Clears the saved click; startup calls it before enabling interrupts. */
void mouse_init(void);

/** Last press X or Y in [-1, 1], initially zero; call from kernel code with interrupts enabled.
    Each getter is atomic, but a new press can arrive between separate getter calls. */
float mouse_getx(void);
float mouse_gety(void);

/** Last pressed button (VULCAN_MOUSE_BUTTON_*), retained after release. */
uint8 mouse_getbutton(void);

/** Consumes the newest pending press, copying its coordinates to *x and *y and returning its button,
    or returns VULCAN_MOUSE_BUTTON_NONE and leaves both floats unchanged when no press is pending.
    Call from normal kernel code with interrupts enabled. */
uint8 mouse_read(float *x, float *y);

#endif
