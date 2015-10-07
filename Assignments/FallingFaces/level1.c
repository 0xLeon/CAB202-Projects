#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"
#include "buttons.h"
#include "pgraphics.h"
#include "level1.h"

static void level1_load(level_p self, game_p game);
static void level1_draw(level_p self, game_p game);
static uint8_t level1_update(level_p self, game_p game);

void respawn_faces(game_p game);
static int rand_sort(const void *a, const void *b);


level_p create_level1(game_p game) {
	level_p level1 = calloc(1U, sizeof(level_t));

	if (NULL == level1) {
		// TODO: display error
		return NULL;
	}

	level1->load = level1_load;
	level1->draw = level1_draw;
	level1->update = level1_update;
	level1->unload = NULL;

	return level1;
}

static void level1_load(level_p self, game_p game) {
	respawn_faces(game);

	for (uint8_t i = 0; i < game->face_count; ++i) {
		game->faces[i]->dy = .4f;
	}

	game->player->x = (LCD_X - game->player->width) / 2U;
	game->player->y = 40.f;
	game->player->dx = 0.f;
	game->player->is_visible = 1U;
}

static void level1_draw(level_p self, game_p game) {
	p_draw_formatted_string(1U, 0U, "L: %1d, S: %2d", game->lives, game->score);

	p_draw_line(0U, 8U, 83U, 8U);

	for (uint8_t i = 0U; i < game->face_count; ++i) {
		p_draw_sprite(game->faces[i]);
	}

	p_draw_char(game->player->x, game->player->y, 'H');
}

static uint8_t level1_update(level_p self, game_p game) {
	uint8_t didUpdate = 0U;

	if (btn0Pressed) {
		if (game->player->dx > -1.5f) {
			game->player->dx -= .25f;
		}

		game->player->x += game->player->dx;
		didUpdate = 1U;
	}
	else if (!btn0Pressed && btn0Changed) {
		game->player->dx = 0.f;
	}

	if (btn1Pressed) {
		if (game->player->dx < 1.5f) {
			game->player->dx += .25f;
		}

		game->player->x += game->player->dx;
		
		didUpdate = 1U;
	}
	else if (!btn1Pressed && btn1Changed) {
		game->player->dx = 0.f;
	}

	if (didUpdate) {
		if (game->player->x < 0U) {
			game->player->x = 0.f;
		}
		else if (game->player->x >= (LCD_X - game->player->width)) {
			game->player->x = LCD_X - game->player->width;
		}
	}


	for (uint8_t i = 0U; i < game->face_count; ++i) {
		float tmp = game->faces[i]->y;
		game->faces[i]->y += game->faces[i]->dy;

		didUpdate = didUpdate || (tmp != game->faces[i]->y);
	}

	if (game->faces[0]->y > LCD_Y) {
		respawn_faces(game);
	}

	return didUpdate;
}

void respawn_faces(game_p game) {
	uint8_t face_ids[] = {0U, 1U, 2U};
	uint8_t max_xes[] = {15U, 47U, 68U};
	// uint8_t max_xes[] = {26U, 47U, 68U};

	qsort(face_ids, 3U, sizeof(uint8_t), rand_sort);

	uint8_t min_x = 0U;
	uint8_t max_x = 0U;
	uint8_t current_x = 0U;
	uint8_t current_face_id = 0U;

	for (uint8_t i = 0; i < 3U; ++i) {
		max_x = max_xes[i];
		current_face_id = face_ids[i];

		current_x = min_x + ((uint8_t) (rand() % (max_x - min_x + 1U)));

		game->faces[current_face_id]->x = current_x;
		game->faces[current_face_id]->y = 10.f;
		game->faces[current_face_id]->is_visible = 1U;

		min_x = current_x + game->faces[current_face_id]->width + 5U;
	}
}

static int rand_sort(const void *a, const void *b) {
	return (rand() % 2);
}
