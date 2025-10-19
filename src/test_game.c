#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

void main(void) {
    // Exactly like test_input.c but with game logic
    uint8_t game_state = 0;  // 0=title, 1=game

    printf("\n\n  MORSE COMMAND\n\n");
    printf("  DEFEND CITIES!\n\n");
    printf("  A: DIT   B: DAH\n\n");
    printf("   PRESS START\n");

    while(1) {
        uint8_t keys = joypad();

        // Show debug info
        gotoxy(0, 15);
        printf("Keys:%02X State:%d  ", keys, game_state);

        if (game_state == 0) {
            // Title screen - check for START
            if (keys & J_START) {
                game_state = 1;
                gotoxy(0, 12);
                printf("GAME STARTED!     ");

                // Make a beep
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
            gotoxy(0, 13);
            if (keys & J_A) {
                printf("A: DIT!      ");
                // Dit sound
                NR52_REG = 0x80;
                NR51_REG = 0xFF;
                NR50_REG = 0x77;
                NR10_REG = 0x00;
                NR11_REG = 0x80;
                NR12_REG = 0xF3;
                NR13_REG = 0x00;
                NR14_REG = 0x86;
            } else if (keys & J_B) {
                printf("B: DAH!      ");
                // Dah sound
                NR52_REG = 0x80;
                NR51_REG = 0xFF;
                NR50_REG = 0x77;
                NR10_REG = 0x00;
                NR11_REG = 0x80;
                NR12_REG = 0xF5;
                NR13_REG = 0x00;
                NR14_REG = 0x85;
            } else if (keys & J_SELECT) {
                game_state = 0;
                gotoxy(0, 12);
                printf("BACK TO TITLE     ");
            } else {
                printf("              ");
            }
        }

        vsync();
    }
}