#include <gb/gb.h>
#include <stdint.h>
#include "defense.h"
#include "missiles.h"
#include "cities.h"
#include "font_tiles.h"

// Array of defensive missiles
DefenseMissile defense_missiles[MAX_DEFENSE_MISSILES];

// Initialize defense system
void init_defense(void) {
    for (uint8_t i = 0; i < MAX_DEFENSE_MISSILES; i++) {
        defense_missiles[i].active = 0;
        defense_missiles[i].sprite_id = DEFENSE_SPRITE_START + i;
    }
}

// Launch a defensive missile at a target
void launch_defense_missile(uint8_t target_missile_id) {
    // Get the target missile
    Missile* target = get_missile(target_missile_id);
    if (!target || !target->active) {
        return;
    }

    // Find an inactive defense missile slot
    for (uint8_t i = 0; i < MAX_DEFENSE_MISSILES; i++) {
        if (!defense_missiles[i].active) {
            // Find the nearest city to launch from
            uint8_t city_id = find_nearest_city(target->x);
            City* city = get_city(city_id);

            if (city && !city->destroyed) {
                defense_missiles[i].active = 1;

                // Start from the city position
                defense_missiles[i].start_x = city->x + 8;  // Center of 16px city
                defense_missiles[i].start_y = city->y;
                defense_missiles[i].current_x = defense_missiles[i].start_x;
                defense_missiles[i].current_y = defense_missiles[i].start_y;

                // Target the missile's current position
                defense_missiles[i].target_x = target->x;
                defense_missiles[i].target_y = target->y;
                defense_missiles[i].target_missile_id = target_missile_id;

                // Calculate trajectory (simple linear for now)
                int16_t dx = defense_missiles[i].target_x - defense_missiles[i].start_x;
                int16_t dy = defense_missiles[i].target_y - defense_missiles[i].start_y;

                // Calculate speed components (fixed point math)
                if (dx != 0 || dy != 0) {
                    // Normalize to defense speed
                    uint16_t distance = 1;  // Simplified - would need proper sqrt
                    defense_missiles[i].speed_x = (dx * DEFENSE_SPEED) / distance;
                    defense_missiles[i].speed_y = (dy * DEFENSE_SPEED) / distance;
                }

                // Show the sprite
                set_sprite_tile(defense_missiles[i].sprite_id, MISSILE_TILE_INDEX);
                move_sprite(defense_missiles[i].sprite_id,
                           defense_missiles[i].current_x,
                           defense_missiles[i].current_y);

                break;
            }
        }
    }
}

// Launch a defense missile at coordinates (for missed morse codes)
void launch_defense_missile_xy(uint8_t x, uint8_t y) {
    // Find an inactive defense missile slot
    for (uint8_t i = 0; i < MAX_DEFENSE_MISSILES; i++) {
        if (!defense_missiles[i].active) {
            // Find the nearest city to launch from
            uint8_t city_id = find_nearest_city(x);
            City* city = get_city(city_id);

            if (city && !city->destroyed) {
                defense_missiles[i].active = 1;

                // Start from the city position
                defense_missiles[i].start_x = city->x + 8;
                defense_missiles[i].start_y = city->y;
                defense_missiles[i].current_x = defense_missiles[i].start_x;
                defense_missiles[i].current_y = defense_missiles[i].start_y;

                // Target the specified coordinates
                defense_missiles[i].target_x = x;
                defense_missiles[i].target_y = y;
                defense_missiles[i].target_missile_id = 0xFF;  // No specific target

                // Simple trajectory
                defense_missiles[i].speed_x = (x > defense_missiles[i].start_x) ? 2 : -2;
                defense_missiles[i].speed_y = -3;  // Always goes up

                // Show the sprite
                set_sprite_tile(defense_missiles[i].sprite_id, MISSILE_TILE_INDEX);
                move_sprite(defense_missiles[i].sprite_id,
                           defense_missiles[i].current_x,
                           defense_missiles[i].current_y);

                break;
            }
        }
    }
}

// Update all defense missiles
void update_defense_missiles(void) {
    for (uint8_t i = 0; i < MAX_DEFENSE_MISSILES; i++) {
        if (defense_missiles[i].active) {
            // Move towards target
            defense_missiles[i].current_x += defense_missiles[i].speed_x;
            defense_missiles[i].current_y += defense_missiles[i].speed_y;

            // Check if reached target area
            uint8_t reached = 0;
            if (defense_missiles[i].target_missile_id != 0xFF) {
                // Targeting a specific missile
                Missile* target = get_missile(defense_missiles[i].target_missile_id);
                if (target && target->active) {
                    // Check collision (within 8 pixels)
                    int8_t dx = defense_missiles[i].current_x - target->x;
                    int8_t dy = defense_missiles[i].current_y - target->y;
                    if (dx < 8 && dx > -8 && dy < 8 && dy > -8) {
                        // Hit the target!
                        reached = 1;
                        explode_defense_missile(i);
                        destroy_missile(defense_missiles[i].target_missile_id);
                    }
                } else {
                    // Target no longer exists
                    defense_missiles[i].active = 0;
                    hide_sprite(defense_missiles[i].sprite_id);
                }
            } else {
                // Just going to coordinates
                if (defense_missiles[i].current_y <= defense_missiles[i].target_y) {
                    reached = 1;
                    explode_defense_missile(i);
                }
            }

            // Check if went off screen
            if (defense_missiles[i].current_y < 8 ||
                defense_missiles[i].current_y > 144 ||
                defense_missiles[i].current_x < 8 ||
                defense_missiles[i].current_x > 160) {
                defense_missiles[i].active = 0;
                hide_sprite(defense_missiles[i].sprite_id);
            } else if (!reached) {
                // Update sprite position
                move_sprite(defense_missiles[i].sprite_id,
                           defense_missiles[i].current_x,
                           defense_missiles[i].current_y);
            }
        }
    }
}

// Create explosion effect for defense missile
void explode_defense_missile(uint8_t missile_id) {
    if (missile_id >= MAX_DEFENSE_MISSILES) return;

    DefenseMissile* missile = &defense_missiles[missile_id];

    // Create explosion at current position
    // TODO: Implement explosion animation system
    set_sprite_tile(missile->sprite_id, EXPLOSION_TILES_START);

    // Check for any missiles in blast radius
    for (uint8_t i = 0; i < MAX_MISSILES; i++) {
        Missile* m = get_missile(i);
        if (m && m->active) {
            int8_t dx = missile->current_x - m->x;
            int8_t dy = missile->current_y - m->y;
            if (dx < 12 && dx > -12 && dy < 12 && dy > -12) {
                // Caught in explosion!
                destroy_missile(i);
            }
        }
    }

    // Deactivate the defense missile
    missile->active = 0;
    // Note: In full implementation, would have explosion linger for a few frames
}