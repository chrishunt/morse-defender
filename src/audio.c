#include <gb/gb.h>
#include <stdint.h>
#include "audio.h"

// Sound frequencies for morse code
#define MORSE_TONE_FREQ 0x0700  // ~800 Hz tone
#define DOT_DURATION 15          // Quarter second at 60fps
#define DASH_DURATION 45         // Three quarters second

// Sound timer for note duration
static uint8_t sound_timer = 0;

void init_audio(void) {
    // Enable sound system
    NR52_REG = 0x80;  // Master sound enable
    NR50_REG = 0x77;  // Volume max on both left and right
    NR51_REG = 0xFF;  // Enable all channels on both speakers

    sound_timer = 0;
}

void update_audio(void) {
    // Decrement sound timer and stop sound when it reaches 0
    if (sound_timer > 0) {
        sound_timer--;
        if (sound_timer == 0) {
            stop_sound();
        }
    }
}

void play_tone(uint16_t frequency, uint8_t duration) {
    // Use sound channel 1 (square wave with envelope)

    // NR10: Sweep (disabled)
    NR10_REG = 0x00;

    // NR11: Wave duty and length
    // Bits 7-6: Wave duty (50%)
    // Bits 5-0: Sound length (not used when continuous)
    NR11_REG = 0x80;

    // NR12: Volume envelope
    // Bits 7-4: Initial volume (max)
    // Bit 3: Direction (0=decrease)
    // Bits 2-0: Sweep pace
    NR12_REG = 0xF0;

    // NR13: Frequency low bits
    NR13_REG = frequency & 0xFF;

    // NR14: Frequency high bits and control
    // Bit 7: Restart sound
    // Bit 6: Use length
    // Bits 2-0: Frequency high bits
    NR14_REG = 0x80 | ((frequency >> 8) & 0x07);

    // Set timer for sound duration
    sound_timer = duration;
}

void play_dot_beep(void) {
    play_tone(MORSE_TONE_FREQ, DOT_DURATION);
}

void play_dash_beep(void) {
    play_tone(MORSE_TONE_FREQ, DASH_DURATION);
}

void play_success_sound(void) {
    // Play a higher pitched beep for success
    play_tone(0x0600, 20);
}

void play_error_sound(void) {
    // Play a lower pitched beep for error
    play_tone(0x0400, 30);
}

void stop_sound(void) {
    // Stop channel 1
    NR12_REG = 0x00;  // Set volume to 0
    NR14_REG = 0x80;  // Restart with no sound
}