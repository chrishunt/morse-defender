#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdint.h>

void main(void) {
    // Absolutely minimal - no other includes or dependencies
    printf("\n\n  MORSE COMMAND\n\n");
    printf("  DEFEND CITIES!\n\n");
    printf("  A: DIT   B: DAH\n\n");
    printf("   PRESS START\n");

    uint8_t game_state = 0;

    while(1) {
        uint8_t keys = joypad();

        gotoxy(0, 15);
        printf("Keys:%02X State:%d  ", keys, game_state);

        if (game_state == 0 && (keys & J_START)) {
            game_state = 1;
            gotoxy(0, 12);
            printf("GAME STARTED!     ");

            // Beep
            NR52_REG = 0x80;
            NR51_REG = 0xFF;
            NR50_REG = 0x77;
            NR10_REG = 0x00;
            NR11_REG = 0x80;
            NR12_REG = 0xF3;
            NR13_REG = 0x00;
            NR14_REG = 0x86;
        }

        vsync();
    }
}