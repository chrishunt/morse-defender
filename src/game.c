#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdint.h>
#include <stdio.h>
#include "game.h"
#include "font_tiles.h"
#include "missiles.h"
#include "cities.h"
#include "defense.h"
#include "morse_input.h"
#include "audio.h"
#include "graphics.h"

// Game state variables
static GameState current_state = STATE_TITLE;
static uint16_t score = 0;
static uint16_t high_score = 0;
static uint8_t wave_missiles_remaining = 0;
static uint16_t frame_counter = 0;

// Initialize the game
void init_game(void) {
    current_state = STATE_TITLE;
    score = 0;
    wave_missiles_remaining = 0;
    frame_counter = 0;

    // DON'T initialize subsystems yet - they break the display
    // init_missiles();
    // init_cities();
    // init_defense();
}

// Start a new game
void start_new_game(void) {
    score = 0;
    wave_missiles_remaining = MISSILES_PER_WAVE;
    current_state = STATE_PLAYING;

    // DON'T reset game systems yet - they break the display
    // init_missiles();
    // init_cities();
    // init_defense();

    // Clear screen and draw game area
    setup_game_screen();
}

// Update game state
void update_game(void) {
    frame_counter++;

    switch (current_state) {
        case STATE_TITLE:
            update_title_screen();
            break;

        case STATE_PLAYING:
            update_gameplay();
            break;

        case STATE_WAVE_COMPLETE:
            update_wave_complete();
            break;

        case STATE_GAME_OVER:
            update_game_over();
            break;
    }
}

// Update title screen
void update_title_screen(void) {
    // Check for button presses
    uint8_t keys = joypad();

    // Debug: Show button state at bottom of screen
    gotoxy(0, 17);
    printf("Keys: %02X State:%d", keys, current_state);

    // Simple START button check
    if (keys & J_START) {
        // Beep and start game
        play_dot_beep();

        gotoxy(0, 15);
        printf("STARTING!");

        // Change state to playing
        current_state = STATE_PLAYING;
        setup_game_screen();
    }

    // Test A button
    if (keys & J_A) {
        play_dot_beep();
        gotoxy(0, 16);
        printf("A WORKS!");
    }

    // Test B button
    if (keys & J_B) {
        play_dash_beep();
        gotoxy(0, 16);
        printf("B WORKS!");
    }
}

// Main gameplay update
void update_gameplay(void) {
    // Simple button test for now
    uint8_t keys = joypad();

    if (keys & J_A) {
        gotoxy(0, 6);
        printf("A pressed - DIT!  ");
        play_dot_beep();
    } else if (keys & J_B) {
        gotoxy(0, 6);
        printf("B pressed - DAH!  ");
        play_dash_beep();
    } else if (keys & J_SELECT) {
        // Return to title
        current_state = STATE_TITLE;
        show_title_screen();
        return;
    }

    // Don't update game systems until display is working
    // update_missiles();
    // update_defense_missiles();

    // Commented out missile/city logic until display is fixed
    /*
    // Check for missile impacts on cities
    for (uint8_t i = 0; i < MAX_MISSILES; i++) {
        Missile* m = get_missile(i);
        if (m && m->active && m->y >= GROUND_LEVEL) {
            // Missile hit ground - damage nearest city
            uint8_t city_id = find_nearest_city(m->x);
            damage_city(city_id);
            m->active = 0;

            // Play explosion sound
            play_explosion_sound();
        }
    }

    // Check game over condition
    if (all_cities_destroyed()) {
        current_state = STATE_GAME_OVER;
        if (score > high_score) {
            high_score = score;
        }
    }
    */

    // Check wave complete (simplified - based on timer for now)
    if (frame_counter % 3600 == 0) {  // Every minute
        current_state = STATE_WAVE_COMPLETE;
    }

    // Update display
    update_game_display();
}

// Update wave complete screen
void update_wave_complete(void) {
    static uint16_t wave_timer = 0;

    wave_timer++;

    // Show wave complete message for 3 seconds
    if (wave_timer >= 180) {
        wave_timer = 0;

        // Commented out until display is fixed
        // increase_wave_difficulty();
        // if (count_remaining_cities() < 3) {
        //     repair_all_cities();
        // }

        // Continue playing
        current_state = STATE_PLAYING;
    }
}

// Update game over screen
void update_game_over(void) {
    // Check for START button to play again
    if (joypad() & J_START) {
        start_new_game();
    }

    // Check for SELECT to return to title
    if (joypad() & J_SELECT) {
        current_state = STATE_TITLE;
        show_title_screen();
    }
}

// Add to score
void add_score(uint16_t points) {
    score += points;
}

// Get current score
uint16_t get_score(void) {
    return score;
}

// Get high score
uint16_t get_high_score(void) {
    return high_score;
}

// Get current game state
GameState get_game_state(void) {
    return current_state;
}

// Set game state
void set_game_state(GameState state) {
    current_state = state;
}

// Setup game screen layout
void setup_game_screen(void) {
    // Clear screen
    gotoxy(0, 0);

    // Simple text-based game screen for now
    printf("Score: 0     Wave: 1\n");
    printf("--------------------\n");

    // Show some debug info
    printf("\nGame Started!\n");
    printf("A=Dit B=Dash\n");

    // Don't draw sprites yet until we fix the display
    // draw_cities();
}

// Update game display elements
void update_game_display(void) {
    // Update score every 10 frames
    if (frame_counter % 10 == 0) {
        update_score_display();
    }

    // Update morse input display
    update_morse_display();
}

// Update score display
void update_score_display(void) {
    // Position at top of screen
    gotoxy(0, 0);
    printf("Score:%05u W:%u", score, 1);  // Hardcode wave 1 for now
}

// Show title screen
void show_title_screen(void) {
    // Match the simple approach from test_input.c which works
    printf("\n\n  MORSE COMMAND\n\n");
    printf("  DEFEND CITIES!\n\n");
    printf("  A: DIT   B: DAH\n\n");
    printf("   PRESS START\n");
}

// Show game over screen
void show_game_over_screen(void) {
    // Don't call clear_screen() - it breaks the display
    // Just clear with printf like the working test
    gotoxy(0, 0);
    for (int i = 0; i < 18; i++) {
        printf("                    \n");
    }

    gotoxy(5, 4);
    printf("GAME OVER");

    gotoxy(3, 7);
    printf("ALL CITIES");
    gotoxy(4, 8);
    printf("DESTROYED");

    gotoxy(2, 11);
    printf("SCORE: %05u", score);

    if (score >= high_score) {
        gotoxy(3, 13);
        printf("NEW HIGH!");
    }

    gotoxy(2, 16);
    printf("START: RETRY");
}

// Play explosion sound effect
void play_explosion_sound(void) {
    // Use noise channel for explosion
    NR41_REG = 0x00;  // Length
    NR42_REG = 0xF1;  // Envelope (loud, fade out)
    NR43_REG = 0x22;  // Frequency
    NR44_REG = 0xC0;  // Trigger
}