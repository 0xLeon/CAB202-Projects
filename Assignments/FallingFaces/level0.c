#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "buttons.h"
#include "pgraphics.h"
#include "game.h"

#include "level0.h"

static void level0_load(level_p self, game_p game);
static void level0_draw(level_p self, game_p game);
static uint8_t level0_update(level_p self, game_p game);
static void level0_unload(level_p self, game_p game);

static uint8_t chosen_level = 1U;
static uint8_t rand_seed = 0U;

level_p create_level0(game_p game) {
	level_p level0 = calloc(1U, sizeof(level_t));

	if (NULL == level0) {
		// TODO: display error
		return NULL;
	}

	level0->load = level0_load;
	level0->draw = level0_draw;
	level0->update = level0_update;
	level0->unload = level0_unload;

	return level0;
}

static void level0_load(level_p self, game_p game) {
	for (uint8_t i = 0U; i < game->face_count; ++i) {
		game->faces[i]->is_visible = 0U;
	}

	game->player->is_visible = 0U;
	game->active_face_count = 0U;

	rand_seed = 0U;
	chosen_level = 1U;
}

static void level0_draw(level_p self, game_p game) {
	p_draw_string(1U, 12U, "Stefan Hahn");
	p_draw_string(1U, 20U, "n9529977");
	p_draw_formatted_string(1U, 28U, "Level: %1d", chosen_level);
}

static uint8_t level0_update(level_p self, game_p game) {
	++rand_seed;

	if (btn1Changed && btn1Pressed) {
		chosen_level = 1U + (chosen_level % 3U);

		return 1U;
	}

	if (btn0Changed && !btn0Pressed) {
		if (NULL != game->current_level->unload) {
			game->current_level->unload(game->current_level, game);
		}

		game->current_level = game->levels[chosen_level];
		game->current_level->load(game->current_level, game);

		return 1U;
	}

	return 0U;
}

static void level0_unload(level_p self, game_p game) {
	srand(rand_seed);
}
