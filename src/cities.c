#include <gb/gb.h>
#include <stdint.h>
#include "cities.h"
#include "font_tiles.h"

// Array of cities/buildings
City cities[NUM_CITIES];

// Initialize all cities
void init_cities(void) {
    // Position cities evenly across the bottom of the screen
    uint8_t spacing = 160 / (NUM_CITIES + 1);

    for (uint8_t i = 0; i < NUM_CITIES; i++) {
        cities[i].x = spacing * (i + 1) - 8;  // Center the 16px wide building
        cities[i].y = CITY_Y_POSITION;
        cities[i].health = MAX_CITY_HEALTH;
        cities[i].destroyed = 0;
        cities[i].sprite_base_id = CITY_SPRITE_START + (i * 4);  // Each city uses 4 sprites (2x2 tiles)
    }

    // Draw all cities
    draw_cities();
}

// Draw all cities on screen
void draw_cities(void) {
    for (uint8_t i = 0; i < NUM_CITIES; i++) {
        if (!cities[i].destroyed) {
            draw_city(i);
        }
    }
}

// Draw a single city
void draw_city(uint8_t city_id) {
    if (city_id >= NUM_CITIES) return;

    City* city = &cities[city_id];

    if (city->destroyed) {
        // Hide sprites for destroyed city
        for (uint8_t j = 0; j < 4; j++) {
            hide_sprite(city->sprite_base_id + j);
        }
    } else {
        // Draw city as 2x2 tile block (16x16 pixels)
        // Top-left tile
        set_sprite_tile(city->sprite_base_id, CITY_TILES_START);
        move_sprite(city->sprite_base_id, city->x, city->y);

        // Top-right tile
        set_sprite_tile(city->sprite_base_id + 1, CITY_TILES_START + 1);
        move_sprite(city->sprite_base_id + 1, city->x + 8, city->y);

        // Bottom-left tile
        set_sprite_tile(city->sprite_base_id + 2, CITY_TILES_START + 2);
        move_sprite(city->sprite_base_id + 2, city->x, city->y + 8);

        // Bottom-right tile
        set_sprite_tile(city->sprite_base_id + 3, CITY_TILES_START + 3);
        move_sprite(city->sprite_base_id + 3, city->x + 8, city->y + 8);

        // Set sprite palette based on health
        uint8_t palette = 0;
        if (city->health == 2) {
            palette = 1;  // Yellow - damaged
        } else if (city->health == 1) {
            palette = 2;  // Red - critical
        }

        for (uint8_t j = 0; j < 4; j++) {
            set_sprite_prop(city->sprite_base_id + j, palette);
        }
    }
}

// Damage a city
void damage_city(uint8_t city_id) {
    if (city_id >= NUM_CITIES) return;

    City* city = &cities[city_id];

    if (!city->destroyed && city->health > 0) {
        city->health--;

        if (city->health == 0) {
            city->destroyed = 1;
            // TODO: Trigger explosion animation
            // TODO: Play destruction sound
        }

        // Redraw the city to show damage
        draw_city(city_id);
    }
}

// Find nearest city to an X position
uint8_t find_nearest_city(uint8_t x) {
    uint8_t nearest = 0;
    uint8_t min_distance = 255;

    for (uint8_t i = 0; i < NUM_CITIES; i++) {
        if (!cities[i].destroyed) {
            uint8_t distance;
            if (x > cities[i].x) {
                distance = x - cities[i].x;
            } else {
                distance = cities[i].x - x;
            }

            if (distance < min_distance) {
                min_distance = distance;
                nearest = i;
            }
        }
    }

    return nearest;
}

// Check if all cities are destroyed (game over condition)
uint8_t all_cities_destroyed(void) {
    for (uint8_t i = 0; i < NUM_CITIES; i++) {
        if (!cities[i].destroyed) {
            return 0;  // At least one city still standing
        }
    }
    return 1;  // All cities destroyed
}

// Get city at index
City* get_city(uint8_t index) {
    if (index < NUM_CITIES) {
        return &cities[index];
    }
    return NULL;
}

// Count remaining cities
uint8_t count_remaining_cities(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < NUM_CITIES; i++) {
        if (!cities[i].destroyed) {
            count++;
        }
    }
    return count;
}

// Repair a city (for between waves or power-ups)
void repair_city(uint8_t city_id) {
    if (city_id >= NUM_CITIES) return;

    City* city = &cities[city_id];

    if (!city->destroyed) {
        city->health = MAX_CITY_HEALTH;
        draw_city(city_id);
    }
}

// Repair all cities
void repair_all_cities(void) {
    for (uint8_t i = 0; i < NUM_CITIES; i++) {
        if (!cities[i].destroyed) {
            cities[i].health = MAX_CITY_HEALTH;
        }
    }
    draw_cities();
}