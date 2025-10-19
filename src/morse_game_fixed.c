/*
 * Morse Command - Fixed version with proper CGB palette initialization
 */

#include <gb/gb.h>
#include <gb/cgb.h>  // Include CGB header for palette functions
#include <gbdk/console.h>
#include <stdio.h>
#include <stdint.h>
#include <rand.h>

// Game constants
#define STATE_TITLE 0
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2

// Morse patterns for A-Z (corrected)
const char* morse_patterns[26] = {
    ".-",   "-...", "-.-.", "-..",  ".",    // A-E
    "..-.", "--.",  "....", "..",   ".---", // F-J
    "-.-",  ".-..", "--",   "-.",   "---",  // K-O
    ".--.", "--.-", ".-.",  "...",  "-",    // P-T
    "..-",  "...-", ".--",  "-..-", "-.--", // U-Y
    "--.."                                   // Z
};

// Game state
uint8_t game_state = STATE_TITLE;
uint16_t score = 0;
uint8_t city_health = 3;

// Current falling letter
char current_letter = 'A';
uint8_t letter_y = 0;
uint8_t letter_x = 10;

// Morse input buffer
char morse_buffer[8] = "";
uint8_t morse_index = 0;

// Frame counter for timing
uint16_t frame_counter = 0;

// Beep with duration for proper morse code
// Faster timing: dit = 8 frames (125ms), dah = 23 frames (375ms) - 3x ratio maintained
static uint8_t beep_timer = 0;

void start_beep(void) {
    // Same 500Hz tone for both dit and dah - proper morse!
    NR52_REG = 0x80;  // Enable sound
    NR51_REG = 0xFF;  // All channels
    NR50_REG = 0x77;  // Max volume

    NR10_REG = 0x00;  // No sweep
    NR11_REG = 0x80;  // 50% duty cycle, no length
    NR12_REG = 0xF0;  // Max volume, no envelope (continuous tone)

    // Frequency calculation for ~500Hz:
    // GB frequency = 131072/(2048-x) Hz
    // For 500Hz: x = 2048 - 262 = 1786 (0x6FA)
    NR13_REG = 0xFA;  // Low 8 bits of frequency
    NR14_REG = 0x86;  // High 3 bits + trigger (0x80 | 0x06)
}

void stop_beep(void) {
    // Properly stop the sound
    NR12_REG = 0x00;  // Set volume to 0
    NR14_REG = 0x00;  // Stop the channel
}

void update_beep(void) {
    if (beep_timer > 0) {
        beep_timer--;
        if (beep_timer == 0) {
            stop_beep();
        }
    }
}

// Quick beep for UI feedback (not morse)
void beep_quick(uint8_t pitch) {
    NR52_REG = 0x80;  // Enable sound
    NR51_REG = 0xFF;  // All channels
    NR50_REG = 0x77;  // Max volume

    NR10_REG = 0x00;
    NR11_REG = 0x80;  // 50% duty
    NR12_REG = 0xF3;  // Quick envelope decay

    // Different pitches for feedback:
    // 0x84 = low (error), 0x86 = medium, 0x88 = high (success)
    if (pitch == 0x88) {
        // Success - higher pitch (~700Hz)
        NR13_REG = 0x73;
        NR14_REG = 0x87;
    } else if (pitch == 0x84) {
        // Error - lower pitch (~350Hz)
        NR13_REG = 0x2C;
        NR14_REG = 0x87;
    } else {
        // Default - medium pitch
        NR13_REG = 0xFA;
        NR14_REG = 0x86;
    }
}

// Add morse input
void add_morse(char symbol) {
    if (morse_index < 7) {
        morse_buffer[morse_index++] = symbol;
        morse_buffer[morse_index] = '\0';

        // Show input more clearly with visual indication
        gotoxy(0, 15);
        printf("Your input: %-7s", morse_buffer);

        // Debug: Show what was just added
        gotoxy(0, 14);
        if (symbol == '.') {
            printf("Added: DIT    ");
        } else {
            printf("Added: DAH    ");
        }
    }
}

// Clear morse buffer
void clear_morse(void) {
    morse_index = 0;
    morse_buffer[0] = '\0';
    gotoxy(0, 14);
    printf("              ");  // Clear "Added:" line
    gotoxy(0, 15);
    printf("Your input:        ");
}

// Check morse status - simplified and clearer
uint8_t check_morse_status(void) {
    const char* pattern = morse_patterns[current_letter - 'A'];

    // Empty buffer
    if (morse_index == 0) return 0;

    // Check each character
    for (uint8_t i = 0; i < morse_index; i++) {
        if (!pattern[i]) {
            // Pattern ended but we have more input - too long
            return 2;
        }
        if (morse_buffer[i] != pattern[i]) {
            // Mismatch
            return 2;
        }
    }

    // All input matches so far
    if (!pattern[morse_index]) {
        // Pattern complete!
        return 1;
    }

    // Partial match, need more input
    return 0;
}

// Spawn new letter
void spawn_letter(void) {
    current_letter = 'A' + (rand() % 26);
    letter_y = 3;
    letter_x = 8 + (rand() % 12);
}

// Draw game screen
void draw_game(void) {
    // Clear old letter position (entire row)
    if (letter_y > 3 && letter_y < 15) {
        gotoxy(0, letter_y - 1);
        printf("                    ");
    }

    // Clear feedback message area only if there was a message
    static uint8_t had_feedback = 0;
    if (had_feedback) {
        gotoxy(8, 8);
        printf("        ");
        had_feedback = 0;
    }

    // Draw score and health
    gotoxy(0, 0);
    printf("Score:%04u Health:%d", score, city_health);

    // Draw separator
    gotoxy(0, 1);
    printf("--------------------");

    // Draw falling letter
    if (letter_y >= 3 && letter_y < 14) {
        gotoxy(letter_x, letter_y);
        printf("%c", current_letter);
    }

    // Show pattern hint clearly
    gotoxy(0, 16);
    printf("Need: %c = %-5s", current_letter, morse_patterns[current_letter - 'A']);
    gotoxy(0, 17);
    printf("[Auto-recognizes!]");
}

// Title screen
void show_title(void) {
    // Don't clear with spaces - just overwrite
    gotoxy(0, 3);
    printf("   MORSE COMMAND");

    gotoxy(0, 6);
    printf("  Stop the letters");
    gotoxy(0, 7);
    printf("  before they land!");

    gotoxy(0, 10);
    printf("  A = Dit (.)");
    gotoxy(0, 11);
    printf("  B = Dah (-)");
    gotoxy(0, 12);
    printf("  SELECT = Clear");

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
    uint8_t pressed = keys & ~old_keys;

    switch(game_state) {
        case STATE_TITLE:
            if (pressed & J_START) {
                game_state = STATE_PLAYING;
                score = 0;
                city_health = 3;
                spawn_letter();
                clear_morse();
                draw_game();
                beep_quick(0x86);  // Start game sound
            }
            break;

        case STATE_PLAYING:
            // Handle input
            if (pressed & J_A) {
                // A button = Dit (.)
                if (beep_timer > 0) stop_beep();
                add_morse('.');
                start_beep();
                beep_timer = 8;  // Dit: 125ms

                // Check for auto-recognition
                uint8_t status = check_morse_status();
                if (status == 1) {
                    // Complete match - auto succeed!
                    score += 10;
                    beep_quick(0x88);
                    gotoxy(8, 8);
                    printf("CORRECT!");
                    spawn_letter();
                    clear_morse();
                    draw_game();
                }
                // Don't auto-clear on wrong - let player use SELECT to clear
            }
            if (pressed & J_B) {
                // B button = Dah (-)
                if (beep_timer > 0) stop_beep();
                add_morse('-');
                start_beep();
                beep_timer = 23;  // Dah: 375ms (3x dit)

                // Check for auto-recognition
                uint8_t status = check_morse_status();
                if (status == 1) {
                    // Complete match - auto succeed!
                    score += 10;
                    beep_quick(0x88);
                    gotoxy(8, 8);
                    printf("CORRECT!");
                    spawn_letter();
                    clear_morse();
                    draw_game();
                }
                // Don't auto-clear on wrong - let player use SELECT to clear
            }
            if (pressed & J_SELECT) {
                // SELECT now clears input to start over
                clear_morse();
                gotoxy(8, 8);
                printf("CLEARED ");
            }

            // Move letter down
            if (frame_counter % 30 == 0) {
                if (letter_y >= 3 && letter_y < 14) {
                    gotoxy(letter_x, letter_y);
                    printf(" ");
                }

                letter_y++;

                if (letter_y >= 14) {
                    city_health--;
                    beep_quick(0x83);  // Explosion sound

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
                beep_quick(0x86);
            }
            break;
    }

    // Update morse beep timer
    update_beep();

    old_keys = keys;
    frame_counter++;
}

void main(void) {
    // CRITICAL FIX: Set palette for Game Boy Color
    // Without this, text is white on white!
    if (_cpu == CGB_TYPE) {
        // Classic Game Boy palette: greenish background, black text
        uint16_t palette[] = {
            RGB(28, 28, 24),  // Background: Light green (like original GB)
            RGB(20, 20, 16),  // Color 1: Medium
            RGB(8, 8, 8),     // Color 2: Dark gray
            RGB(0, 0, 0)      // Color 3: Black (text uses this)
        };
        set_bkg_palette(0, 1, palette);
    }

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