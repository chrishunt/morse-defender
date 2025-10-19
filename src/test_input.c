#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

void main(void) {
    // Simple input test program
    printf("\n\n  BUTTON TEST\n\n");
    printf("  Press any button\n\n");

    while(1) {
        uint8_t keys = joypad();

        // Show raw key value
        gotoxy(0, 8);
        printf("Raw Keys: %02X  ", keys);

        // Test each button individually
        gotoxy(0, 10);
        if (keys & J_A)      printf("A PRESSED    ");
        else if (keys & J_B) printf("B PRESSED    ");
        else if (keys & J_SELECT) printf("SELECT PRESSED");
        else if (keys & J_START) printf("START PRESSED ");
        else if (keys & J_RIGHT) printf("RIGHT PRESSED ");
        else if (keys & J_LEFT) printf("LEFT PRESSED  ");
        else if (keys & J_UP) printf("UP PRESSED    ");
        else if (keys & J_DOWN) printf("DOWN PRESSED  ");
        else printf("              ");

        // Also try waitpad for any button
        gotoxy(0, 12);
        printf("Waiting for button...");

        // Don't block, just check
        if (keys != 0) {
            gotoxy(0, 14);
            printf("DETECTED: %02X", keys);

            // Make a sound
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