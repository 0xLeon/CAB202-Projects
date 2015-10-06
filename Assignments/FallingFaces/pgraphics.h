#ifndef PGRAPHICS_H_
#define PGRAPHICS_H_

#include <stdint.h>
#include "sprite.h"

typedef Sprite psprite_t;
typedef psprite_t* psprite_p;


uint8_t p_reverse_byte(const uint8_t b);

void p_clear_screen(void);

void p_draw_char(uint8_t x, uint8_t y, char c);

void p_draw_string(uint8_t x, uint8_t y, char *str);

void p_draw_stringn(uint8_t x, uint8_t y, uint8_t n, char *str);

void p_draw_formatted_string(uint8_t x, uint8_t y, const char *fmt, ...);

void p_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void p_draw_sprite(const psprite_p sprite);

#endif
