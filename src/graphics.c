#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "morse_input.h"

// Color palettes for Game Boy Color
const uint16_t sky_palette[] = {
    RGB(20, 24, 31),  // Dark blue sky
    RGB(15, 20, 28),  // Darker blue
    RGB(10, 15, 20),  // Very dark blue
    RGB(0, 0, 0)      // Black
};

const uint16_t missile_palette[] = {
    RGB(31, 31, 31),  // White (for letters)
    RGB(31, 20, 20),  // Light red
    RGB(31, 10, 10),  // Red
    RGB(20, 0, 0)     // Dark red
};

const uint16_t city_palette[] = {
    RGB(20, 20, 20),  // Light gray
    RGB(15, 15, 15),  // Gray
    RGB(10, 10, 10),  // Dark gray
    RGB(5, 5, 5)      // Very dark gray
};

const uint16_t explosion_palette[] = {
    RGB(31, 31, 0),   // Yellow
    RGB(31, 20, 0),   // Orange
    RGB(31, 10, 0),   // Red-orange
    RGB(20, 0, 0)     // Dark red
};

void init_graphics(void) {
    // Set up color palettes for Game Boy Color
    if (_cpu == CGB_TYPE) {
        // Background palettes
        set_bkg_palette(0, 1, sky_palette);

        // Sprite palettes
        set_sprite_palette(0, 1, missile_palette);
        set_sprite_palette(1, 1, city_palette);
        set_sprite_palette(2, 1, explosion_palette);
    }
}

void clear_screen(void) {
    // Don't use set_bkg_tile_xy - it corrupts the display!
    // Just use gotoxy and spaces like the working test program
    for (uint8_t y = 0; y < 18; y++) {
        gotoxy(0, y);
        printf("                    ");  // 20 spaces to clear line
    }
}

void display_text(uint8_t x, uint8_t y, const char* text) {
    // Simple text display (would need proper font tiles in real implementation)
    gotoxy(x, y);
    printf("%s", text);
}

void update_morse_display(void) {
    // Display current morse input at bottom of screen
    const char* buffer = get_morse_buffer();
    if (buffer[0] != '\0') {
        gotoxy(14, 17);
        printf("%-6s", buffer);
    }
}