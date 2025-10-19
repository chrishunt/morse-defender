#ifndef MISSILES_H
#define MISSILES_H

#include <stdint.h>

// Maximum number of missiles on screen at once
#define MAX_MISSILES 10

// Ground level Y position (where cities are)
#define GROUND_LEVEL 112

// Missile structure
typedef struct {
    uint8_t x;          // X position
    uint8_t y;          // Y position
    char letter;        // The letter/number this missile shows
    uint8_t speed;      // Fall speed
    uint8_t active;     // Is this missile active?
    uint8_t sprite_id;  // Sprite ID for this missile
} Missile;

// Global missiles array
extern Missile missiles[MAX_MISSILES];

// Function prototypes
void init_missiles(void);
void spawn_missile(void);
void update_missiles(void);
uint8_t check_missile_hit(char letter);
void destroy_missile(uint8_t missile_id);
Missile* get_missile(uint8_t index);
void increase_wave_difficulty(void);
uint8_t get_wave_number(void);
uint8_t count_active_missiles(void);

#endif // MISSILES_H