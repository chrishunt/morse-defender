#include <gb/gb.h>
#include <stdlib.h>
#include <stdint.h>
#include <rand.h>
#include "missiles.h"
#include "font_tiles.h"

// Array to store all active missiles
Missile missiles[MAX_MISSILES];

// Current difficulty settings
static uint8_t missile_speed = 1;
static uint16_t spawn_delay = 180;  // 3 seconds at 60fps
static uint16_t spawn_timer = 0;
static uint8_t wave_number = 1;

// Initialize missile system
void init_missiles(void) {
    for (uint8_t i = 0; i < MAX_MISSILES; i++) {
        missiles[i].active = 0;
        missiles[i].sprite_id = i * 2;  // Each missile uses 2 sprite slots for potential effects
    }
    missile_speed = 1;
    spawn_delay = 180;
    spawn_timer = 0;
    wave_number = 1;
}

// Spawn a new missile at random position
void spawn_missile(void) {
    // Find inactive missile slot
    for (uint8_t i = 0; i < MAX_MISSILES; i++) {
        if (!missiles[i].active) {
            missiles[i].active = 1;

            // Random X position across screen (8-152 for 8x8 sprite)
            missiles[i].x = 8 + (rand() % 144);
            missiles[i].y = 0;  // Start at top

            // Random letter or number
            uint8_t char_type = rand() % 36;  // 26 letters + 10 numbers
            if (char_type < 26) {
                missiles[i].letter = 'A' + char_type;
            } else {
                missiles[i].letter = '0' + (char_type - 26);
            }

            // Set speed based on wave
            missiles[i].speed = missile_speed;

            // Position sprite
            move_sprite(missiles[i].sprite_id, missiles[i].x, missiles[i].y);

            // Set sprite tile to the letter
            set_sprite_tile(missiles[i].sprite_id, get_font_tile_index(missiles[i].letter));

            break;
        }
    }
}

// Update all missiles (move down, check if hit ground)
void update_missiles(void) {
    // Handle spawning
    spawn_timer++;
    if (spawn_timer >= spawn_delay) {
        spawn_missile();
        spawn_timer = 0;
    }

    // Update each active missile
    for (uint8_t i = 0; i < MAX_MISSILES; i++) {
        if (missiles[i].active) {
            // Move missile down
            missiles[i].y += missiles[i].speed;

            // Check if hit ground (y > 112 for ground level)
            if (missiles[i].y >= GROUND_LEVEL) {
                // Missile hit ground - damage nearest city
                missiles[i].active = 0;
                hide_sprite(missiles[i].sprite_id);

                // TODO: Call city damage function
            } else {
                // Update sprite position
                move_sprite(missiles[i].sprite_id, missiles[i].x, missiles[i].y);
            }
        }
    }
}

// Check if a letter matches any active missile
uint8_t check_missile_hit(char letter) {
    for (uint8_t i = 0; i < MAX_MISSILES; i++) {
        if (missiles[i].active && missiles[i].letter == letter) {
            // Found a match!
            return i;
        }
    }
    return 0xFF;  // No match found
}

// Destroy a missile (when hit by defense)
void destroy_missile(uint8_t missile_id) {
    if (missile_id < MAX_MISSILES && missiles[missile_id].active) {
        missiles[missile_id].active = 0;
        hide_sprite(missiles[missile_id].sprite_id);

        // TODO: Trigger explosion animation at missile position
        // TODO: Add score points
    }
}

// Get missile at index
Missile* get_missile(uint8_t index) {
    if (index < MAX_MISSILES) {
        return &missiles[index];
    }
    return NULL;
}

// Increase difficulty for next wave
void increase_wave_difficulty(void) {
    wave_number++;

    // Increase speed every 2 waves
    if (wave_number % 2 == 0 && missile_speed < 3) {
        missile_speed++;
    }

    // Decrease spawn delay (more frequent missiles)
    if (spawn_delay > 60) {  // Minimum 1 second between spawns
        spawn_delay -= 20;
    }
}

// Get current wave number
uint8_t get_wave_number(void) {
    return wave_number;
}

// Count active missiles
uint8_t count_active_missiles(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MAX_MISSILES; i++) {
        if (missiles[i].active) {
            count++;
        }
    }
    return count;
}