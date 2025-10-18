#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

// Function prototypes
void init_audio(void);
void update_audio(void);
void play_tone(uint16_t frequency, uint8_t duration);
void play_dot_beep(void);
void play_dash_beep(void);
void play_success_sound(void);
void play_error_sound(void);
void stop_sound(void);

#endif // AUDIO_H