#include <string.h>
#include <time.h>
#include "vulcan/console.h"
#include "vulcan/graphics.h"
#include "vulcan/video.h"

#define CONSOLE_COLUMNS (VULCAN_VIDEO_WIDTH / VULCAN_GLYPH_WIDTH)
#define CONSOLE_ROWS (VULCAN_VIDEO_HEIGHT / VULCAN_GLYPH_HEIGHT)
#define CONSOLE_TAB_WIDTH 4
#define CONSOLE_BLINK_MS 500
#define CONSOLE_FOREGROUND VULCAN_COLOR_INDEX_WHITE
#define CONSOLE_BACKGROUND VULCAN_COLOR_INDEX_BLACK

static uint16 console_column;
static uint16 console_row;
static uint8 console_text[CONSOLE_ROWS * CONSOLE_COLUMNS];
static uint8 cursor_visible;
static clock_t cursor_toggled_at;

static void cell_draw(uint8 foreground, uint8 background) {
    graphics_draw_glyph(console_text[console_row * CONSOLE_COLUMNS + console_column],
                        console_column * VULCAN_GLYPH_WIDTH, console_row * VULCAN_GLYPH_HEIGHT,
                        foreground, background);
}

/** The cursor is the cell drawn in inverse video, so it shows on any palette that has the console colors. */
static void cursor_draw(uint8 visible) {
    if (visible) cell_draw(CONSOLE_BACKGROUND, CONSOLE_FOREGROUND);
    else cell_draw(CONSOLE_FOREGROUND, CONSOLE_BACKGROUND);
    cursor_visible = visible;
    cursor_toggled_at = clock();
}

static void cursor_hide(void) {
    if (cursor_visible) cursor_draw(0);
}

static void cell_write(uint8 character) {
    console_text[console_row * CONSOLE_COLUMNS + console_column] = character;
    cell_draw(CONSOLE_FOREGROUND, CONSOLE_BACKGROUND);
}

static void console_newline(void) {
    console_column = 0;
    if (console_row + 1 < CONSOLE_ROWS) {
        console_row++;
        return;
    }
    graphics_scroll_glyph_row(CONSOLE_BACKGROUND);
    memmove(console_text, console_text + CONSOLE_COLUMNS, (CONSOLE_ROWS - 1) * CONSOLE_COLUMNS);
    memset(console_text + (CONSOLE_ROWS - 1) * CONSOLE_COLUMNS, ' ', CONSOLE_COLUMNS);
}

static void console_write(int character) {
    if (character == '\r') return;
    if (character == '\n') {
        console_newline();
    } else if (character == '\b' || character == 0x7F) {
        if (console_column > 0) console_column--;
        else if (console_row > 0) { console_row--; console_column = CONSOLE_COLUMNS - 1; }
        else return;
        cell_write(' ');
    } else if (character == '\t') {
        do console_write(' '); while (console_column % CONSOLE_TAB_WIDTH != 0);
    } else {
        cell_write((uint8)character);
        if (++console_column == CONSOLE_COLUMNS) console_newline();
    }
}

static uint16 pointer_cell(float position, uint16 cells) {
    float cell = (position + 1.0f) * 0.5f * cells;
    if (cell < 0.0f) return 0;
    if (cell >= cells) return cells - 1;
    return (uint16)cell;
}

void console_init(void) {
    memset(console_text, ' ', sizeof(console_text));
    console_column = 0;
    console_row = 0;
    cursor_draw(1);
}

void console_put(int character) {
    cursor_hide();
    console_write(character);
    cursor_draw(1);
}

void console_move_to(float x, float y) {
    cursor_hide();
    console_column = pointer_cell(x, CONSOLE_COLUMNS);
    console_row = pointer_cell(-y, CONSOLE_ROWS);
    cursor_draw(1);
}

void console_tick(void) {
    if (clock() - cursor_toggled_at >= CONSOLE_BLINK_MS) cursor_draw(!cursor_visible);
}
