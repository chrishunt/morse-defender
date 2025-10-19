#ifndef FONT_TILES_H
#define FONT_TILES_H

#include <stdint.h>

// Font tile data arrays
extern const unsigned char font_tiles[];
extern const unsigned char city_tiles[];
extern const unsigned char explosion_tiles[];
extern const unsigned char missile_tile[];
extern const unsigned char ground_tiles[];

// Tile indices
#define FONT_TILES_START 0
#define FONT_TILES_COUNT 36  // A-Z (26) + 0-9 (10)

#define CITY_TILES_START 36
#define CITY_TILES_COUNT 4

#define EXPLOSION_TILES_START 40
#define EXPLOSION_FRAMES 4

#define MISSILE_TILE_INDEX 44
#define GROUND_TILE_INDEX 45

// Helper function to get tile index for a character
uint8_t get_font_tile_index(char c);

#endif // FONT_TILES_H