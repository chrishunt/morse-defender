#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "morse_input.h"
#include "audio.h"

// Morse code patterns for letters A-Z
static const char* morse_alphabet[26] = {
    ".-",     // A
    "-...",   // B
    "-.-.",   // C
    "-..",    // D
    ".",      // E
    "..-.",   // F
    "--.",    // G
    "....",   // H
    "..",     // I
    ".---",   // J
    "-.-",    // K
    ".-..",   // L
    "--",     // M
    "-.",     // N
    "---",    // O
    ".--.",   // P
    "--.-",   // Q
    ".-.",    // R
    "...",    // S
    "-",      // T
    "..-",    // U
    "...-",   // V
    ".--",    // W
    "-..-",   // X
    "-.--",   // Y
    "--.."    // Z
};

// Morse code patterns for numbers 0-9
static const char* morse_numbers[10] = {
    "-----",  // 0
    ".----",  // 1
    "..---",  // 2
    "...--",  // 3
    "....-",  // 4
    ".....",  // 5
    "-....",  // 6
    "--...",  // 7
    "---..",  // 8
    "----."   // 9
};

// Current morse input state
static char morse_buffer[MAX_MORSE_LENGTH + 1];
static uint8_t morse_buffer_pos = 0;
static uint8_t morse_complete_flag = 0;
static char last_decoded_letter = 0;

// Timing variables for input detection
static uint8_t button_held_timer = 0;
static uint8_t gap_timer = 0;
static uint8_t last_input_state = 0;

// Initialize morse input system
void init_morse_input(void) {
    clear_morse_buffer();
    morse_complete_flag = 0;
    last_decoded_letter = 0;
    button_held_timer = 0;
    gap_timer = 0;
    last_input_state = 0;
}

// Clear the morse buffer
void clear_morse_buffer(void) {
    memset(morse_buffer, 0, sizeof(morse_buffer));
    morse_buffer_pos = 0;
    morse_complete_flag = 0;
}

// Update morse input system (call every frame)
void update_morse_input(void) {
    uint8_t current_input = joypad();
    uint8_t dit_pressed = current_input & J_A;
    uint8_t dah_pressed = current_input & J_B;

    // Handle button press/release for morse input
    if (dit_pressed || dah_pressed) {
        // Button is being pressed
        if (!last_input_state) {
            // Just pressed - play sound once
            if (dit_pressed) {
                play_dot_beep();
                add_morse_signal(MORSE_DOT);
            } else if (dah_pressed) {
                play_dash_beep();
                add_morse_signal(MORSE_DASH);
            }
        }
        last_input_state = 1;
        gap_timer = 0;  // Reset gap timer while button held
    } else {
        // No button pressed
        last_input_state = 0;

        // Check for letter completion after gap
        if (morse_buffer_pos > 0) {
            gap_timer++;

            if (gap_timer >= LETTER_GAP_TIME) {
                // Enough time has passed - decode the buffer
                char decoded = decode_morse();
                if (decoded != 0) {
                    last_decoded_letter = decoded;
                    morse_complete_flag = 1;
                }
                // Clear buffer whether we decoded or not
                clear_morse_buffer();
                gap_timer = 0;
            }
        }
    }
}

// Add a morse signal to the buffer
void add_morse_signal(MorseSignal signal) {
    if (morse_buffer_pos < MAX_MORSE_LENGTH) {
        if (signal == MORSE_DOT) {
            morse_buffer[morse_buffer_pos++] = '.';
        } else if (signal == MORSE_DASH) {
            morse_buffer[morse_buffer_pos++] = '-';
        }
        morse_buffer[morse_buffer_pos] = '\0';
    }
}

// Decode current morse buffer
char decode_morse(void) {
    // Check alphabet first
    for (uint8_t i = 0; i < 26; i++) {
        if (strcmp(morse_buffer, morse_alphabet[i]) == 0) {
            return 'A' + i;
        }
    }

    // Check numbers
    for (uint8_t i = 0; i < 10; i++) {
        if (strcmp(morse_buffer, morse_numbers[i]) == 0) {
            return '0' + i;
        }
    }

    // No match found
    return 0;
}

// Check if a morse letter is ready
uint8_t is_morse_complete(void) {
    return morse_complete_flag;
}

// Get the last decoded letter
char get_morse_letter(void) {
    morse_complete_flag = 0;  // Clear flag when read
    return last_decoded_letter;
}

// Get current morse buffer (for display)
const char* get_morse_buffer(void) {
    return morse_buffer;
}

// Get morse pattern for a character (for reference display)
const char* get_morse_for_char(char c) {
    if (c >= 'A' && c <= 'Z') {
        return morse_alphabet[c - 'A'];
    } else if (c >= 'a' && c <= 'z') {
        return morse_alphabet[c - 'a'];
    } else if (c >= '0' && c <= '9') {
        return morse_numbers[c - '0'];
    }
    return NULL;
}

// Display current morse input on screen
void display_morse_input(uint8_t x, uint8_t y) {
    // Show current morse buffer at specified position
    gotoxy(x, y);
    printf("%-6s", morse_buffer);  // Left-aligned, 6 chars wide
}

// Display morse reference for a character
void display_morse_reference(char c, uint8_t x, uint8_t y) {
    const char* pattern = get_morse_for_char(c);
    if (pattern) {
        gotoxy(x, y);
        printf("%c:%-5s", c, pattern);
    }
}