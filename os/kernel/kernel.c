#include <stdio.h>
#include "vulcan/console.h"
#include "vulcan/heap.h"
#include "vulcan/keyboard.h"
#include "vulcan/memory.h"
#include "vulcan/mouse.h"

/** Brings up the kernel subsystems in dependency order. The interrupt-driven drivers (keyboard, mouse)
    are initialized by boot/startup.asm before interrupts are enabled, so they are not repeated here. */
static void kernel_init(void) {
    heap_init((void *)VULCAN_HEAP_START, VULCAN_HEAP_SIZE);
    printf("VulcanOS starting...\n");
    console_init();
}

/** Routes the newest pending click to the console and reports it on the debug console.
    Returns 1 when a click was handled, 0 when none was pending. */
static int kernel_poll_mouse(void) {
    float x, y;
    if (mouse_read(&x, &y) == VULCAN_MOUSE_BUTTON_NONE) return 0;
    printf("Mouse at: x=%f, y=%f\n", x, y);
    console_move_to(x, y);
    return 1;
}

/** Routes one queued keyboard byte to the console. Returns 1 when a byte was handled, 0 when the ring is empty. */
static int kernel_poll_keyboard(void) {
    int character = keyboard_getchar();
    if (character < 0) return 0;
    console_put(character);
    return 1;
}

/** Kernel entry, called by boot/startup.asm. Never returns: each pass services every input source,
    and an idle pass runs the console's periodic work (cursor blink). */
void kernel_main(void) {
    kernel_init();
    while (1) {
        int busy = kernel_poll_mouse();
        busy |= kernel_poll_keyboard();
        if (!busy) console_tick();
    }
}
