#ifndef PGRAPHICS_H_
#define PGRAPHICS_H_

#include <stdint.h>

typedef struct psprite {
	uint8_t *bitmap;
	float x;
	float y;
	float dx;
	float dy;
	uint8_t width;
	uint8_t height;
	uint8_t is_visible;
} psprite_t;
typedef psprite_t* psprite_p;


uint8_t p_reverse_byte(const uint8_t b);

void p_clear_screen(void);

void p_show_screen(void);

void p_draw_char(uint8_t x, uint8_t y, uint8_t c);

void p_draw_string(uint8_t x, uint8_t y, char *str);

void p_draw_stringn(uint8_t x, uint8_t y, uint8_t n, char *str);

void p_draw_formatted_string(uint8_t x, uint8_t y, const char *fmt, ...);

void p_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);


void p_init_sprite(psprite_p sprite, float x, float y, uint8_t width, uint8_t height, uint8_t *bitmap);

void p_draw_sprite(const psprite_p sprite);

#endif
