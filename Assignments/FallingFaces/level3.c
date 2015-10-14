#include <stdlib.h>
#include <math.h>
#include "usb_serial.h"
#include "lcd.h"
#include "pgraphics.h"
#include "game.h"
#include "level3.h"

static void level3_load(level_p self, game_p game);
static uint8_t level3_update(level_p self, game_p game);
static void level3_draw(level_p self, game_p game);
static void level3_unload(level_p self, game_p game);

static void spawn_player(game_p game);
static void spawn_faces(game_p game);
static uint8_t find_valid_face_location(psprite_p face, game_p game);
static void randomize_travel_direction(psprite_p sprite, double speed);

level_p create_level3(game_p game) {
	level_p level3 = calloc(1U, sizeof(level_t));

	if (NULL == level3) {
		// TODO: display error
		return NULL;
	}

	level3->load = level3_load;
	level3->draw = level3_draw;
	level3->update = level3_update;
	level3->unload = level3_unload;

	return level3;
}

static void level3_load(level_p self, game_p game) {
	for (uint8_t i = 0U; i < game->face_count; ++i) {
		game->faces[i]->is_visible = 0U;
		game->faces[i]->dx = 0.f;
		game->faces[i]->dy = 0.f;
	}

	game->active_face_count = 0U;

	spawn_player(game);
	spawn_faces(game);

	p_clear_screen();

	p_draw_string(14U, 15U, "Waiting for");
	p_draw_string(0U, 25U, "USB connection...");

	p_show_screen();

	usb_init();

	while (!usb_configured() || !usb_serial_get_control());

	usb_serial_flush_input();
	usb_serial_flush_output();
}

static uint8_t level3_update(level_p self, game_p game) {
	return 0U;
}

static void level3_draw(level_p self, game_p game) {
	game->levels[1U]->draw(self, game);
}

static void level3_unload(level_p self, game_p game) {
	return;
}

static void spawn_player(game_p game) {
	game->player->x = rand() % (LCD_X - game->player->width);
	game->player->y = 9U + (rand() % (LCD_Y - 9U - game->player->height));
}

static void spawn_faces(game_p game) {
	for (uint8_t i = 0U; (i < game->face_count) && (game->active_face_count < game->face_count); ++i) {
		if (!game->faces[i]->is_visible) {
			if (find_valid_face_location(game->faces[i], game)) {
				randomize_travel_direction(game->faces[i], .4);
				game->faces[i]->is_visible = 1U;
				++(game->active_face_count);
			}
		}
	}
}

static uint8_t find_valid_face_location(psprite_p test_face, game_p game) {
	psprite_t test_face_padded;
	psprite_p test_face_padded_ptr = &test_face_padded;

	test_face_padded_ptr->width = test_face->width + 5U;
	test_face_padded_ptr->height = test_face->height + 5U;

	uint8_t found_valid_location = 0U;
	uint8_t didCollide = 0U;

	for (uint8_t c = 0U, c_max = 5U; (c < c_max) && !found_valid_location; ++c) {
		test_face->x = rand() % (LCD_X - test_face->width);
		test_face->y = 9U + (rand() % (LCD_Y - 9U - test_face->height));
		test_face_padded_ptr->x = test_face->x - 5U;
		test_face_padded_ptr->y = test_face->y - 5U;
		didCollide = 0U;

		if (sprites_collided(test_face, game->player)) {
			continue;
		}

		for (uint8_t i = 0; (i < game->face_count) && !didCollide; ++i) {
			if ((test_face != game->faces[i]) && game->faces[i]->is_visible) {
				didCollide = sprites_collided(test_face_padded_ptr, game->faces[i]);
			}
		}

		found_valid_location = !didCollide;
	}

	return found_valid_location;
}

static void randomize_travel_direction(psprite_p sprite, double speed) {
	double angle = ((rand() % 360U) / 180.) * M_PI;
	double direction = (0U == (rand() % 2U)) ? 1. : -1.;

	speed *= direction;

	sprite->dx = ((float) (sin(angle) * speed));
	sprite->dy = ((float) (cos(angle) * speed));
}
