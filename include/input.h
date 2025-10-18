#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

// Morse signal types
typedef enum {
    MORSE_NONE,
    MORSE_DOT,
    MORSE_DASH
} morse_signal_t;

// Timing thresholds (in frames, 60fps)
#define MORSE_DOT_THRESHOLD 20   // Less than 1/3 second is a dot
#define MORSE_DASH_THRESHOLD 60  // More than 1 second for long dash

// Function prototypes
void init_input(void);
void update_input(void);
uint8_t get_keys(void);
uint8_t get_key_pressed(void);
uint8_t get_key_released(void);
uint8_t is_key_held(uint8_t key);
uint16_t get_button_hold_time(void);
uint8_t get_last_button(void);
morse_signal_t get_morse_signal_from_hold(void);

#endif // INPUT_H