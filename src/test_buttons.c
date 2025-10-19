/*
 * Simple test to verify A and B buttons work consistently
 */

#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include <stdio.h>

void main(void) {
    // Set palette for GBC
    if (_cpu == CGB_TYPE) {
        uint16_t palette[] = {
            RGB(28, 28, 24),  // Background
            RGB(20, 20, 16),  // Mid
            RGB(8, 8, 8),     // Dark
            RGB(0, 0, 0)      // Black (text)
        };
        set_bkg_palette(0, 1, palette);
    }

    printf("\n\n  BUTTON TEST\n\n");
    printf("  A = Dit (.)\n");
    printf("  B = Dah (-)\n\n");

    // Track button presses
    uint8_t a_count = 0;
    uint8_t b_count = 0;
    uint8_t old_keys = 0;
    char buffer[8] = "";
    uint8_t buffer_pos = 0;

    while(1) {
        uint8_t keys = joypad();
        uint8_t pressed = keys & ~old_keys;  // Newly pressed only

        // Show raw state
        gotoxy(0, 8);
        printf("Keys: %02X Press: %02X", keys, pressed);

        // A button test
        if (pressed & J_A) {
            a_count++;
            if (buffer_pos < 7) {
                buffer[buffer_pos++] = '.';
                buffer[buffer_pos] = '\0';
            }

            gotoxy(0, 10);
            printf("A pressed! Count:%d", a_count);

            // Beep for dit
            NR52_REG = 0x80;
            NR51_REG = 0xFF;
            NR50_REG = 0x77;
            NR10_REG = 0x00;
            NR11_REG = 0x80;
            NR12_REG = 0xF3;
            NR13_REG = 0xFA;
            NR14_REG = 0x86;
        }

        // B button test
        if (pressed & J_B) {
            b_count++;
            if (buffer_pos < 7) {
                buffer[buffer_pos++] = '-';
                buffer[buffer_pos] = '\0';
            }

            gotoxy(0, 11);
            printf("B pressed! Count:%d", b_count);

            // Beep for dah
            NR52_REG = 0x80;
            NR51_REG = 0xFF;
            NR50_REG = 0x77;
            NR10_REG = 0x00;
            NR11_REG = 0x80;
            NR12_REG = 0xF5;
            NR13_REG = 0xFA;
            NR14_REG = 0x86;
        }

        // Show buffer
        gotoxy(0, 13);
        printf("Buffer: %-7s", buffer);

        // SELECT to clear
        if (pressed & J_SELECT) {
            buffer_pos = 0;
            buffer[0] = '\0';
            a_count = 0;
            b_count = 0;

            gotoxy(0, 15);
            printf("CLEARED!");
        }

        old_keys = keys;
        vsync();
    }
}