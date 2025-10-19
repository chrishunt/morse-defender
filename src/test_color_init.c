/*
 * Test if Game Boy Color mode needs special initialization
 */

#include <gb/gb.h>
#include <gb/cgb.h>  // Color GB specific header
#include <gbdk/console.h>
#include <stdio.h>

void main(void) {
    // Check if we're on CGB
    if (_cpu == CGB_TYPE) {
        printf("\n\n  RUNNING ON GBC\n\n");
    } else {
        printf("\n\n  RUNNING ON GB\n\n");
    }

    printf("  Do you see this?\n");
    printf("  Color mode test\n\n");

    while(1) {
        uint8_t keys = joypad();

        gotoxy(0, 8);
        printf("Keys: %02X", keys);

        vsync();
    }
}