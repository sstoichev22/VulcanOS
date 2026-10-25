#ifndef VULCAN_STDIO_H
#define VULCAN_STDIO_H

#define EOF (-1)

/** Write formatted bytes to console port 0 and return the emitted byte count.
   Supports %d/%i, %u, %x/%X, %o, %p, %c, %s and %%; l/ll/z select wide
   integer arguments. %f/%F supports precision 0..9 (default 6). Pointer
   values are lowercase hexadecimal without a 0x prefix. */
int printf(const char *format, ...);

/** Emit value's low byte to console port 0 and return that unsigned byte. */
int putchar(int value);

/** Wait for and return the next input byte (0..255); blocks while input is
   empty. The keyboard device has no end-of-file state, so EOF is not returned. */
int getchar(void);

/** Write text followed by a line feed and return the number of bytes written,
   including the line feed. */
int puts(const char *text);

#endif
