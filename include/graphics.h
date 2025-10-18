#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

// Function prototypes
void init_graphics(void);
void display_title(void);
void clear_screen(void);
void display_text(uint8_t x, uint8_t y, const char* text);
void update_morse_display(void);

#endif // GRAPHICS_H