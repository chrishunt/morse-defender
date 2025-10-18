#include <gb/gb.h>
#include <stdint.h>
#include <string.h>
#include "morse.h"

// Morse code patterns for letters A-Z
const char* morse_alphabet[26] = {
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
const char* morse_numbers[10] = {
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

// Current morse input buffer
#define MAX_MORSE_LENGTH 6
static char morse_buffer[MAX_MORSE_LENGTH + 1];
static uint8_t morse_buffer_pos = 0;

// Timing variables
static uint16_t input_gap_timer = 0;

void init_morse(void) {
    clear_morse_buffer();
    input_gap_timer = 0;
}

void clear_morse_buffer(void) {
    memset(morse_buffer, 0, sizeof(morse_buffer));
    morse_buffer_pos = 0;
}

void add_morse_input(morse_signal_t signal) {
    if (morse_buffer_pos < MAX_MORSE_LENGTH) {
        if (signal == MORSE_DOT) {
            morse_buffer[morse_buffer_pos++] = '.';
        } else if (signal == MORSE_DASH) {
            morse_buffer[morse_buffer_pos++] = '-';
        }
        morse_buffer[morse_buffer_pos] = '\0';

        // Reset gap timer when new input is added
        input_gap_timer = 0;
    }
}

char decode_morse_buffer(void) {
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

const char* get_morse_pattern(char character) {
    // Convert to uppercase if needed
    if (character >= 'a' && character <= 'z') {
        character = character - 'a' + 'A';
    }

    if (character >= 'A' && character <= 'Z') {
        return morse_alphabet[character - 'A'];
    } else if (character >= '0' && character <= '9') {
        return morse_numbers[character - '0'];
    }

    return NULL;
}

const char* get_current_morse_buffer(void) {
    return morse_buffer;
}

uint8_t get_morse_buffer_length(void) {
    return morse_buffer_pos;
}

void update_morse_timing(void) {
    // Increment gap timer
    input_gap_timer++;

    // If gap is long enough, decode the current buffer
    if (input_gap_timer > MORSE_LETTER_GAP && morse_buffer_pos > 0) {
        // Attempt to decode
        char decoded = decode_morse_buffer();
        if (decoded != 0) {
            // Successfully decoded - would trigger game event here
            clear_morse_buffer();
        }
    }

    // If gap is very long, clear the buffer
    if (input_gap_timer > MORSE_WORD_GAP) {
        clear_morse_buffer();
    }
}