/*
 * Test if palette initialization is the issue
 * White screen could mean palette is all white on CGB
 */

#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include <stdio.h>

void main(void) {
    // Set a default palette for CGB
    // The first color is background, last is foreground for text
    if (_cpu == CGB_TYPE) {
        // DMG/Original GB palette: light background, dark text
        uint16_t palette[] = {
            RGB(28, 28, 24),  // Light greenish (GB screen color)
            RGB(20, 20, 16),  // Medium
            RGB(8, 8, 8),     // Dark gray
            RGB(0, 0, 0)      // Black (text should use this)
        };
        set_bkg_palette(0, 1, palette);
    }

    printf("\n\n  PALETTE TEST\n\n");
    printf("  Can you see\n");
    printf("  this text?\n\n");

    while(1) {
        uint8_t keys = joypad();

        gotoxy(0, 8);
        printf("Keys: %02X", keys);

        vsync();
    }
}