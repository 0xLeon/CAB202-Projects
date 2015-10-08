#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "buttons.h"
#include "hardware.h"
#include "graphics.h"
#include "pgraphics.h"
#include "game.h"
#include "fallingfaces.h"

#include "level0.h"
#include "level1.h"

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
	game->ended = 0U;

	game->player = calloc(1U, sizeof(psprite_t));

	if (NULL == game->player) {
		free(game);
		return NULL;
	}

	p_init_sprite(game->player, 0.f, 0.f, 5U, 7U, NULL);


	game->faces[FACE_HAPPY] = calloc(1U, sizeof(psprite_t));
	game->face_collision_handlers[FACE_HAPPY] = default_collision_face_happy;

	if (NULL == game->faces[FACE_HAPPY]) {
		free(game->player);
		free(game);
		return NULL;
	}


	game->faces[FACE_ANGRY] = calloc(1U, sizeof(psprite_t));
	game->face_collision_handlers[FACE_ANGRY] = default_collision_face_angry;

	if (NULL == game->faces[FACE_ANGRY]) {
		free(game->faces[FACE_HAPPY]);
		free(game->player);
		free(game);
		return NULL;
	}


	game->faces[FACE_MAD] = calloc(1U, sizeof(psprite_t));
	game->face_collision_handlers[FACE_MAD] = default_collision_face_mad;

	if (NULL == game->faces[FACE_MAD]) {
		free(game->faces[FACE_ANGRY]);
		free(game->faces[FACE_HAPPY]);
		free(game->player);
		free(game);
		return NULL;
	}

	p_init_sprite(game->faces[FACE_HAPPY], 0.f, 0.f, 16U, 16U, bm_face_happy);
	p_init_sprite(game->faces[FACE_ANGRY], 0.f, 0.f, 16U, 16U, bm_face_angry);
	p_init_sprite(game->faces[FACE_MAD], 0.f, 0.f, 16U, 16U, bm_face_mad);
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
		debounceDpadLeft();
		debounceDpadRight();
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
		if (game->ended) {
			cli();

			char *message;
			
			if (game->score >= 20U) {
				message = "You Won!";
			}
			else if (0U == game->lives) {
				message = "You Lost!";
			}
			else {
				message = "Unexpected End!";
			}

			uint8_t i = 5U;
			do {
				p_clear_screen();
				p_draw_string(1U, 16U, message);
				p_draw_formatted_string(1U, 24U, "Restarting in %1d", i);
				show_screen();
				_delay_ms(1000.);
				--i;
			}
			while (i > 0);

			if (NULL != game->current_level->unload) {
				game->current_level->unload(game->current_level, game);
			}

			game->ended = 0U;
			game->lives = 3U;
			game->score = 0U;

			game->current_level = game->levels[0U];
			game->current_level->load(game->current_level, game);

			TCNT0 = 0U;
			TCNT1 = 0U;
			timer0_ovfl = 0U;
			timer1_ovfl = 0U;
			cli();

			redraw = 1U;
		}

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
		dpadLeftChanged = 0U;
		dpadRightChanged = 0U;
		timer1_ovfl = 0U;
		TCNT1 = 0U;
	}

	TIMSK1 ^= (1U << TOIE1);
}
