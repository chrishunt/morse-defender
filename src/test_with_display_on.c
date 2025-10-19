/*
 * Test 2: Add SHOW_BKG and DISPLAY_ON
 */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

void main(void) {
    // ADD: Display initialization that morse_game.c has
    SHOW_BKG;
    DISPLAY_ON;

    printf("\n\n  TEST WITH DISPLAY\n\n");
    printf("  Testing SHOW_BKG\n");
    printf("  and DISPLAY_ON\n\n");

    while(1) {
        uint8_t keys = joypad();

        gotoxy(0, 8);
        printf("Keys: %02X", keys);

        vsync();
    }
}