#ifndef VULCANOS_KEYBOARD_H
#define VULCANOS_KEYBOARD_H

/** Clears the keyboard ring buffer; startup calls it before enabling interrupts. */
void keyboard_init(void);

/** Returns the next queued keyboard byte (0..255), or -1 when the ring is empty. Call from normal kernel code with interrupts enabled. */
int keyboard_getchar(void);

#endif
