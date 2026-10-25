# VulcanOS architecture

## Source layout

```
boot/startup.asm          RST vectors, stack, driver init, jump to kernel_main
kernel/kernel.c           subsystem init order and the input/idle event loop
kernel/console.c          text console: cell grid, cursor, blink, scrolling
mm/heap.c                 kernel heap (kmalloc/kfree/kcalloc/krealloc)
drivers/keyboard.asm      RST 1 keyboard ISR and ring buffer
drivers/mouse.asm         RST 2 mouse ISR and click snapshot
drivers/video/graphics.c  framebuffer pixel, glyph blit, scroll
drivers/video/font16x32.h 16x32 glyph ROM (private to the video driver)
include/vulcan/*.h        public kernel headers, included as "vulcan/<name>.h"
```

New subsystems get their own directory and a header under `include/vulcan/`; the kernel wires them
up in `kernel_init` (or `boot/startup.asm` for interrupt-driven drivers that must be ready before EI).
The compiler finds `include/` by walking up from each source file, so nested modules need no `-I`.

## Build

From the `vulcanos` folder in Vulcan's terminal (or open `os/build.run`):

```
vcc os/boot/startup.asm os/drivers/keyboard.asm os/drivers/mouse.asm os/kernel/kernel.c os/kernel/console.c os/mm/heap.c os/drivers/video/graphics.c -o VulcanOS.exe
./VulcanOS.exe
```

The kernel keeps its startup banner on the console and draws keyboard input directly
to the framebuffer. The console submits lines on Enter (including LF); clicking the
display and typing supplies individual bytes through the same Java CPU input API.

Text uses one built-in 16x32 printable-ASCII ROM (`font16x32.h`) for bytes 32 through 126,
giving an 80-column by 22-row terminal on the 1280x720 framebuffer. Each glyph is stored as
thirty-two `uint32` row masks; the most significant bit is the leftmost of the sixteen pixels and
the low sixteen bits are zero. The bitmaps were rasterized from Cascadia Mono (SIL Open Font
License 1.1) at 8x supersampling and thresholded to on/off pixels. `graphics_draw_glyph` blits a
cell with an inline-assembly loop (about 3,800 instructions, roughly 0.3 ms at the 100 MHz cap).
Unsupported bytes use the visible ? glyph.

`kernel.c` only boots the services and runs the input loop; `console.c` (declared in `vulcan/console.h`)
owns the console grid, cursor, and blinking. `console_put` handles one byte: printable bytes draw at the
cursor cell and advance; the last column wraps to the next row. Line feed starts the next row,
carriage return is ignored, backspace (0x08 or 0x7F) moves back one cell, including to the end of
the previous row, and erases it, and tab advances to the next four-column stop. A newline on the
last row calls `graphics_scroll_glyph_row`, which moves the framebuffer up one glyph row (about
158 ms at the 100 MHz cap) and clears the bottom band, instead of wrapping over old text. A mouse
click calls `console_move_to`, which moves the cursor to the clicked cell, snapped and clamped to the
80x22 grid. The cursor starts in the top-left cell, drawn at boot.
The cursor is the cell in inverse video (black on white), so it shows on any display palette that has the
console colors; `console_tick` toggles it every 500 ms of `clock()` time and it stays solid after each
key or click. The console keeps one byte per cell so hiding
the cursor restores the character underneath; that buffer scrolls with the framebuffer.

Public OS headers live under `include/vulcan/`: `memory.h` owns the framebuffer
base address and the boot heap range, while `video.h` owns the pixel dimensions and the RGB 3:3:2
palette indices.
`graphics_pixel` writes one 8-bit palette index per row-major pixel and ignores
coordinates outside that framebuffer. Glyph drawing validates a
complete cell before writing. They calculate the first pixel address once, then
advance one byte per column and one framebuffer width per row. The IDE's `vos` display
preset decodes every index as RGB 3:3:2 (`0x00` black, `0x92` gray, `0xE0` red, `0xFF` white); an
unedited older `vos` preset that colored only black, red, and white is upgraded on load.

The OS header `mouse.h` documents the byte-wide mouse ports: OUT `0x22` selects a snapshot byte,
IN `0x23` reads it, and IN `0x24` returns the captured button number (0 none,
1 left, 2 middle, 3 right). OUT command 0 snapshots two IEEE-754 binary32 values
in little-endian order (X bytes 0â€“3, then Y bytes 4â€“7); commands 1â€“7 select the
corresponding byte. Coordinates are normalized to [-1, 1], with X=-1 at the left,
X=+1 at the right, Y=+1 at the top, and Y=-1 at the bottom. The display clamps
pointer positions in the letterbox area to the nearest image edge. Binary32 keeps
the C interface direct; a 16-bit fixed-point pair would use fewer guest reads and
less software arithmetic on a strict 8080 target.

The production kernel consumes mouse interrupts independently of keyboard input and prints one normalized X/Y line for each consumed click. Holding a button, releasing it, or moving without pressing does not print another line.

`startup.asm` owns the fixed vector ORGs and the executable entry at 0x100.
RST 0 at 0x00 resets the stack, initializes mouse and keyboard state, and calls `kernel_main`.
RST 1 at 0x10 jumps directly to the keyboard ISR. RST 2 at 0x20 jumps to a
mouse handler that saves the last press coordinates and button. Each sixteen-byte slot fits
Vulcan's nine-byte JMP, so no bridge vectors or extra stack adjustment are needed.
RST 3 through RST 7 remain available at 0x30 through 0x70. Other kernel modules
use relocatable .text/.data with no fixed ORGs.
Java `offerKeyInput` queues bytes in arrival order and asserts a keyboard interrupt
at 0x10 while data remains unread. DI defers it; EI enables it after the following
instruction. It wakes HLT and uses an eight-byte return address. Explicit interrupts
requested through `triggerInterrupt` retain priority and are not replaced by a key.
Mouse-button presses request RST 2 at 0x20, after explicit vectors and before keyboard
input; movement remains available through the mouse coordinate ports.
IN 0x01 bit 0 reports queued input; IN 0x00 consumes one byte. OUT 0x00 remains
independent display output. Polling programs may continue consuming the same ports
with interrupts disabled. Reset discards pending keyboard input.

The ISR preserves PSW, BC, DE, HL and the interrupted stack, then uses EI/RET.
Its 256-byte ring holds up to 255 unread bytes; it consumes and drops new bytes
when full. The emulator FIFO holds up to 8192 bytes and drops new bytes when full.
`keyboard_getchar()` returns 0..255 or -1, using the widened C ABI (HL/A result,
BC preserved). Call it from normal kernel code with interrupts enabled, not an ISR.

Run `./build.ps1` to compile and run the keyboard, glyph-rendering, and existing
regression checks. The keyboard check compiles the current user kernel, then links
a controlled test kernel to exercise boot and RST 0 restart, byte FIFO, polling,
register/stack restoration, and halted console delivery. The glyph check boots the
production kernel and verifies packed ROM pixels, framebuffer bounds, cursor advance,
line feed, backspace, tabs, wrapping, scrolling, click-to-cell snapping, and the absence of
typed-character console echo.

The OS header `mouse.h` declares `mouse_getx()`, `mouse_gety()`, and
`mouse_getbutton()`. They return the last press snapshot, initially zero,
retained after release. Coordinate getters require normal kernel code with
interrupts enabled. Each getter is atomic; separate calls can observe different
presses if an interrupt arrives between calls. The ISR preserves registers and
flags and returns with EI/RET. Pending presses coalesce to the newest press.
Use `mouse_read(&x, &y)` from normal kernel code with interrupts enabled.
It atomically copies one coherent press snapshot into two writable floats and
returns the button, or `VULCAN_MOUSE_BUTTON_NONE` when no new press is pending
(leaving the floats unchanged). A second press of the same button is a new event.
The OS retains only the latest unconsumed click; rapid pending clicks coalesce.
Startup calls `mouse_init()` before enabling interrupts; the kernel must not
reset mouse state again after boot. The legacy getters remain available for
inspecting the last press, but separate getter calls are not one atomic snapshot.
