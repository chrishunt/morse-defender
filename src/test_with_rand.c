/*
 * Test 5: Add rand initialization
 */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <rand.h>

void main(void) {
    // Add random init like morse_game.c
    initrand(DIV_REG);

    printf("\n\n  TEST WITH RAND\n\n");
    printf("  Testing initrand\n");
    printf("  Random: %d\n\n", rand() % 10);

    while(1) {
        uint8_t keys = joypad();

        gotoxy(0, 8);
        printf("Keys: %02X", keys);

        vsync();
    }
}