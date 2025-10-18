#include <gb/gb.h>
#include <stdint.h>
#include "input.h"

// Input state tracking
static uint8_t previous_keys = 0;
static uint8_t current_keys = 0;

// Timing for morse code input
static uint16_t button_hold_timer = 0;
static uint8_t last_button_pressed = 0;

void init_input(void) {
    previous_keys = 0;
    current_keys = 0;
    button_hold_timer = 0;
    last_button_pressed = 0;
}

void update_input(void) {
    // Store previous state
    previous_keys = current_keys;

    // Read current button state
    current_keys = joypad();

    // Update hold timer if button is being held
    if (current_keys & (J_A | J_B)) {
        button_hold_timer++;

        // Store which button is being held
        if (current_keys & J_A) {
            last_button_pressed = J_A;
        } else if (current_keys & J_B) {
            last_button_pressed = J_B;
        }
    } else {
        // Button released, reset timer
        button_hold_timer = 0;
        last_button_pressed = 0;
    }
}

uint8_t get_keys(void) {
    return current_keys;
}

uint8_t get_key_pressed(void) {
    // Return buttons that were just pressed (not held)
    return (current_keys ^ previous_keys) & current_keys;
}

uint8_t get_key_released(void) {
    // Return buttons that were just released
    return (current_keys ^ previous_keys) & previous_keys;
}

uint8_t is_key_held(uint8_t key) {
    return (current_keys & key) && (previous_keys & key);
}

uint16_t get_button_hold_time(void) {
    return button_hold_timer;
}

uint8_t get_last_button(void) {
    return last_button_pressed;
}

// Helper function to determine if a hold was a dot or dash
morse_signal_t get_morse_signal_from_hold(void) {
    // Morse timing: dot = 1 unit, dash = 3 units
    // Using ~15 frames as one unit (quarter second at 60fps)
    if (button_hold_timer < MORSE_DOT_THRESHOLD) {
        return MORSE_DOT;
    } else {
        return MORSE_DASH;
    }
}