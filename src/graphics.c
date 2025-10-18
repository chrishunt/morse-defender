#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"

// Simple color palette for Game Boy Color
const uint16_t bg_palette[] = {
    RGB(31, 31, 31),  // White
    RGB(21, 21, 21),  // Light gray
    RGB(10, 10, 10),  // Dark gray
    RGB(0, 0, 0)      // Black
};

// Title screen palette with colors
const uint16_t title_palette[] = {
    RGB(31, 31, 25),  // Light yellow
    RGB(0, 15, 0),    // Green
    RGB(0, 0, 15),    // Blue
    RGB(0, 0, 0)      // Black
};

void init_graphics(void) {
    // Set up color palettes for Game Boy Color
    if (_cpu == CGB_TYPE) {
        // Set background palette 0
        set_bkg_palette(0, 1, bg_palette);
        // Set background palette 1 for title
        set_bkg_palette(1, 1, title_palette);
    }
}

void display_title(void) {
    // Clear the screen first
    clear_screen();

    // Display title text
    printf("\n\n\n");
    printf("   MORSE CODE\n");
    printf("      GAME\n");
    printf("\n\n");
    printf("  Press START\n");
    printf("\n\n");
    printf(" A: Dot  (.)\n");
    printf(" B: Dash (-)\n");
}

void clear_screen(void) {
    // Clear the background
    for (uint8_t y = 0; y < 20; y++) {
        for (uint8_t x = 0; x < 20; x++) {
            set_bkg_tile_xy(x, y, ' ');
        }
    }
}

void display_text(uint8_t x, uint8_t y, const char* text) {
    uint8_t len = strlen(text);
    for (uint8_t i = 0; i < len; i++) {
        set_bkg_tile_xy(x + i, y, text[i]);
    }
}

void update_morse_display(void) {
    // This will be implemented to show morse code patterns
    // For now, just a placeholder
    display_text(1, 1, "MORSE:");
}