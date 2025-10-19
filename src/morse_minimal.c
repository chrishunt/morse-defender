/*
 * Minimal Morse Command Game
 * All-in-one version without problematic modules
 */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdint.h>
#include <rand.h>

// Game states
#define STATE_TITLE 0
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2

// Morse code patterns (simplified)
const char* morse_patterns[] = {
    ".-",    // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".",     // E
    "..-.",  // F
    "--.",   // G
    "....",  // H
    "..",    // I
    ".---",  // J
    "-.-",   // K
    ".-..",  // L
    "--",    // M
    "-.",    // N
    "---",   // O
    ".--.",  // P
    "--.-",  // Q
    ".-.",   // R
    "...",   // S
    "-",     // T
    "..-",   // U
    "...-",  // V
    ".--",   // W
    "-..-",  // X
    "-.--",  // Y
    "--.."   // Z
};

// Game variables
uint8_t game_state = STATE_TITLE;
uint16_t score = 0;
uint8_t current_letter = 0;  // 0-25 for A-Z
char morse_buffer[8] = "";
uint8_t morse_index = 0;
uint16_t frame_counter = 0;
uint8_t missile_y = 0;
uint8_t city_health = 3;

// Play a beep sound
void play_beep(uint8_t pitch) {
    NR52_REG = 0x80;  // Enable sound
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    NR10_REG = 0x00;
    NR11_REG = 0x80;
    NR12_REG = 0xF3;
    NR13_REG = 0x00;
    NR14_REG = 0x80 | pitch;  // Vary pitch
}

// Add to morse buffer
void add_morse_input(char symbol) {
    if (morse_index < 7) {
        morse_buffer[morse_index++] = symbol;
        morse_buffer[morse_index] = '\0';
    }
}

// Check if morse matches current letter
uint8_t check_morse_match(void) {
    // Simple comparison with the pattern for current letter
    const char* pattern = morse_patterns[current_letter];

    // Compare strings
    uint8_t i = 0;
    while (pattern[i] != '\0' && morse_buffer[i] != '\0') {
        if (pattern[i] != morse_buffer[i]) return 0;
        i++;
    }

    // Check both strings ended
    return (pattern[i] == '\0' && morse_buffer[i] == '\0');
}

// Reset morse buffer
void reset_morse(void) {
    morse_index = 0;
    morse_buffer[0] = '\0';
}

// Generate new random letter
void spawn_new_letter(void) {
    current_letter = rand() % 26;  // Random letter A-Z
    missile_y = 0;
}

// Draw the game screen
void draw_game_screen(void) {
    // Clear screen
    gotoxy(0, 0);

    // Score line
    printf("Score: %05u  City: %d\n", score, city_health);
    printf("--------------------\n");

    // Show falling letter (missile)
    if (game_state == STATE_PLAYING && missile_y < 14) {
        gotoxy(9, 3 + missile_y);
        printf("%c", 'A' + current_letter);
    }

    // Show morse input area
    gotoxy(0, 16);
    printf("Morse: %-7s", morse_buffer);

    // Show current letter's pattern (for learning)
    gotoxy(0, 17);
    printf("%c=%s     ", 'A' + current_letter, morse_patterns[current_letter]);
}

// Update game
void update_game(void) {
    uint8_t keys = joypad();
    static uint8_t old_keys = 0;
    uint8_t pressed = keys & ~old_keys;  // Newly pressed keys

    if (game_state == STATE_TITLE) {
        // Title screen
        if (pressed & J_START) {
            game_state = STATE_PLAYING;
            score = 0;
            city_health = 3;
            spawn_new_letter();
            play_beep(0x86);
            draw_game_screen();
        }
    }
    else if (game_state == STATE_PLAYING) {
        // Game playing

        // Handle morse input (on button press, not hold)
        if (pressed & J_A) {
            add_morse_input('.');
            play_beep(0x86);  // Dit
        }
        if (pressed & J_B) {
            add_morse_input('-');
            play_beep(0x85);  // Dah
        }

        // Check for match on SELECT
        if (pressed & J_SELECT) {
            if (check_morse_match()) {
                // Correct!
                score += 10;
                play_beep(0x87);  // Success sound
                spawn_new_letter();
                reset_morse();
            } else {
                // Wrong
                play_beep(0x84);  // Error sound
            }
            draw_game_screen();
        }

        // Clear morse on START
        if (pressed & J_START) {
            reset_morse();
            draw_game_screen();
        }

        // Move missile down every 30 frames
        if (frame_counter % 30 == 0) {
            missile_y++;

            // Check if missile hit ground
            if (missile_y >= 14) {
                city_health--;
                play_beep(0x83);  // Explosion

                if (city_health == 0) {
                    // Game over
                    game_state = STATE_GAME_OVER;
                    gotoxy(5, 7);
                    printf("GAME OVER!");
                    gotoxy(3, 9);
                    printf("Score: %05u", score);
                    gotoxy(2, 12);
                    printf("PRESS START");
                } else {
                    // Spawn new missile
                    spawn_new_letter();
                    reset_morse();
                }
            }

            draw_game_screen();
        }
    }
    else if (game_state == STATE_GAME_OVER) {
        // Game over screen
        if (pressed & J_START) {
            // Back to title
            game_state = STATE_TITLE;
            gotoxy(0, 0);
            printf("\n\n  MORSE COMMAND\n\n");
            printf("  DEFEND YOUR CITY!\n\n");
            printf("  A=DIT B=DAH\n");
            printf("  SELECT=FIRE\n");
            printf("  START=CLEAR/BEGIN\n\n");
            printf("  PRESS START!\n");
        }
    }

    old_keys = keys;
    frame_counter++;
}

void main(void) {
    // Initialize random
    initrand(DIV_REG);

    // Show title screen
    printf("\n\n  MORSE COMMAND\n\n");
    printf("  DEFEND YOUR CITY!\n\n");
    printf("  A=DIT B=DAH\n");
    printf("  SELECT=FIRE\n");
    printf("  START=CLEAR/BEGIN\n\n");
    printf("  PRESS START!\n");

    // Main loop
    while(1) {
        update_game();
        vsync();
    }
}