/*
 * Test 4: Add const array like morse_game.c has
 */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

// Add the same type of const array as morse_game.c
const char* morse_patterns[26] = {
    ".-",   "-...", "-.-.", "-..",  ".",
    "..-.", "--.",  "....", "..",   ".---",
    "-.-",  ".-..", "--",   "-.",   "---",
    ".--.", "--.-", ".-.",  "...",  "-",
    "..-",  "...-", ".--",  "-..-", "-.--",
    "--.."
};

void main(void) {
    printf("\n\n  TEST WITH ARRAY\n\n");
    printf("  Testing const\n");
    printf("  array: %s\n\n", morse_patterns[0]);  // Show ".-" for A

    while(1) {
        uint8_t keys = joypad();

        gotoxy(0, 8);
        printf("Keys: %02X", keys);

        vsync();
    }
}