#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "usb_serial.h"
#include "lcd.h"
#include "pgraphics.h"
#include "game.h"
#include "fallingfaces.h"
#include "level3.h"

#define SPEED_MODE_SLOW		0U
#define SPEED_MODE_MED		1U
#define SPEED_MODE_FAST		2U

static void level3_load(level_p self, game_p game);
static uint8_t level3_update(level_p self, game_p game);
static void level3_draw(level_p self, game_p game);
static void level3_unload(level_p self, game_p game);

static void spawn_player(game_p game);
static void spawn_faces(game_p game);
static uint8_t find_valid_face_location(psprite_p face, game_p game);
static void randomize_travel_direction(psprite_p sprite, double speed);
static void level3_collision_face_mad(game_p game);

const static double speed_mode_speeds[] = {.4, .8, 1.2};

static uint8_t speed_mode = SPEED_MODE_SLOW;

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
	speed_mode = SPEED_MODE_SLOW;

	for (uint8_t i = 0U; i < game->face_count; ++i) {
		game->faces[i]->is_visible = 0U;
		game->faces[i]->dx = 0.f;
		game->faces[i]->dy = 0.f;
	}

	game->face_collision_handlers[FACE_MAD] = level3_collision_face_mad;

	game->active_face_count = 0U;

	spawn_player(game);
	spawn_faces(game);

	game->player->dx = 2.f;
	game->player->dy = 2.f;

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
	uint8_t didUpdate = 0U;
	int16_t c = 0U;

	c = usb_serial_getchar();

	if (c > 0U) {
		float player_current_x = game->player->x;
		float player_current_y = game->player->y;

		switch (c) {
			case 'a':
				game->player->x -= game->player->dx;

				if (game->player->x <= 0U) {
					game->player->x = 0U;
				}

				break;
			case 'd':
				game->player->x += game->player->dx;

				if (game->player->x >= (LCD_X - game->player->width)) {
					game->player->x = LCD_X - game->player->width;
				}

				break;
			case 'w':
				game->player->y -= game->player->dy;

				if (game->player->y <= 9U) {
					game->player->y = 9U;
				}

				break;
			case 's':
				game->player->y += game->player->dy;

				if (game->player->y >= (LCD_Y - game->player->height)) {
					game->player->y = LCD_Y - game->player->height;
				}

				break;
		}

		didUpdate = (player_current_x != game->player->x) || (player_current_y != game->player->y);

		if (didUpdate) {
			check_face_player_collision(game);
		}
	}

	if (game->active_face_count < game->face_count) {
		spawn_faces(game);
	}

	uint8_t old_x = 0U;
	uint8_t old_y = 0U;
	uint8_t face_didUpdate = 0U;

	for (uint8_t i = 0U; i < game->face_count; ++i) {
		if (game->faces[i]->is_visible) {
			old_x = (uint8_t) game->faces[i]->x;
			old_y = (uint8_t) game->faces[i]->y;

			game->faces[i]->x += game->faces[i]->dx;
			game->faces[i]->y += game->faces[i]->dy;

			if (game->faces[i]->x < 0U) {
				game->faces[i]->x = 0.f;

				game->faces[i]->dx *= -1;
				game->faces[i]->x += game->faces[i]->dx;
			}
			else if (game->faces[i]->x >= (LCD_X - game->faces[i]->width)) {
				game->faces[i]->x = LCD_X - game->faces[i]->width - 1U;

				game->faces[i]->dx *= -1;
				game->faces[i]->x += game->faces[i]->dx;
			}

			if (game->faces[i]->y < 9U) {
				game->faces[i]->y = 9.f;

				game->faces[i]->dy *= -1;
				game->faces[i]->y += game->faces[i]->dy;
			}
			else if (game->faces[i]->y >= (LCD_Y - game->faces[i]->height)) {
				game->faces[i]->y = LCD_Y - game->faces[i]->height - 1U;

				game->faces[i]->dy *= -1;
				game->faces[i]->y += game->faces[i]->dy;
			}

			for (uint8_t j = 0U; j < game->face_count; ++j) {
				if ((game->faces[i] != game->faces[j]) && game->faces[j]->is_visible && sprites_collided(game->faces[i], game->faces[j])) {
					double delta_x = game->faces[j]->x - game->faces[i]->x;
					double delta_y = game->faces[i]->y - game->faces[j]->y;

					double angle = (atan2(delta_y, delta_x) * 180.) / M_PI;

					if (angle < 0U) {
						angle += 360U;
					}

					double angle_m45 = angle - 45.;
					double angle_m135 = angle - 135.;
					double angle_m225 = angle - 225.;
					double angle_m315 = angle - 315.;

					if ((GAME_ABSOLUTE(angle_m45) < .01) || (GAME_ABSOLUTE(angle_m135) < .01) || (GAME_ABSOLUTE(angle_m225) < .01) || (GAME_ABSOLUTE(angle_m315) < .01)) {
						game->faces[i]->x -= game->faces[i]->dx;
						game->faces[i]->y -= game->faces[i]->dy;
						
						game->faces[i]->dx *= -1;
						game->faces[i]->dy *= -1;

						game->faces[i]->x += game->faces[i]->dx;
						game->faces[i]->y += game->faces[i]->dy;

						game->faces[j]->dx *= -1;
						game->faces[j]->dy *= -1;
					}
					else if (((angle > 45.) && (angle < 135.)) || ((angle > 225.) && (angle < 315.))) {
						// top or bottom
						game->faces[i]->y -= game->faces[i]->dy;
						game->faces[i]->dy *= -1;
						game->faces[i]->y += game->faces[i]->dy;

						game->faces[j]->dy *= -1;
					}
					else {
						// left or right
						game->faces[i]->x -= game->faces[i]->dx;
						game->faces[i]->dx *= -1;
						game->faces[i]->x += game->faces[i]->dx;

						game->faces[j]->dx *= -1;
					}

					break;
				}
			}

			face_didUpdate = (old_x != ((uint8_t) game->faces[i]->x)) || (old_y != ((uint8_t) game->faces[i]->y));

			if (face_didUpdate) {
				didUpdate = 1U;

				if (sprites_collided(game->faces[i], game->player)) {
					game->faces[i]->is_visible = 0U;
					game->face_collision_handlers[i](game);
					--(game->active_face_count);
				}
			}
		}
	}

	return didUpdate;
}

static void level3_draw(level_p self, game_p game) {
	game->levels[1U]->draw(self, game);
}

static void level3_unload(level_p self, game_p game) {
	game->face_collision_handlers[FACE_MAD] = default_collision_face_mad;
}

static void spawn_player(game_p game) {
	game->player->x = rand() % (LCD_X - game->player->width);
	game->player->y = 9U + (rand() % (LCD_Y - 9U - game->player->height));
}

static void spawn_faces(game_p game) {
	for (uint8_t i = 0U; (i < game->face_count) && (game->active_face_count < game->face_count); ++i) {
		if (!game->faces[i]->is_visible) {
			if (find_valid_face_location(game->faces[i], game)) {
				randomize_travel_direction(game->faces[i], speed_mode_speeds[speed_mode]);
				game->faces[i]->is_visible = 1U;
				++(game->active_face_count);
			}
		}
	}
}

static uint8_t find_valid_face_location(psprite_p test_face, game_p game) {
	psprite_t test_face_padded;
	psprite_p test_face_padded_ptr = &test_face_padded;

	test_face_padded_ptr->width = test_face->width + 10U;
	test_face_padded_ptr->height = test_face->height + 10U;

	uint8_t found_valid_location = 0U;
	uint8_t didCollide = 0U;

	for (uint8_t c = 0U, c_max = 5U; (c < c_max) && !found_valid_location; ++c) {
		test_face->x = rand() % (LCD_X - test_face->width);
		test_face->y = 9U + (rand() % (LCD_Y - 9U - test_face->height));
		test_face_padded_ptr->x = test_face->x - 5U;
		test_face_padded_ptr->y = test_face->y - 5U;
		didCollide = 0U;

		if (sprites_collided(test_face_padded_ptr, game->player)) {
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

static void level3_collision_face_mad(game_p game) {
	if (speed_mode >= 2U) {
		return;
	}

	++speed_mode;

	for (uint8_t i = 0U; i < game->face_count; ++i) {
		if (game->faces[i]->is_visible) {
			double angle = atan2(game->faces[i]->dy, game->faces[i]->dx);

			game->faces[i]->dx = ((float) GAME_SIGNUM(game->faces[i]->dx) * sin(angle) * speed_mode_speeds[speed_mode]);
			game->faces[i]->dy = ((float) GAME_SIGNUM(game->faces[i]->dy) * sin(angle) * speed_mode_speeds[speed_mode]);
		}
	}
}
