#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <avr/pgmspace.h>
#include "ascii_font.h"
#include "graphics.h"
#include "pgraphics.h"

#define MIN(a, b)	(((a) < (b)) ? (a) : (b))

static uint8_t reverse_byte_lookup[16U] = {0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe, 0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf};

uint8_t p_reverse_byte(const uint8_t b) {
	return ((reverse_byte_lookup[b & 15U] << 4U) | (reverse_byte_lookup[b >> 4U]));
}

void p_clear_screen(void) {
	memset(screenBuffer, 0U, LCD_BUFFER_SIZE);
}

void p_draw_char(uint8_t x, uint8_t y, char c) {
	if ((c < 32U) || (c > 127U)) {
		return;
	}

	// TODO: partly visible chars, left or top
	if ((x > LCD_X) || (y > LCD_Y) || (x < 0U) || (y < 0U)) {
		return;
	}

	uint8_t x_max = MIN((x + 5U), LCD_X);
	uint8_t v_offset = y - (((uint8_t) floor(y / 8.f)) * 8U);
	uint8_t y_sb = ((uint8_t) floor(y / 8.f));

	c -= 32U;

	if ((y_sb * LCD_X) < LCD_BUFFER_SIZE) {
		if (0U == v_offset) {
			for (uint8_t x_sb = x, i = 0; (i < 5U) && (x_sb < x_max); ++i, ++x_sb) {
				screenBuffer[y_sb * LCD_X + x_sb] |= pgm_read_byte((&(ASCII[c][i])));
			}
		}
		else {
			uint8_t i_v_offset = 8U - v_offset;

			for (uint8_t x_sb = x, i = 0; (i < 5U) && (x_sb < x_max); ++i, ++x_sb) {
				screenBuffer[y_sb * LCD_X + x_sb] |= (pgm_read_byte((&(ASCII[c][i]))) << v_offset);

				if (((y_sb + 1U) * LCD_X) < LCD_BUFFER_SIZE) {
					screenBuffer[y_sb * LCD_X + x_sb] |= (pgm_read_byte((&(ASCII[c][i]))) >> i_v_offset);
				}
			}
		}
	}
}

void p_draw_string(uint8_t x, uint8_t y, char *str) {
	for (uint8_t i = 0U; (*str) != '\0'; ++i, ++str) {
		p_draw_char(x + i * 5U, y, (*str));
	}
}

void p_draw_stringn(uint8_t x, uint8_t y, uint8_t n, char *str) {
	--n;
	for (; n >= 0U; --n) {
		p_draw_char(x + n * 5U, y, str[n]);
	}
}

void p_draw_formatted_string(uint8_t x, uint8_t y, const char *fmt, ...) {
	char buffer[18U];
	va_list args;
	va_start(args, fmt);

	vsnprintf(buffer, 18U, fmt, args);

	p_draw_string(x, y, buffer);
}

void p_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	// TODO: performance bottle neck, implement own draw line function
	draw_line(x1, y1, x2, y2);
}


void p_init_sprite(psprite_p sprite, float x, float y, uint8_t width, uint8_t height, uint8_t *bitmap) {
	sprite->x = x;
	sprite->y = y;
	sprite->width = width;
	sprite->height = height;
	sprite->bitmap = bitmap;

	sprite->is_visible = 1U;
	sprite->dx = 0.f;
	sprite->dy = 0.f;
}

void p_draw_sprite(const psprite_p sprite) {
	if ((NULL == sprite) || !(sprite->is_visible)) {
		return;
	}

	// TODO: partly visible sprites, left or top
	if ((sprite->x >= LCD_X) || (sprite->y >= LCD_Y) ||
		((sprite->x < 0U)/* && ((sprite->x + sprite->width) < 0)*/) ||
		((sprite->y < 0U)/* && ((sprite->y + sprite->height) < 0)*/)) {
		return;
	}

	uint8_t x_max = MIN(((uint8_t) sprite->x + sprite->width), LCD_X);
	uint8_t v_byte_count = ((uint8_t) ceil(sprite->height / 8.f));
	uint8_t v_offset = ((uint8_t) sprite->y) - (((uint8_t) floor(sprite->y / 8U)) * 8U);

	if (0U == v_offset) {
		for (uint8_t y = ((uint8_t) floor(sprite->y / 8U)), c = 0U; (c < v_byte_count) && ((y * LCD_X) < LCD_BUFFER_SIZE); ++y, ++c) {
			for (uint8_t x = sprite->x, d = 0U; x < x_max; ++x, ++d) {
				screenBuffer[y * LCD_X + x] = sprite->bitmap[v_byte_count * d + c];
			}
		}
	}
	else {
		uint8_t i_v_offset = 8U - v_offset;

		for (uint8_t y = ((uint8_t) floor(sprite->y / 8)), c = 0U; (c < v_byte_count) && ((y * LCD_X) < LCD_BUFFER_SIZE); ++y, ++c) {
			for (uint8_t x = sprite->x, d = 0U; x < x_max; ++x, ++d) {
				screenBuffer[y * LCD_X + x] |= (sprite->bitmap[v_byte_count * d + c] << v_offset);
				
				if (((y + 1U) * LCD_X) < LCD_BUFFER_SIZE) {
					screenBuffer[(y + 1U) * LCD_X + x] |= (sprite->bitmap[v_byte_count * d + c] >> i_v_offset);
				}
			}
		}
	}
}
