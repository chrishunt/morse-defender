/*
 * Morse Command - Adaptive Timing Version
 * Single button morse with relative timing detection
 */

#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <rand.h>

// Game constants
#define STATE_TITLE 0
#define STATE_CALIBRATE 1  // New calibration state
#define STATE_PLAYING 2    // Was 1
#define STATE_GAME_OVER 3  // Was 2

// Morse timing constants (in frames at 60fps)
#define MIN_DIT_LENGTH 5      // Minimum dit = 83ms
#define MAX_DIT_LENGTH 30     // Maximum dit = 500ms
#define DEFAULT_DIT_UNIT 12   // Default = 200ms

// Sprite graphics data (8x8 pixels each)
unsigned char missile_sprite[] = {
    0x18,0x3C,0x7E,0xFF,0xFF,0x7E,0x3C,0x18  // Diamond shape missile
};

unsigned char explosion_frames[] = {
    // Frame 0: Small explosion
    0x00,0x00,0x18,0x3C,0x3C,0x18,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    // Frame 1: Medium explosion
    0x00,0x66,0xFF,0xFF,0xFF,0xFF,0x66,0x00,
    0x00,0x66,0xFF,0xFF,0xFF,0xFF,0x66,0x00,
    // Frame 2: Large explosion
    0x18,0x3C,0x7E,0xFF,0xFF,0x7E,0x3C,0x18,
    0x18,0x3C,0x7E,0xFF,0xFF,0x7E,0x3C,0x18
};

unsigned char trail_sprite[] = {
    0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00  // Tiny single pixel dot
};

/* Title screen tile graphics - commented out, keeping for potential future use
// Stylized block letters for "MORSE DEFENSE"
unsigned char title_tiles[] = {
    // Tile 0: 'M' - Wide military style
    0x00,0xC6,0xEE,0xFE,0xD6,0xC6,0xC6,0x00,
    // Tile 1: 'O' - Round tech style
    0x00,0x7C,0xFE,0xC6,0xC6,0xFE,0x7C,0x00,
    // Tile 2: 'R' - Strong angular
    0x00,0xFC,0xFE,0xC6,0xFE,0xFC,0xC6,0x00,
    // Tile 3: 'S' - Curved tactical
    0x00,0x7E,0xE0,0x7C,0x0E,0xFE,0x7C,0x00,
    // Tile 4: 'E' - Bold block
    0x00,0xFE,0xC0,0xFC,0xC0,0xFE,0xFE,0x00,
    // Tile 5: 'D' - Rounded strong
    0x00,0xF8,0xFC,0xC6,0xC6,0xFC,0xF8,0x00,
    // Tile 6: 'F' - Top-heavy tactical
    0x00,0xFE,0xFE,0xC0,0xFC,0xC0,0xC0,0x00,
    // Tile 7: 'N' - Diagonal strength
    0x00,0xC6,0xE6,0xF6,0xDE,0xCE,0xC6,0x00,
    // Tile 8: Space (empty)
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    // Tile 9: Morse dot pattern
    0x00,0x18,0x3C,0x3C,0x18,0x00,0x00,0x00,
    // Tile 10: Morse dash pattern
    0x00,0x00,0x00,0x7E,0x7E,0x00,0x00,0x00,
    // Tile 11: Decorative corner
    0xFF,0xFF,0xC0,0xC0,0xC0,0x00,0x00,0x00,
    // Tile 12: Radio wave pattern 1
    0x08,0x04,0x02,0x01,0x01,0x02,0x04,0x08,
    // Tile 13: Radio wave pattern 2
    0x10,0x20,0x40,0x80,0x80,0x40,0x20,0x10
};

// Tile base index in VRAM
#define TILE_BASE 0x80u
// Mapping for title letters to tile indices (add to TILE_BASE)
#define TILE_M (TILE_BASE + 0)
#define TILE_O (TILE_BASE + 1)
#define TILE_R (TILE_BASE + 2)
#define TILE_S (TILE_BASE + 3)
#define TILE_E (TILE_BASE + 4)
#define TILE_D (TILE_BASE + 5)
#define TILE_F (TILE_BASE + 6)
#define TILE_N (TILE_BASE + 7)
#define TILE_SPACE (TILE_BASE + 8)
#define TILE_DOT (TILE_BASE + 9)
#define TILE_DASH (TILE_BASE + 10)
*/

// Morse patterns for A-Z
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

// Multiple falling letters system
#define MAX_LETTERS 8
#define MAX_BUILDINGS 6

// Building structure
typedef struct {
    uint8_t x;        // X position of building center
    uint8_t active;   // Is this building still standing?
} Building;

// Letter/missile structure with trajectory
typedef struct {
    char letter;      // The letter (A-Z)
    int16_t x;        // X position in fixed-point (256 = 1 pixel)
    int16_t y;        // Y position in fixed-point
    uint8_t active;   // Is this letter slot in use?
    uint8_t targeted; // Is this letter hit but waiting for explosion?
    uint8_t target_building;  // Which building this is targeting
    int16_t x_velocity;       // X movement per frame (fixed-point)
    int16_t y_velocity;       // Y movement per frame (fixed-point)
} FallingLetter;

FallingLetter letters[MAX_LETTERS];
Building buildings[MAX_BUILDINGS];

// Current morse target (for pattern display)
char current_letter = 'A';

// Spawn timing
uint16_t spawn_timer = 0;
uint16_t spawn_interval = 240;  // Start at 4 seconds between spawns (slower start)

// Morse input buffer
char morse_buffer[8] = "";
uint8_t morse_index = 0;

// Calibration variables
uint8_t calibration_phase = 0;            // 0 = waiting for H, 1 = waiting for C
uint8_t calibration_indicator_timer = 0;  // Timer for showing feedback indicator

// Timing variables for adaptive morse
uint8_t button_held = 0;          // Is button currently held?
uint16_t hold_duration = 0;       // How long button has been held
uint16_t gap_duration = 0;        // Time since last button release
uint8_t dit_unit = DEFAULT_DIT_UNIT;  // Current dit reference length

// History for adaptive timing - track short tones only
#define HISTORY_SIZE 8
uint8_t short_tone_history[HISTORY_SIZE];
uint8_t history_index = 0;
uint8_t history_count = 0;

// Frame counter
uint16_t frame_counter = 0;

// Debug - last tone duration for display
uint16_t last_tone_duration = 0;

// Feedback message timer (will be removed once missiles work)
uint8_t feedback_timer = 0;

// Missile system
typedef struct {
    uint8_t active;      // Is missile flying?
    uint8_t start_x;     // Launch position
    uint8_t start_y;     // Bottom of screen
    int16_t current_x;   // Current position (16-bit for smooth movement)
    int16_t current_y;
    int16_t target_x;    // Target position
    int16_t target_y;
    int8_t vel_x;        // Velocity
    int8_t vel_y;
    uint8_t trail_count; // Number of trail sprites
} Missile;

Missile missile = {0};
uint8_t explosion_timer = 0;
uint8_t explosion_x, explosion_y;

// Forward declarations
void clear_missile_trail(void);
void show_game_over(void);
void play_feedback(uint8_t type);
void draw_city(void);
void play_correct(void);
void play_incorrect(void);
void play_explosion(void);
void play_building_explosion(void);

// Calculate the current dit unit from recent short tones ONLY
void update_dit_unit(uint8_t new_duration, uint8_t is_likely_dit) {
    // Only track tones that are likely dits (short tones)
    if (!is_likely_dit) return;

    // Add to history
    short_tone_history[history_index] = new_duration;
    history_index = (history_index + 1) % HISTORY_SIZE;
    if (history_count < HISTORY_SIZE) history_count++;

    // Need at least 2 samples before adapting
    if (history_count < 2) return;

    // Calculate average of short tones (more stable than minimum)
    uint16_t sum = 0;
    uint8_t valid_count = 0;
    for (uint8_t i = 0; i < history_count; i++) {
        if (short_tone_history[i] > 0) {
            sum += short_tone_history[i];
            valid_count++;
        }
    }

    if (valid_count > 0) {
        uint8_t avg = sum / valid_count;
        // Update dit unit, keeping it in reasonable range
        if (avg >= MIN_DIT_LENGTH && avg <= MAX_DIT_LENGTH) {
            dit_unit = avg;
        }
    }
}

// Sound functions
void start_tone(void) {
    NR52_REG = 0x80;  // Enable sound
    NR51_REG = 0xFF;  // All channels
    NR50_REG = 0x77;  // Max volume

    NR10_REG = 0x00;  // No sweep
    NR11_REG = 0x80;  // 50% duty cycle
    NR12_REG = 0xF0;  // Max volume, no envelope (continuous)

    // 500Hz tone
    NR13_REG = 0xFA;
    NR14_REG = 0x86;
}

void stop_tone(void) {
    NR12_REG = 0x00;  // Volume to 0
    NR14_REG = 0x00;  // Stop channel
}

void play_feedback(uint8_t type) {
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    NR10_REG = 0x00;
    NR11_REG = 0x80;
    NR12_REG = 0xF3;  // Quick decay

    if (type == 1) {
        // Success - high pitch
        NR13_REG = 0x73;
        NR14_REG = 0x87;
    } else if (type == 2) {
        // Error - low pitch
        NR13_REG = 0x2C;
        NR14_REG = 0x87;
    } else {
        // Neutral
        NR13_REG = 0xFA;
        NR14_REG = 0x86;
    }
}

// Correct pattern sound - major third above morse tone (Channel 2)
void play_correct(void) {
    // Enable sound
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x66;  // Medium volume (matching morse level)

    // Channel 2 - 625Hz (major third above 500Hz morse tone)
    NR21_REG = 0x80;  // 50% duty cycle
    NR22_REG = 0x53;  // Lower volume (5/15 ≈ 1/3), quick decay
    NR23_REG = 0x2E;  // 625Hz (low byte)
    NR24_REG = 0x87;  // 625Hz (high byte) + trigger
}

// Incorrect pattern sound - major third below morse tone (Channel 2)
void play_incorrect(void) {
    // Enable sound
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x66;  // Same volume as correct sound

    // Channel 2 - 400Hz (major third below 500Hz morse tone)
    NR21_REG = 0x80;  // 50% duty cycle
    NR22_REG = 0x53;  // Lower volume (5/15 ≈ 1/3), quick decay
    NR23_REG = 0xB8;  // 400Hz (low byte)
    NR24_REG = 0x86;  // 400Hz (high byte) + trigger
}

// Explosion sound using Channel 4 noise (for missile hits)
void play_explosion(void) {
    // Enable sound
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;  // Full volume for explosion

    // Channel 4 (noise channel) registers
    NR41_REG = 0x00;  // Sound length (unused)
    NR42_REG = 0xF2;  // Start loud, decay quickly
    NR43_REG = 0x32;  // Low frequency noise for rumble
    NR44_REG = 0xC0;  // Trigger noise channel
}

// Deeper explosion sound for building destruction
void play_building_explosion(void) {
    // Enable sound
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;  // Full volume for building explosion

    // Channel 4 (noise channel) registers
    NR41_REG = 0x00;  // Sound length (unused)
    NR42_REG = 0xF5;  // Start louder, slower decay for longer rumble
    NR43_REG = 0x42;  // Lower frequency noise for deeper rumble
    NR44_REG = 0xC0;  // Trigger noise channel
}

// Add morse symbol to buffer
void add_morse(char symbol) {
    if (morse_index < 7) {
        morse_buffer[morse_index++] = symbol;
        morse_buffer[morse_index] = '\0';
    }
}

// Clear morse buffer
void clear_morse(void) {
    morse_index = 0;
    morse_buffer[0] = '\0';
}

// Decode morse pattern to find which letter it represents
char decode_morse_pattern(void) {
    if (morse_index == 0) return 0;  // No input

    // Check against all letters A-Z
    for (uint8_t i = 0; i < 26; i++) {
        const char* pattern = morse_patterns[i];
        uint8_t match = 1;

        // Check if pattern length matches
        uint8_t pattern_len = 0;
        while (pattern[pattern_len]) pattern_len++;
        if (pattern_len != morse_index) continue;

        // Check if all characters match
        for (uint8_t j = 0; j < morse_index; j++) {
            if (morse_buffer[j] != pattern[j]) {
                match = 0;
                break;
            }
        }

        if (match) {
            return 'A' + i;  // Return the matching letter
        }
    }

    return 0;  // No match found
}

// Check morse pattern (DEPRECATED - keeping for reference)
uint8_t check_morse_status(void) {
    const char* pattern = morse_patterns[current_letter - 'A'];

    if (morse_index == 0) return 0;

    // Check each character
    for (uint8_t i = 0; i < morse_index; i++) {
        if (!pattern[i]) return 2;  // Too long
        if (morse_buffer[i] != pattern[i]) return 2;  // Wrong
    }

    // Complete match?
    if (!pattern[morse_index]) return 1;

    return 0;  // Partial match
}

// Initialize all letters as inactive
void init_letters(void) {
    for (uint8_t i = 0; i < MAX_LETTERS; i++) {
        letters[i].active = 0;
        letters[i].targeted = 0;
    }
}

// Initialize buildings
void init_buildings(void) {
    // Evenly space 6 buildings across the bottom
    buildings[0].x = 2;   buildings[0].active = 1;
    buildings[1].x = 5;   buildings[1].active = 1;
    buildings[2].x = 8;   buildings[2].active = 1;
    buildings[3].x = 11;  buildings[3].active = 1;
    buildings[4].x = 14;  buildings[4].active = 1;
    buildings[5].x = 17;  buildings[5].active = 1;
}

// Count active buildings
uint8_t count_active_buildings(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MAX_BUILDINGS; i++) {
        if (buildings[i].active) count++;
    }
    return count;
}

// Pick a random active building
uint8_t pick_random_active_building(void) {
    uint8_t active_count = count_active_buildings();
    if (active_count == 0) return 255;  // No buildings left

    uint8_t target = rand() % active_count;
    uint8_t count = 0;

    for (uint8_t i = 0; i < MAX_BUILDINGS; i++) {
        if (buildings[i].active) {
            if (count == target) return i;
            count++;
        }
    }
    return 255;  // Should never reach here
}

// Find a matching letter on screen - prioritizes bottom-most (most dangerous) letter
int8_t find_matching_letter(char target) {
    int8_t best_match = -1;
    int16_t highest_y = -1;  // Track the highest y position (closest to bottom)

    for (uint8_t i = 0; i < MAX_LETTERS; i++) {
        if (letters[i].active && !letters[i].targeted && letters[i].letter == target) {
            // Found a match - check if it's lower than our current best
            if (letters[i].y > highest_y) {
                highest_y = letters[i].y;
                best_match = i;
            }
        }
    }
    return best_match;  // Return index of bottom-most matching letter (or -1 if no match)
}

// Removed ASCII trail functions - no longer needed

// Spawn new letter in first available slot
void spawn_new_letter(void) {
    for (uint8_t i = 0; i < MAX_LETTERS; i++) {
        if (!letters[i].active) {
            // Pick a random active building as target
            uint8_t target_idx = pick_random_active_building();
            if (target_idx == 255) return;  // No buildings left

            // Random starting position at top (below header)
            uint8_t start_x = rand() % 20;
            letters[i].x = start_x << 8;  // Convert to fixed-point (256 units per pixel)
            letters[i].y = 3 << 8;  // Start at line 3 (below moved header separator)

            // Store target building
            letters[i].target_building = target_idx;

            // Calculate trajectory to target building (now at y=16)
            int16_t dx = (buildings[target_idx].x << 8) - letters[i].x;
            int16_t dy = (16 << 8) - (3 << 8);  // Distance from line 3 to building at y=16

            // Calculate velocity (spread movement over the fall distance of 13 lines)
            letters[i].x_velocity = dx / 13;  // Horizontal step per vertical unit
            letters[i].y_velocity = 256;      // Move down 1 pixel per frame

            letters[i].letter = 'A' + (rand() % 26);
            letters[i].active = 1;
            letters[i].targeted = 0;
            return;
        }
    }
}

// Update all letters (move down)
void update_all_letters(void) {
    uint8_t need_city_redraw = 0;  // Track if we need to redraw buildings

    // First pass: Clear old positions, move letters, check for building hits
    for (uint8_t i = 0; i < MAX_LETTERS; i++) {
        if (letters[i].active && !letters[i].targeted) {  // Don't move targeted letters
            // Get old pixel position and clear it
            uint8_t old_px = letters[i].x >> 8;
            uint8_t old_py = letters[i].y >> 8;
            if (old_px < 20 && old_py < 16) {
                gotoxy(old_px, old_py);
                printf(" ");
            }

            // Move using trajectory (fixed-point math)
            letters[i].x += letters[i].x_velocity;
            letters[i].y += letters[i].y_velocity;

            // Get new pixel position
            uint8_t py = letters[i].y >> 8;

            // Check if hit building level (buildings now at y=16)
            if (py >= 16) {
                // Destroy the targeted building
                if (letters[i].target_building < MAX_BUILDINGS) {
                    buildings[letters[i].target_building].active = 0;
                    need_city_redraw = 1;  // SET FLAG instead of immediate redraw
                }

                letters[i].active = 0;
                play_building_explosion();  // Deeper explosion for building destruction

                // Check for game over
                if (count_active_buildings() == 0) {
                    game_state = STATE_GAME_OVER;
                    show_game_over();
                }
            }
        }
    }

    // If city needs redraw, do it now
    if (need_city_redraw) {
        draw_city();
    }

    // Second pass: Draw all active letters at their current positions
    // This ensures letters are always drawn correctly after any city redraw
    for (uint8_t i = 0; i < MAX_LETTERS; i++) {
        if (letters[i].active && !letters[i].targeted) {
            uint8_t px = letters[i].x >> 8;
            uint8_t py = letters[i].y >> 8;
            if (px < 20 && py < 16) {
                gotoxy(px, py);
                printf("%c", letters[i].letter);
            }
        }
    }
}

// Draw city skyline
void draw_city(void) {
    uint8_t ground = '_';  // Ground line character

    // Clear lines 15-17 (buildings now at bottom of screen)
    gotoxy(0, 15);
    printf("                    ");
    gotoxy(0, 16);
    printf("                    ");
    gotoxy(0, 17);
    printf("                    ");

    // Draw all buildings at the bottom (lines 16-17)
    for (uint8_t i = 0; i < MAX_BUILDINGS; i++) {
        if (buildings[i].active) {
            uint8_t x = buildings[i].x;

            // Line 16: Building top (triangular roof)
            if (x > 0) {
                gotoxy(x - 1, 16);
            } else {
                gotoxy(x, 16);
            }
            printf("/\\");

            // Line 17: Building base (brackets)
            if (x > 0) {
                gotoxy(x - 1, 17);
            } else {
                gotoxy(x, 17);
            }
            printf("[]");
        }
    }

    // Fill in ground line between buildings at line 17
    gotoxy(0, 17);
    for(uint8_t i = 0; i < 20; i++) {
        // Check if this position is part of a building
        uint8_t is_building = 0;
        for (uint8_t j = 0; j < MAX_BUILDINGS; j++) {
            if (buildings[j].active) {
                uint8_t bx = buildings[j].x;
                if ((bx > 0 && (i == bx - 1 || i == bx)) || (bx == 0 && (i == 0 || i == 1))) {
                    is_building = 1;
                    break;
                }
            }
        }
        // Only draw ground where there's no building
        if (!is_building) {
            gotoxy(i, 17);
            printf("%c", ground);
        }
    }
}

// Clear all missile and trail sprites
void clear_missile_trail(void) {
    // Hide missile sprite
    move_sprite(0, 0, 0);
    // Hide explosion sprite
    move_sprite(1, 0, 0);
    // Hide all trail sprites (2-6) - move them off-screen
    for (uint8_t i = 2; i < 7; i++) {
        move_sprite(i, 0, 0);
        set_sprite_tile(i, 0);  // Clear sprite tile too
    }
    // Reset missile trail count
    if (missile.active) {
        missile.trail_count = 0;
    }
}

// Fire missile at specific letter
void fire_missile_at_letter(int8_t letter_index) {
    if (letter_index < 0 || letter_index >= MAX_LETTERS) return;

    missile.active = 1;

    // Random launch position from building locations
    uint8_t building_positions[] = {32, 48, 64, 80, 96, 112, 128, 144};
    missile.start_x = building_positions[rand() % 8];
    missile.start_y = 136; // Bottom of screen

    // Target the specific letter with sprite offset (8,16) plus character center (4,4)
    // FIXED: Convert from fixed-point to pixels first, then to sprite coordinates
    missile.target_x = ((letters[letter_index].x >> 8) * 8) + 8 + 4;  // Convert fixed-point to pixels, then to sprite coords
    missile.target_y = ((letters[letter_index].y >> 8) * 8) + 16 + 4; // Convert fixed-point to pixels, then to sprite coords

    // Set initial position (use 16-bit for smooth movement)
    missile.current_x = missile.start_x << 4;  // Fixed point (x16)
    missile.current_y = missile.start_y << 4;
    int16_t target_x_fixed = missile.target_x << 4;
    int16_t target_y_fixed = missile.target_y << 4;

    // Calculate velocity
    missile.vel_x = (target_x_fixed - missile.current_x) / 20;
    missile.vel_y = (target_y_fixed - missile.current_y) / 20;

    // Reset trail
    missile.trail_count = 0;

    // Show missile sprite at starting position
    move_sprite(0, missile.start_x, missile.start_y);
}

// Fire missile that misses (no matching letter on screen)
void fire_missile_miss(void) {
    missile.active = 1;

    // Random launch position from building locations
    uint8_t building_positions[] = {32, 48, 64, 80, 96, 112, 128, 144};
    missile.start_x = building_positions[rand() % 8];
    missile.start_y = 136; // Bottom of screen

    // Shoot at random position in upper area
    missile.target_x = 40 + (rand() % 80);
    missile.target_y = 40 + (rand() % 60);

    // Set initial position (use 16-bit for smooth movement)
    missile.current_x = missile.start_x << 4;  // Fixed point (x16)
    missile.current_y = missile.start_y << 4;
    int16_t target_x_fixed = missile.target_x << 4;
    int16_t target_y_fixed = missile.target_y << 4;

    // Calculate velocity
    missile.vel_x = (target_x_fixed - missile.current_x) / 20;
    missile.vel_y = (target_y_fixed - missile.current_y) / 20;

    // Reset trail
    missile.trail_count = 0;

    // Show missile sprite at starting position
    move_sprite(0, missile.start_x, missile.start_y);
}

// Update missile animation
void update_missile(void) {
    if (missile.active) {
        // Move missile (fixed point math)
        missile.current_x += missile.vel_x;
        missile.current_y += missile.vel_y;

        // Convert to screen coordinates
        uint8_t screen_x = missile.current_x >> 4;
        uint8_t screen_y = missile.current_y >> 4;

        // Update missile sprite position
        move_sprite(0, screen_x, screen_y);

        // Simplified trail effect - just one or two trail dots
        if (missile.trail_count < 2) {
            // Place trail sprites behind the missile
            for (uint8_t t = 0; t <= missile.trail_count && t < 2; t++) {
                uint8_t trail_idx = 2 + t;
                uint8_t trail_x = screen_x - (missile.vel_x >> 3) * (t + 1);
                uint8_t trail_y = screen_y - (missile.vel_y >> 3) * (t + 1);

                // Make sure trail is on screen
                if (trail_x < 160 && trail_y < 144) {
                    set_sprite_tile(trail_idx, 4);  // Use trail dot sprite
                    move_sprite(trail_idx, trail_x, trail_y);
                }
            }

            // Gradually add more trail dots
            if (frame_counter % 8 == 0 && missile.trail_count < 2) {
                missile.trail_count++;
            }
        }

        // Check if reached target
        if (abs(screen_x - missile.target_x) < 4 &&
            abs(screen_y - missile.target_y) < 4) {
            // Start explosion
            missile.active = 0;
            explosion_timer = 9;  // 3 frames x 3 ticks
            explosion_x = screen_x;
            explosion_y = screen_y;

            // Play explosion sound using Channel 4 (noise)
            play_explosion();

            // Clear any targeted letters now that missile has hit
            for (uint8_t i = 0; i < MAX_LETTERS; i++) {
                if (letters[i].targeted) {
                    // Convert fixed-point to pixel coordinates for clearing
                    uint8_t letter_px = letters[i].x >> 8;
                    uint8_t letter_py = letters[i].y >> 8;
                    if (letter_px < 20 && letter_py < 16) {
                        gotoxy(letter_px, letter_py);
                        printf(" ");
                    }
                    letters[i].active = 0;
                    letters[i].targeted = 0;
                    break;
                }
            }

            // Clear missile and trail
            clear_missile_trail();
        }
    }

    // Update explosion animation
    if (explosion_timer > 0) {
        uint8_t frame = (9 - explosion_timer) / 3;  // 0, 1, or 2
        set_sprite_tile(1, 1 + frame);  // Set explosion frame
        move_sprite(1, explosion_x, explosion_y);
        explosion_timer--;

        if (explosion_timer == 0) {
            move_sprite(1, 0, 0);  // Hide explosion
        }
    }
}

// Draw game screen (UI only, letters drawn separately)
void draw_game(void) {
    // Leave line 0 blank for visual spacing - explicitly clear it
    gotoxy(0, 0);
    printf("                    ");  // Clear line 0 to prevent artifacts

    // Draw score on line 1 (moved down from line 0)
    gotoxy(0, 1);
    printf("Score:%04u          ", score);

    // Draw separator on line 2 (moved down from line 1)
    gotoxy(0, 2);
    printf("--------------------");
}

// Show title screen
/* Tile graphics functions - commented out, keeping for potential future use
void init_title_graphics(void) {
    // Load custom font tiles starting at tile 128 (0x80) in VRAM
    set_bkg_data(TILE_BASE, 14, title_tiles);

    // Clear the screen with space tiles
    unsigned char space_tile = TILE_SPACE;
    for (uint8_t y = 0; y < 18; y++) {
        for (uint8_t x = 0; x < 20; x++) {
            set_bkg_tiles(x, y, 1, 1, &space_tile);
        }
    }
}

void draw_title_text(void) {
    unsigned char tile_index;

    // "MORSE" - centered at line 5 (position 7)
    tile_index = TILE_M;
    set_bkg_tiles(7, 5, 1, 1, &tile_index);
    tile_index = TILE_O;
    set_bkg_tiles(8, 5, 1, 1, &tile_index);
    tile_index = TILE_R;
    set_bkg_tiles(9, 5, 1, 1, &tile_index);
    tile_index = TILE_S;
    set_bkg_tiles(10, 5, 1, 1, &tile_index);
    tile_index = TILE_E;
    set_bkg_tiles(11, 5, 1, 1, &tile_index);

    // "DEFENSE" - centered at line 7 (position 6)
    tile_index = TILE_D;
    set_bkg_tiles(6, 7, 1, 1, &tile_index);
    tile_index = TILE_E;
    set_bkg_tiles(7, 7, 1, 1, &tile_index);
    tile_index = TILE_F;
    set_bkg_tiles(8, 7, 1, 1, &tile_index);
    tile_index = TILE_E;
    set_bkg_tiles(9, 7, 1, 1, &tile_index);
    tile_index = TILE_N;
    set_bkg_tiles(10, 7, 1, 1, &tile_index);
    tile_index = TILE_S;
    set_bkg_tiles(11, 7, 1, 1, &tile_index);
    tile_index = TILE_E;
    set_bkg_tiles(12, 7, 1, 1, &tile_index);

    // Decorative morse pattern line at line 9
    for (uint8_t x = 3; x < 17; x++) {
        if ((x % 3) == 0) {
            tile_index = TILE_DOT;
        } else if ((x % 3) == 1) {
            tile_index = TILE_DASH;
        } else {
            tile_index = TILE_SPACE;
        }
        set_bkg_tiles(x, 9, 1, 1, &tile_index);
    }
}
*/

void show_title(void) {
    // Enable display first
    DISPLAY_ON;

    // Clear screen
    cls();

    // Simple ASCII title
    gotoxy(3, 7);
    printf("MORSE DEFENDER");

    // Press start message
    gotoxy(4, 12);
    printf("PRESS START");
}

// Initialize calibration screen - minimal display
void init_calibration_screen(void) {
    // Clear screen once at the beginning
    for (uint8_t i = 0; i < 18; i++) {
        gotoxy(0, i);
        printf("                    ");
    }

    // Draw title
    gotoxy(0, 2);
    printf("   CALIBRATION");
    gotoxy(0, 3);
    printf("  ==============");

    // Simple instruction - no morse patterns shown
    gotoxy(0, 8);
    if (calibration_phase == 0) {
        printf(" Send the letter H");
    } else {
        printf(" Send the letter C");
    }

    // Line 11 will be used for the indicator when needed
}

// Show indicator that a letter was received
void show_calibration_indicator(void) {
    gotoxy(9, 11);
    printf("*");  // Simple asterisk indicator
    calibration_indicator_timer = 30;  // Show for 0.5 seconds
}

// Clear the calibration indicator
void clear_calibration_indicator(void) {
    gotoxy(9, 11);
    printf(" ");
    calibration_indicator_timer = 0;
}

// Update calibration display - handles indicator timer
void update_calibration_display(void) {
    // Only handle the indicator timer countdown
    if (calibration_indicator_timer > 0) {
        calibration_indicator_timer--;
        if (calibration_indicator_timer == 0) {
            clear_calibration_indicator();
        }
    }
}

// Old function kept for compatibility but simplified
void show_calibration(void) {
    init_calibration_screen();
    update_calibration_display();
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

// Main update
void update_game(void) {
    uint8_t keys = joypad();
    static uint8_t old_keys = 0;
    uint8_t pressed = keys & ~old_keys;
    uint8_t released = ~keys & old_keys;

    switch(game_state) {
        case STATE_TITLE:
            if (pressed & J_START) {
                // Go to calibration instead of directly to playing
                game_state = STATE_CALIBRATE;

                // Reset calibration variables
                calibration_phase = 0;  // Start with H
                calibration_indicator_timer = 0;
                clear_morse();
                dit_unit = DEFAULT_DIT_UNIT;  // Start with default timing

                // Initialize calibration screen (full draw)
                init_calibration_screen();
                play_feedback(0);
            }
            break;

        case STATE_CALIBRATE:
            // Handle morse button (A or B work the same)
            uint8_t calib_pressed = pressed & (J_A | J_B);
            uint8_t calib_released = released & (J_A | J_B);

            // Update indicator timer
            update_calibration_display();

            if (calib_pressed && !button_held) {
                // Button just pressed
                button_held = 1;
                hold_duration = 0;
                gap_duration = 0;
                start_tone();
            }

            if (button_held) {
                hold_duration++;

                if (calib_released) {
                    // Button released - classify the tone
                    stop_tone();
                    button_held = 0;

                    // Classify and add to morse buffer (with same adaptation as gameplay)
                    if (hold_duration < dit_unit * 22 / 10) {  // Use 2.2x threshold like gameplay
                        add_morse('.');
                        // Update dit reference only from short tones (same as gameplay)
                        update_dit_unit(hold_duration, 1);
                    } else {
                        add_morse('-');
                        // Don't update dit unit from dahs
                    }

                    // Reset gap timer
                    gap_duration = 0;
                }
            }

            // Track gap duration when not pressing
            if (!button_held && morse_index > 0) {
                gap_duration++;

                // Auto-complete on 3-unit gap (letter complete)
                if (gap_duration >= dit_unit * 3) {
                    // Show indicator that we received something
                    show_calibration_indicator();

                    if (calibration_phase == 0) {
                        // Phase 0: Check if pattern matches H (....)
                        if (morse_index == 4 &&
                            morse_buffer[0] == '.' && morse_buffer[1] == '.' &&
                            morse_buffer[2] == '.' && morse_buffer[3] == '.') {

                            // Success! H recognized - 4 dits gave us excellent timing calibration
                            calibration_phase = 1;  // Move to C phase
                            clear_morse();
                            init_calibration_screen();  // Redraw to show "Send the letter C"
                            play_feedback(1);
                        } else {
                            // Wrong pattern, just clear and wait for next attempt
                            // dit_unit has already adapted from any dits sent, so next attempt will be better
                            clear_morse();
                        }
                    } else {
                        // Phase 1: Check if pattern matches C (-.-.)
                        if (morse_index == 4 &&
                            morse_buffer[0] == '-' && morse_buffer[1] == '.' &&
                            morse_buffer[2] == '-' && morse_buffer[3] == '.') {

                            // Success! The dit_unit has already been adapted from H and C
                            // Start the game
                            game_state = STATE_PLAYING;
                            score = 0;

                            // Initialize buildings
                            init_buildings();

                            // Clear screen
                            for (uint8_t i = 0; i < 18; i++) {
                                gotoxy(0, i);
                                printf("                    ");
                            }

                            // Draw the city at the bottom
                            draw_city();

                            // Initialize letters system
                            init_letters();
                            spawn_timer = 0;
                            spawn_interval = 240;

                            // Start with one letter
                            spawn_new_letter();

                            clear_morse();
                            draw_game();

                            // Reset history for in-game adaptation
                            history_count = 0;
                            history_index = 0;
                            play_incorrect();  // Wrong calibration pattern
                        } else {
                            // Wrong pattern, just clear and wait for next attempt
                            clear_morse();
                        }
                    }
                }
            }

            // Allow SELECT to reset morse input
            if (pressed & J_SELECT) {
                calibration_indicator_timer = 0;
                clear_morse();
                clear_calibration_indicator();
            }
            break;

        case STATE_PLAYING:
            // Handle morse button (A or B work the same)
            uint8_t morse_button = keys & (J_A | J_B);
            uint8_t morse_pressed = pressed & (J_A | J_B);
            uint8_t morse_released = released & (J_A | J_B);

            if (morse_pressed && !button_held) {
                // Button just pressed - start tone and timer
                button_held = 1;
                hold_duration = 0;
                gap_duration = 0;
                start_tone();
            }

            if (button_held) {
                hold_duration++;

                // Safety limit
                if (hold_duration > 120) {  // 2 seconds max
                    button_held = 0;
                    stop_tone();
                }
            }

            if (morse_released && button_held) {
                // Button released - classify the tone
                button_held = 0;
                stop_tone();

                // Store for debug display
                last_tone_duration = hold_duration;

                // Classify based on threshold (2.2x for better separation)
                uint8_t is_dit = (hold_duration < dit_unit * 22 / 10);  // 2.2x threshold

                if (is_dit) {
                    add_morse('.');
                    // Update dit reference only from short tones
                    update_dit_unit(hold_duration, 1);
                } else {
                    add_morse('-');
                    // Don't update dit unit from dahs
                }

                // Reset gap timer (don't check pattern yet - wait for gap)
                gap_duration = 0;
            }

            // Track gap duration when not pressing
            if (!button_held && morse_index > 0) {
                gap_duration++;

                // Auto-complete on 3-unit gap (or longer)
                if (gap_duration >= dit_unit * 3 && morse_index > 0) {
                    // Decode what letter was entered
                    char entered_letter = decode_morse_pattern();

                    if (entered_letter) {
                        // Valid morse pattern - find this letter on screen
                        int8_t letter_index = find_matching_letter(entered_letter);

                        if (letter_index >= 0) {
                            // Found matching letter - hit it!
                            score += 10;
                            play_correct();  // Higher pitched confirmation sound

                            // Fire missile at this specific letter
                            fire_missile_at_letter(letter_index);

                            // Mark letter as targeted (will be destroyed when missile hits)
                            letters[letter_index].targeted = 1;
                        } else {
                            // Valid pattern but letter not on screen - miss
                            play_incorrect();  // Lower pitched warning sound
                            fire_missile_miss();
                        }
                    } else {
                        // Invalid pattern entered
                        play_incorrect();  // Lower pitched warning sound
                        fire_missile_miss();
                    }

                    clear_morse();
                    gap_duration = 0;  // Reset to prevent multiple processing
                    draw_game();
                }

                // Reset on 7-unit gap
                if (gap_duration >= dit_unit * 7) {
                    clear_morse();
                }
            }

            // SELECT to manually clear
            if (pressed & J_SELECT) {
                clear_morse();
            }

            // Spawn new letters on timer
            spawn_timer++;
            if (spawn_timer >= spawn_interval) {
                spawn_new_letter();
                spawn_timer = 0;

                // Gradually increase difficulty (faster progression)
                static uint8_t letters_spawned = 0;
                letters_spawned++;
                if (letters_spawned % 3 == 0 && spawn_interval > 30) {
                    spawn_interval -= 20;  // Faster ramp up, more frequent increases
                }
            }

            // Move all letters down every 60 frames (1 second)
            if (frame_counter % 60 == 0) {
                update_all_letters();
                draw_game();
            }

            // Update missile animation
            update_missile();
            break;

        case STATE_GAME_OVER:
            if (pressed & J_START) {
                game_state = STATE_TITLE;
                clear_missile_trail();  // Clear any sprites
                show_title();
                play_feedback(0);
            }
            break;
    }

    old_keys = keys;
    frame_counter++;
}

void main(void) {
    // Set palette for GBC
    if (_cpu == CGB_TYPE) {
        uint16_t palette[] = {
            RGB(28, 28, 24),  // Light green background
            RGB(20, 20, 16),
            RGB(8, 8, 8),
            RGB(0, 0, 0)      // Black text
        };
        set_bkg_palette(0, 1, palette);
        set_sprite_palette(0, 1, palette);  // Use same palette for sprites
    }

    // Initialize sprite graphics
    set_sprite_data(0, 1, missile_sprite);       // Sprite 0: missile
    set_sprite_data(1, 3, explosion_frames);     // Sprites 1-3: explosion frames
    set_sprite_data(4, 1, trail_sprite);         // Sprite 4: trail dot
    // Use same trail sprite for all trail positions
    for (uint8_t i = 5; i < 7; i++) {
        set_sprite_data(i, 1, trail_sprite);
    }

    // Enable sprites
    SHOW_SPRITES;

    // Hide all sprites initially
    for (uint8_t i = 0; i < 10; i++) {
        move_sprite(i, 0, 0);
    }

    // Initialize
    initrand(DIV_REG);

    // Initialize history
    for (uint8_t i = 0; i < HISTORY_SIZE; i++) {
        short_tone_history[i] = DEFAULT_DIT_UNIT;
    }

    // Show title
    show_title();

    // Main loop
    while(1) {
        update_game();
        vsync();
    }
}