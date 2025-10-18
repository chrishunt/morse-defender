#ifndef MORSE_H
#define MORSE_H

#include <stdint.h>
#include "input.h"  // For morse_signal_t

// Morse timing constants (in frames at 60fps)
#define MORSE_UNIT_TIME 15        // Base unit (1/4 second)
#define MORSE_LETTER_GAP 45       // Gap between letters (3 units)
#define MORSE_WORD_GAP 105        // Gap between words (7 units)

// Function prototypes
void init_morse(void);
void clear_morse_buffer(void);
void add_morse_input(morse_signal_t signal);
char decode_morse_buffer(void);
const char* get_morse_pattern(char character);
const char* get_current_morse_buffer(void);
uint8_t get_morse_buffer_length(void);
void update_morse_timing(void);

#endif // MORSE_H