/*
 * Test 3: Add screen clearing like morse_game.c does
 */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

void clear_screen(void) {
    // This is what morse_game.c does in show_title()
    for (uint8_t i = 0; i < 18; i++) {
        gotoxy(0, i);
        printf("                    ");
    }
}

void main(void) {
    // First clear the screen like morse_game.c
    clear_screen();

    printf("\n\n  TEST WITH CLEAR\n\n");
    printf("  Testing clear\n");
    printf("  screen function\n\n");

    while(1) {
        uint8_t keys = joypad();

        gotoxy(0, 8);
        printf("Keys: %02X", keys);

        vsync();
    }
}