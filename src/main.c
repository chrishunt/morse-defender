#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>
#include <stdio.h>
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include "morse.h"

// Game states
typedef enum {
    STATE_TITLE,
    STATE_GAME,
    STATE_MORSE_INPUT
} game_state_t;

static game_state_t current_state = STATE_TITLE;

void init(void) {
    // Wait for vblank before turning off display
    wait_vbl_done();

    // Turn off display while initializing
    DISPLAY_OFF;

    // Enable Game Boy Color features if available
    if (_cpu == CGB_TYPE) {
        // Set to double speed mode on GBC
        cpu_fast();
    }

    // Initialize subsystems
    init_graphics();
    init_audio();
    init_input();
    init_morse();

    // Turn on display with backgrounds
    DISPLAY_ON;
    SHOW_BKG;
}

void update_title_state(void) {
    // Check for button press to start game
    if (get_key_pressed() & (J_START | J_A)) {
        current_state = STATE_GAME;
        // Clear screen and prepare game
        clear_screen();
    }
}

void update_game_state(void) {
    // Main game logic
    // Handle morse code input
    uint8_t keys = get_key_pressed();

    if (keys & J_A) {
        // Dot pressed
        play_dot_beep();
        add_morse_input(MORSE_DOT);
    } else if (keys & J_B) {
        // Dash pressed
        play_dash_beep();
        add_morse_input(MORSE_DASH);
    }

    // Update morse display
    update_morse_display();
}

void main(void) {
    init();

    // Display title screen
    display_title();

    // Main game loop
    while(1) {
        // Update input state
        update_input();

        // Update based on current game state
        switch(current_state) {
            case STATE_TITLE:
                update_title_state();
                break;

            case STATE_GAME:
                update_game_state();
                break;

            case STATE_MORSE_INPUT:
                // Future: Handle morse code training/recognition
                break;
        }

        // Wait for vertical blank
        vsync();
    }
}