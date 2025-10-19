#ifndef MORSE_INPUT_H
#define MORSE_INPUT_H

#include <stdint.h>

// Morse signal types
typedef enum {
    MORSE_NONE,
    MORSE_DOT,
    MORSE_DASH
} MorseSignal;

// Maximum morse code length (5 for numbers, but we'll allow 6)
#define MAX_MORSE_LENGTH 6

// Timing constants (in frames at 60fps)
#define DOT_TIME 1        // Single frame for button press
#define DASH_TIME 1       // Single frame for button press (differentiated by button)
#define LETTER_GAP_TIME 30  // Half second to complete a letter

// Function prototypes
void init_morse_input(void);
void clear_morse_buffer(void);
void update_morse_input(void);
void add_morse_signal(MorseSignal signal);
char decode_morse(void);
uint8_t is_morse_complete(void);
char get_morse_letter(void);
const char* get_morse_buffer(void);
const char* get_morse_for_char(char c);
void display_morse_input(uint8_t x, uint8_t y);
void display_morse_reference(char c, uint8_t x, uint8_t y);

#endif // MORSE_INPUT_H