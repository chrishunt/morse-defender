/*
 * Test 6: Everything combined like morse_game.c
 */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdint.h>
#include <rand.h>

const char* morse_patterns[26] = {
    ".-",   "-...", "-.-.", "-..",  ".",
    "..-.", "--.",  "....", "..",   ".---",
    "-.-",  ".-..", "--",   "-.",   "---",
    ".--.", "--.-", ".-.",  "...",  "-",
    "..-",  "...-", ".--",  "-..-", "-.--",
    "--.."
};

void clear_screen(void) {
    for (uint8_t i = 0; i < 18; i++) {
        gotoxy(0, i);
        printf("                    ");
    }
}

void main(void) {
    // Everything morse_game.c does at startup
    SHOW_BKG;
    DISPLAY_ON;
    initrand(DIV_REG);
    clear_screen();

    printf("\n\n  TEST WITH ALL\n\n");
    printf("  If broken, one\n");
    printf("  of these caused it\n\n");

    while(1) {
        uint8_t keys = joypad();

        gotoxy(0, 8);
        printf("Keys: %02X", keys);

        vsync();
    }
}