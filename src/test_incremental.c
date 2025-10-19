/*
 * Incremental test to find what breaks the display
 * Starting from working test_input.c and adding morse_game.c features one by one
 */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

// Test 1: Exact copy of working test_input.c structure
void main(void) {
    // Just like test_input.c - printf immediately with no init
    printf("\n\n  TEST DISPLAY\n\n");
    printf("  If you see this,\n");
    printf("  display works!\n\n");

    while(1) {
        uint8_t keys = joypad();

        gotoxy(0, 8);
        printf("Keys: %02X", keys);

        if (keys & J_A) {
            gotoxy(0, 10);
            printf("A PRESSED");
        } else {
            gotoxy(0, 10);
            printf("         ");
        }

        vsync();
    }
}