#include <stdlib.h>
#include <avr/io.h>
#include "buttons.h"
#include "lcd.h"
#include "level2.h"

static void level2_load(level_p self, game_p game);
static void level2_draw(level_p self, game_p game);
static uint8_t level2_update(level_p self, game_p game);
static void level2_unload(level_p self, game_p game);


level_p create_level2(game_p game) {
	level_p level2 = calloc(1U, sizeof(level_t));

	if (NULL == level2) {
		// TODO: display error
		return NULL;
	}

	level2->load = level2_load;
	level2->draw = level2_draw;
	level2->update = level2_update;
	level2->unload = level2_unload;

	return level2;
}

static void level2_load(level_p self, game_p game) {
	game->levels[1]->load(self, game);

	uint16_t adc_value = 0U;

	TIMSK0 &= ~(1U << TOIE0);

	ADMUX = (1U << REFS0);
	ADCSRA = (1U << ADPS2) | (1U << ADPS1) | (1U << ADPS0);
	ADCSRA |= (1U << ADEN);

	ADCSRA |= (1U << ADSC);
	while (ADCSRA & (1U << ADSC));
	adc_value = 1023U - ADC;

	game->player->x = (adc_value / 1023.f) * (LCD_X - game->player->width - 1U);
}

static void level2_draw(level_p self, game_p game) {
	game->levels[1U]->draw(self, game);
}

static uint8_t level2_update(level_p self, game_p game) {
	uint8_t didUpdate = 0U;
	uint8_t new_player_x = 0U;
	uint8_t delta_player_x = 0U;
	uint16_t adc_value = 0U;

	ADCSRA |= (1U << ADSC);
	while (ADCSRA & (1U << ADSC));
	adc_value = 1023U - ADC;

	new_player_x = (adc_value / 1023.f) * (LCD_X - game->player->width - 1U);
	delta_player_x = new_player_x - ((uint8_t) game->player->x);

	if (0U != GAME_ABSOLUTE(delta_player_x)) {
		didUpdate = 1U;
		game->player->x = new_player_x;
	}

	didUpdate = game->levels[1U]->update(self, game) || didUpdate;

	return didUpdate;
}

static void level2_unload(level_p self, game_p game) {
	ADCSRA &= ~(1U << ADEN);

	TIMSK0 |= (1U << TOIE0);
	TCNT0 = 0U;
}
