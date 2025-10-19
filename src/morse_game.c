/*
 * Morse Command - Simplified Single-File Version
 * A working Game Boy morse code game
 */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdint.h>
#include <rand.h>

// Game constants
#define STATE_TITLE 0
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2

// Morse patterns for A-Z (keeping it simple)
const char* morse_patterns[26] = {
    ".-",   "...",  ".-..", "--",   "-.",   // A-E
    "..-.","--.", "....", "..",   ".---",  // F-J
    "-.-", ".-..", "--",   "-.",   "---",   // K-O
    ".--.","-.-.", ".-.",  "...",  "-",     // P-T
    "..-", "...-", ".--",  "-..-", "-.--",  // U-Y
    "--.."                                   // Z
};

// Game state
uint8_t game_state = STATE_TITLE;
uint16_t score = 0;
uint8_t city_health = 3;

// Current falling letter
char current_letter = 'A';
uint8_t letter_y = 0;
uint8_t letter_x = 10;  // Middle of screen

// Morse input buffer
char morse_buffer[8] = "";
uint8_t morse_index = 0;

// Frame counter for timing
uint16_t frame_counter = 0;

// Simple beep
void beep(uint8_t tone) {
    NR52_REG = 0x80;  // Enable sound
    NR51_REG = 0xFF;  // Enable all channels
    NR50_REG = 0x77;  // Max volume

    NR10_REG = 0x00;
    NR11_REG = 0x80;
    NR12_REG = 0xF3;
    NR13_REG = 0x00;
    NR14_REG = 0x80 | tone;
}

// Add morse input
void add_morse(char symbol) {
    if (morse_index < 7) {
        morse_buffer[morse_index++] = symbol;
        morse_buffer[morse_index] = '\0';

        // Show updated morse
        gotoxy(0, 16);
        printf("Input: %-7s", morse_buffer);
    }
}

// Clear morse buffer
void clear_morse(void) {
    morse_index = 0;
    morse_buffer[0] = '\0';
    gotoxy(0, 16);
    printf("Input:         ");
}

// Check if morse matches current letter
uint8_t check_morse(void) {
    const char* pattern = morse_patterns[current_letter - 'A'];

    // Simple string comparison
    uint8_t i;
    for (i = 0; pattern[i] != '\0' && morse_buffer[i] != '\0'; i++) {
        if (pattern[i] != morse_buffer[i]) return 0;
    }

    return (pattern[i] == '\0' && morse_buffer[i] == '\0');
}

// Spawn new letter
void spawn_letter(void) {
    current_letter = 'A' + (rand() % 26);
    letter_y = 3;  // Start at line 3
    letter_x = 8 + (rand() % 12);  // Random X position
}

// Draw game screen
void draw_game(void) {
    // Clear old letter position
    gotoxy(0, 3);
    printf("                    ");

    // Draw score and health
    gotoxy(0, 0);
    printf("Score:%04u Health:%d", score, city_health);

    // Draw separator
    gotoxy(0, 1);
    printf("--------------------");

    // Draw falling letter if in bounds
    if (letter_y >= 3 && letter_y < 14) {
        gotoxy(letter_x, letter_y);
        printf("%c", current_letter);
    }

    // Show pattern hint (for learning)
    gotoxy(0, 17);
    printf("%c = %-5s", current_letter, morse_patterns[current_letter - 'A']);
}

// Title screen
void show_title(void) {
    // Clear screen with simple spaces
    for (uint8_t i = 0; i < 18; i++) {
        gotoxy(0, i);
        printf("                    ");
    }

    gotoxy(0, 3);
    printf("   MORSE COMMAND");

    gotoxy(0, 6);
    printf("  Stop the letters");
    gotoxy(0, 7);
    printf("  before they land!");

    gotoxy(0, 10);
    printf("  A = Dit (.))");
    gotoxy(0, 11);
    printf("  B = Dah (-)");
    gotoxy(0, 12);
    printf("  SELECT = Fire!");

    gotoxy(0, 15);
    printf("   PRESS START");
}

// Game over screen
void show_game_over(void) {
    gotoxy(5, 7);
    printf("GAME OVER!");

    gotoxy(3, 9);
    printf("Final: %04u", score);

    gotoxy(3, 12);
    printf("PRESS START");
}

// Main game update
void update_game(void) {
    static uint8_t old_keys = 0;
    uint8_t keys = joypad();
    uint8_t pressed = keys & ~old_keys;  // Get newly pressed keys

    switch(game_state) {
        case STATE_TITLE:
            if (pressed & J_START) {
                game_state = STATE_PLAYING;
                score = 0;
                city_health = 3;
                spawn_letter();
                clear_morse();
                draw_game();
                beep(0x86);
            }
            break;

        case STATE_PLAYING:
            // Handle input
            if (pressed & J_A) {
                add_morse('.');
                beep(0x87);  // High beep for dit
            }
            if (pressed & J_B) {
                add_morse('-');
                beep(0x85);  // Low beep for dah
            }
            if (pressed & J_SELECT) {
                // Check if morse is correct
                if (check_morse()) {
                    score += 10;
                    beep(0x88);  // Success!
                    spawn_letter();
                    clear_morse();
                } else {
                    beep(0x84);  // Error
                }
                draw_game();
            }
            if (pressed & J_START) {
                // Clear morse buffer
                clear_morse();
            }

            // Move letter down every 30 frames (half second)
            if (frame_counter % 30 == 0) {
                // Clear old position
                if (letter_y >= 3 && letter_y < 14) {
                    gotoxy(letter_x, letter_y);
                    printf(" ");
                }

                letter_y++;

                // Check if letter hit ground
                if (letter_y >= 14) {
                    city_health--;
                    beep(0x83);  // Explosion!

                    if (city_health == 0) {
                        game_state = STATE_GAME_OVER;
                        show_game_over();
                    } else {
                        spawn_letter();
                        clear_morse();
                    }
                }

                draw_game();
            }
            break;

        case STATE_GAME_OVER:
            if (pressed & J_START) {
                game_state = STATE_TITLE;
                show_title();
                beep(0x86);
            }
            break;
    }

    old_keys = keys;
    frame_counter++;
}

void main(void) {
    // CRITICAL: Enable display properly!
    SHOW_BKG;      // Enable background layer (where printf draws)
    DISPLAY_ON;    // Turn on the display

    // Initialize random
    initrand(DIV_REG);

    // Show title
    show_title();

    // Main game loop
    while(1) {
        update_game();
        vsync();
    }
}