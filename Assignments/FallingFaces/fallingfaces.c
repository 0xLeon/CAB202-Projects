#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "buttons.h"
#include "hardware.h"
#include "graphics.h"
#include "sprite.h"
#include "pgraphics.h"
#include "game.h"
#include "fallingfaces.h"

#include "level0.h"
#include "level1.h"

#define FACE_HAPPY	0U
#define FACE_ANGRY	1U
#define FACE_MAD	2U

game_p create_falling_faces(void);
void initializeTimers(void);

unsigned char bm_face_happy[] = {
	0b11111111, 0b11111111,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000011,
	0b00110001, 0b10000100,
	0b00110001, 0b10001000,
	0b00000001, 0b10001000,
	0b00000001, 0b10001000,
	
	0b00000001, 0b10001000,
	0b00000001, 0b10001000,
	0b00110001, 0b10001000,
	0b00110001, 0b10000100,
	0b00000001, 0b10000011,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b11111111, 0b11111111
};

unsigned char bm_face_angry[] = {
	0b11111111, 0b11111111,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00001001, 0b10000000,
	0b00010001, 0b10001000,
	0b00100001, 0b10000100,
	0b00000001, 0b10000010,
	0b00000001, 0b10000010,

	0b00000001, 0b10000010,
	0b00000001, 0b10000010,
	0b00100001, 0b10000100,
	0b00010001, 0b10001000,
	0b00001001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b11111111, 0b11111111
};

unsigned char bm_face_mad[] = {
	0b11111111, 0b11111111,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b00011001, 0b10000001,
	0b00011001, 0b10000001,
	0b00000001, 0b10000001,
	0b00000001, 0b10000001,

	0b00000001, 0b10000111,
	0b00000001, 0b10000111,
	0b00000001, 0b10000001,
	0b01100001, 0b10000000,
	0b01100001, 0b10000000,
	0b00000001, 0b10000000,
	0b00000001, 0b10000000,
	0b11111111, 0b11111111
};


volatile uint8_t redraw = 1U;
volatile game_p game = NULL;

int main(void) {
	initializeHardware();
	initializeTimers();

	game = create_falling_faces();

	game->current_level->load(game->current_level, game);

	sei();

	while (1);

	return 0U;
}

game_p create_falling_faces(void) {
	game_p game = calloc(1, sizeof(game_t));

	if (NULL == game) {
		// TODO: display error
		return NULL;
	}

	game->levels[0U] = create_level0(game);
	game->levels[1U] = create_level1(game);
	// game->levels[2U] = create_level2(game);
	// game->levels[3U] = create_level3(game);
	game->current_level = game->levels[0U];

	game->lives = 3U;
	game->score = 0U;

	game->player = calloc(1U, sizeof(psprite_t));

	if (NULL == game->player) {
		free(game);
		return NULL;
	}

	init_sprite(game->player, 0.f, 40.f, 5U, 7U, NULL);


	game->faces[FACE_HAPPY] = calloc(1U, sizeof(psprite_t));

	if (NULL == game->faces[FACE_HAPPY]) {
		free(game->player);
		free(game);
		return NULL;
	}


	game->faces[FACE_ANGRY] = calloc(1U, sizeof(psprite_t));

	if (NULL == game->faces[FACE_ANGRY]) {
		free(game->faces[FACE_HAPPY]);
		free(game->player);
		free(game);
		return NULL;
	}


	game->faces[FACE_MAD] = calloc(1U, sizeof(psprite_t));

	if (NULL == game->faces[FACE_MAD]) {
		free(game->faces[FACE_ANGRY]);
		free(game->faces[FACE_HAPPY]);
		free(game->player);
		free(game);
		return NULL;
	}

	init_sprite(game->faces[FACE_HAPPY], 0.f, 10.f, 16U, 16U, bm_face_happy);
	init_sprite(game->faces[FACE_ANGRY], 20.f, 10.f, 16U, 16U, bm_face_angry);
	init_sprite(game->faces[FACE_MAD], 40.f, 10.f, 16U, 16U, bm_face_mad);
	game->face_count = 3;

	return game;
}

void initializeTimers(void) {
	TCCR0B &= ~(1U << WGM02);
	TCCR0B |= (1U << CS01) | (1U << CS00);
	TIMSK0 |= (1U << TOIE0);
	TCNT0 = 0U;

	TCCR1B &= ~(1U << WGM12);
	TCCR1B |= (1U << CS10);
	TIMSK1 |= (1U << TOIE1);
	TCNT1 = 0U;

	/*TCCR3B &= ~(1U << WGM32);
	TCCR3B |= (1U << CS30);
	TIMSK3 |= (1U << TOIE3);
	TCNT3 = 0U;*/
}

uint8_t timer0_ovfl = 0U;
ISR(TIMER0_OVF_vect) {
	cli();

	++timer0_ovfl;

	if (timer0_ovfl >= 24U) {
		debounceBtn0();
		debounceBtn1();
		timer0_ovfl = 0U;
		TCNT0 = 0U;
	}

	sei();
}

uint8_t timer1_ovfl = 0U;
ISR(TIMER1_OVF_vect) {
	TIMSK1 ^= (1U << TOIE1);

	++timer1_ovfl;

	if (timer1_ovfl >= 12U) {
		if (redraw && (NULL != game->current_level->draw)) {
			p_clear_screen();
			game->current_level->draw(game->current_level, game);
			show_screen();
		}
		
		if (NULL != game->current_level->update) {
			redraw = game->current_level->update(game->current_level, game);
		}

		btn0Changed = 0U;
		btn1Changed = 0U;
		timer1_ovfl = 0U;
		TCNT1 = 0U;
	}

	TIMSK1 ^= (1U << TOIE1);
}
