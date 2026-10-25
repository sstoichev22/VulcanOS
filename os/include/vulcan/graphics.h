#ifndef VULCANOS_GRAPHICS_H
#define VULCANOS_GRAPHICS_H

/** Size in pixels of one font cell; the terminal grid is the framebuffer divided into these cells. */
#define VULCAN_GLYPH_WIDTH 16
#define VULCAN_GLYPH_HEIGHT 32

/** Write one palette index at (x, y); coordinates outside the framebuffer are ignored. */
void graphics_pixel(uint16 x, uint16 y, uint8 color);

/** Draw a 16x32 ASCII glyph with its top-left corner at (x, y). A cell that would cross the
    framebuffer edge is not drawn; bytes outside 32..126 draw the '?' glyph. */
void graphics_draw_glyph(uint8 character, uint16 x, uint16 y,
                         uint8 foreground, uint8 background);

/** Move the whole framebuffer up by one glyph row and fill the exposed bottom band with background. */
void graphics_scroll_glyph_row(uint8 background);

#endif
