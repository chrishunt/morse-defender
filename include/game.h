#ifndef GAME_H
#define GAME_H

#include <stdint.h>

// Game states
typedef enum {
    STATE_TITLE,
    STATE_PLAYING,
    STATE_WAVE_COMPLETE,
    STATE_GAME_OVER
} GameState;

// Game constants
#define MISSILES_PER_WAVE 20
#define POINTS_PER_HIT 100
#define POINTS_PER_CITY 500

// Function prototypes
void init_game(void);
void start_new_game(void);
void update_game(void);
void update_title_screen(void);
void update_gameplay(void);
void update_wave_complete(void);
void update_game_over(void);
void add_score(uint16_t points);
uint16_t get_score(void);
uint16_t get_high_score(void);
GameState get_game_state(void);
void set_game_state(GameState state);
void setup_game_screen(void);
void update_game_display(void);
void update_score_display(void);
void show_title_screen(void);
void show_game_over_screen(void);
void play_explosion_sound(void);

#endif // GAME_H