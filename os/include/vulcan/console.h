#ifndef VULCANOS_CONSOLE_H
#define VULCANOS_CONSOLE_H

/** Clears the 80x22 cell buffer, homes the cursor at the top-left cell, and draws it solid.
    Call once before the other console functions. */
void console_init(void);

/** Writes one byte at the cursor and advances it. Printable bytes draw and wrap at the last column;
    '\n' starts the next row, scrolling the screen on the last row; '\r' is ignored; '\b' and 0x7F move
    back one cell, including to the end of the previous row, and erase it; '\t' pads to the next
    four-column stop. The cursor is solid after the call. */
void console_put(int character);

/** Moves the cursor to the cell under a normalized pointer position, x and y in [-1, 1] with x = -1
    at the left and y = +1 at the top; positions outside the range clamp to the edge cells.
    The cursor is solid after the call. */
void console_move_to(float x, float y);

/** Periodic console work for the kernel's idle loop: toggles the cursor between an inverse-video block
    and the plain cell every 500 ms of clock() time, drawing nothing between toggles. */
void console_tick(void);

#endif
