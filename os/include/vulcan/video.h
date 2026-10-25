#ifndef VULCANOS_VIDEO_H
#define VULCANOS_VIDEO_H

#include "memory.h"

/** Framebuffer size in pixels; one byte per pixel, row-major from VULCAN_VRAM_START. */
#define VULCAN_VIDEO_WIDTH 1280
#define VULCAN_VIDEO_HEIGHT 720

/** Palette indices in the RGB 3:3:2 layout (bits 7..5 red, 4..2 green, 1..0 blue). */
#define VULCAN_COLOR_INDEX_BLACK 0x00
#define VULCAN_COLOR_INDEX_GRAY 0x92
#define VULCAN_COLOR_INDEX_RED 0xE0
#define VULCAN_COLOR_INDEX_WHITE 0xFF

#endif
