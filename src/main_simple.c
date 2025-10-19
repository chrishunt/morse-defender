#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

void main(void) {
    // Simple title and input test
    uint8_t game_state = 0;  // 0=title, 1=game

    printf("\n\n   MORSE COMMAND\n\n");
    printf("   PRESS START\n\n");

    while(1) {
        uint8_t keys = joypad();

        // Show debug info
        gotoxy(0, 10);
        printf("Keys:%02X State:%d  ", keys, game_state);

        if (game_state == 0) {
            // Title screen
            if (keys & J_START) {
                game_state = 1;
                gotoxy(0, 12);
                printf("GAME STARTED!  ");

                // Simple beep
                NR52_REG = 0x80;
                NR51_REG = 0xFF;
                NR50_REG = 0x77;
                NR10_REG = 0x00;
                NR11_REG = 0x80;
                NR12_REG = 0xF3;
                NR13_REG = 0x00;
                NR14_REG = 0x86;
            }
        } else {
            // Game screen
            gotoxy(0, 14);
            if (keys & J_A) {
                printf("A: DIT!   ");
            } else if (keys & J_B) {
                printf("B: DAH!   ");
            } else if (keys & J_SELECT) {
                game_state = 0;
                gotoxy(0, 12);
                printf("BACK TO TITLE");
            }
        }

        vsync();
    }
}