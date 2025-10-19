#ifndef CITIES_H
#define CITIES_H

#include <stdint.h>

// Number of cities to defend
#define NUM_CITIES 6

// City health (hits before destroyed)
#define MAX_CITY_HEALTH 3

// Y position for cities (bottom of play area)
#define CITY_Y_POSITION 120

// Starting sprite ID for cities
#define CITY_SPRITE_START 20

// City structure
typedef struct {
    uint8_t x;              // X position
    uint8_t y;              // Y position
    uint8_t health;         // Current health (0-3)
    uint8_t destroyed;      // Is city destroyed?
    uint8_t sprite_base_id; // Base sprite ID (uses 4 sprites for 16x16)
} City;

// Global cities array
extern City cities[NUM_CITIES];

// Function prototypes
void init_cities(void);
void draw_cities(void);
void draw_city(uint8_t city_id);
void damage_city(uint8_t city_id);
uint8_t find_nearest_city(uint8_t x);
uint8_t all_cities_destroyed(void);
City* get_city(uint8_t index);
uint8_t count_remaining_cities(void);
void repair_city(uint8_t city_id);
void repair_all_cities(void);

#endif // CITIES_H