#include "vulcan/graphics.h"
#include "vulcan/video.h"
#include "font16x32.h"

void graphics_pixel(uint16 x, uint16 y, uint8 color) {
    if (x >= VULCAN_VIDEO_WIDTH || y >= VULCAN_VIDEO_HEIGHT) return;

    uint32 pixelAddress = (uint32)VULCAN_VRAM_START + (uint32)y * VULCAN_VIDEO_WIDTH + x;
    *(uint8 *)pixelAddress = color;
}

/** Draws one 16x32 glyph cell with a register loop: BC walks the font rows, HL the framebuffer,
    D and E hold the foreground and background palette indices. Arguments follow the C ABI slots at BC+8n. */
void graphics_draw_glyph(uint8 character, uint16 x, uint16 y,
                         uint8 foreground, uint8 background) {
    asm {
    LDBC32 16
    ANI 0xFFFF
    CPI VULCAN_VIDEO_HEIGHT - VULCAN_GLYPH_HEIGHT + 1
    JNC $done
    MOV L, A
    MVI H, 0
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    MOV D, H
    MOV E, L
    DAD H
    DAD H
    DAD D
    LDBC32 8
    ANI 0xFFFF
    CPI VULCAN_VIDEO_WIDTH - VULCAN_GLYPH_WIDTH + 1
    JNC $done
    MOV E, A
    MVI D, 0
    DAD D
    LXI D, VULCAN_VRAM_START
    DAD D
    PUSH H
    LDBC 0
    CPI 32
    JC $fallback
    CPI 127
    JC $glyph
$fallback:
    MVI A, 63
$glyph:
    MOV L, A
    MVI H, 0
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    DAD H
    LXI D, $vulcan_font16x32
    DAD D
    LDBC 24
    MOV D, A
    LDBC 32
    MOV E, A
    MOV B, H
    MOV C, L
    POP H
    MVI A, 32
$row:
    PUSH PSW
    PUSH H
    LDBC32 0
    INX B
    INX B
    INX B
    INX B
    PUSH B
    MVI B, 16
$pixel:
    ADD A
    JNC $background
    MOV M, D
    INX H
    DCR B
    JNZ $pixel
    JMP $row_end
$background:
    MOV M, E
    INX H
    DCR B
    JNZ $pixel
$row_end:
    POP B
    POP H
    PUSH D
    LXI D, VULCAN_VIDEO_WIDTH
    DAD D
    POP D
    POP PSW
    DCR A
    JNZ $row
$done:
    }
}

/** Copies framebuffer rows 32..719 to 0..687 eight words per pass (BC is the destination; the source is
    one glyph row, 40960 bytes, ahead), then fills rows 688..719 with the background byte repeated in A. */
void graphics_scroll_glyph_row(uint8 background) {
    asm {
    LDBC 0
    MOV D, A
    LXI B, VULCAN_VRAM_START
    MVI E, (VULCAN_VIDEO_HEIGHT - VULCAN_GLYPH_HEIGHT) * VULCAN_VIDEO_WIDTH / 32
$copy:
    LDBC32 40960
    STBC32 0
    LDBC32 40964
    STBC32 4
    LDBC32 40968
    STBC32 8
    LDBC32 40972
    STBC32 12
    LDBC32 40976
    STBC32 16
    LDBC32 40980
    STBC32 20
    LDBC32 40984
    STBC32 24
    LDBC32 40988
    STBC32 28
    LXI H, 32
    DAD B
    MOV B, H
    MOV C, L
    DCR E
    JNZ $copy
    MOV A, D
    RLC 8
    ORA D
    MOV L, A
    RLC 16
    ORA L
    MVI E, VULCAN_GLYPH_HEIGHT * VULCAN_VIDEO_WIDTH / 32
$clear:
    STBC32 0
    STBC32 4
    STBC32 8
    STBC32 12
    STBC32 16
    STBC32 20
    STBC32 24
    STBC32 28
    LXI H, 32
    DAD B
    MOV B, H
    MOV C, L
    DCR E
    JNZ $clear
    }
}
