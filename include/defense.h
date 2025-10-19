#ifndef DEFENSE_H
#define DEFENSE_H

#include <stdint.h>

// Maximum number of defense missiles active at once
#define MAX_DEFENSE_MISSILES 5

// Defense missile speed
#define DEFENSE_SPEED 4

// Starting sprite ID for defense missiles
#define DEFENSE_SPRITE_START 50

// Defense missile structure
typedef struct {
    uint8_t active;             // Is this missile active?
    uint8_t sprite_id;          // Sprite ID
    uint8_t start_x, start_y;   // Launch position (from city)
    uint8_t target_x, target_y; // Target position
    uint8_t current_x, current_y; // Current position
    int8_t speed_x, speed_y;    // Velocity components
    uint8_t target_missile_id;  // ID of missile being targeted (0xFF if none)
} DefenseMissile;

// Global defense missiles array
extern DefenseMissile defense_missiles[MAX_DEFENSE_MISSILES];

// Function prototypes
void init_defense(void);
void launch_defense_missile(uint8_t target_missile_id);
void launch_defense_missile_xy(uint8_t x, uint8_t y);
void update_defense_missiles(void);
void explode_defense_missile(uint8_t missile_id);

#endif // DEFENSE_H