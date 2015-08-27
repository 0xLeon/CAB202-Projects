#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "game.h"
#include "game_types.h"
#include "game_object.h"
#include "megamaniac_go_data.h"
#include "megamaniac_go_types.h"
#include "megamaniac.h"
#include "megamaniac_levels.h"

//-------------------------------------------------------
// Game Object Update Methods Forward Declarations
//-------------------------------------------------------

bool go_player_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_bullet_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_bomb_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_bomb_dropper_update(game_object_p self, game_update_p update, game_p game, game_level_p level);


//-------------------------------------------------------
// Game Object Creation Functions
//-------------------------------------------------------

game_object_p megamaniac_create_level_name(game_p megamaniac, char* level_name) {
	assert(NULL != megamaniac);
	assert(NULL != level_name);
	assert(strlen(level_name) > 0);

	int x = (megamaniac->screen_width - strlen(level_name)) / 2;
	int y = megamaniac->screen_height - 1;

	return create_static_string_game_object(GO_TYPE_LEVEL_NAME, x, y, 0., 0., 0L, level_name);
}

game_object_p megamaniac_create_lost_screen(game_p megamaniac) {
	assert(NULL != megamaniac);

	char* text = "             You lost!              Press 'r' to restart or 'q' to quit.";
	game_object_p go_lost_screen = create_static_string_game_object(GO_TYPE_LEVEL_NAME, 0., 0., 0., 0., 0L, text);

	if (NULL == go_lost_screen) {
		return NULL;
	}

	go_lost_screen->width = 36;
	go_lost_screen->height = 2;
	go_lost_screen->x = (megamaniac->screen_width - go_lost_screen->width) / 2;
	go_lost_screen->y = (megamaniac->screen_height - go_lost_screen->height) / 2;

	return go_lost_screen;
}

game_object_p megamaniac_create_pause_screen(game_p megamaniac) {
	assert(NULL != megamaniac);

	char* text = "               Paused               Press 'r' to restart or 'q' to quit.        Press 'p' to continue.      ";
	game_object_p go_pause_screen = create_static_string_game_object(GO_TYPE_PAUSE_SCREEN, 0., 0., 0., 0., 0L, text);

	if (NULL == go_pause_screen) {
		return NULL;
	}

	go_pause_screen->width = 36;
	go_pause_screen->height = 3;
	go_pause_screen->x = (megamaniac->screen_width - go_pause_screen->width) / 2;
	go_pause_screen->y = (megamaniac->screen_height - go_pause_screen->height) / 2;

	return go_pause_screen;
}

game_object_p megamaniac_create_player(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_object_p go_player = create_static_string_game_object(GO_TYPE_PLAYER, (megamaniac->screen_width) / 2, megamaniac->screen_height - 4, 0., 0., 0L, "$");

	if (NULL == go_player) {
		return NULL;
	}

	go_player->update = go_player_update;

	return go_player;
}

game_object_p megamaniac_create_bullet(game_p megamaniac, double x) {
	assert(NULL != megamaniac);
	// TODO: assertion for screen location

	game_object_p go_bullet = create_static_string_game_object(GO_TYPE_BULLET, x, megamaniac->screen_height - 5, 0., -1., 70L, "|");

	if (NULL == go_bullet) {
		return NULL;
	}

	go_bullet->update = go_bullet_update;

	return go_bullet;
}

game_object_p megamaniac_create_enemy(game_p megamaniac, double x, double y, int enemy_type, game_object_update_f enemy_update_f) {
	assert(NULL != megamaniac);
	// TODO: assertion for enemy_type
	// TODO: assertion for screen location

	game_object_p go_enemy = create_static_string_game_object(enemy_type, x, y, 1., 0., 0L, "@");

	if (NULL == go_enemy) {
		return NULL;
	}

	go_enemy->update = enemy_update_f;

	return go_enemy;
}

game_object_p megamaniac_create_enemy_mover(game_p megamaniac, long interval, game_object_update_f enemy_mover_update_f) {
	assert(NULL != megamaniac);
	assert(interval > -1L);
	assert(NULL != enemy_mover_update_f);

	game_object_p go_enemy_mover = create_game_object(GO_TYPE_ENEMY_MOVER, 0., 0., 0, 0, 0., 0., NULL, interval);

	if (NULL == go_enemy_mover) {
		return NULL;
	}

	go_enemy_mover->update = enemy_mover_update_f;

	return go_enemy_mover;
}

game_object_p megamaniac_create_bomb(game_p megamaniac, double x, double y) {
	assert(NULL != megamaniac);
	// TODO: assertion for screen location

	game_object_p go_bomb = create_static_string_game_object(GO_TYPE_BOMB, x, y, 0., 1., 200L, "*");

	if (NULL == go_bomb) {
		return NULL;
	}

	go_bomb->update = go_bomb_update;

	return go_bomb;
}

game_object_p megamaniac_create_bomb_dropper(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_object_p go_bomb_dropper = create_null_game_object(GO_TYPE_BOMB_DROPPER);

	if (NULL == go_bomb_dropper) {
		return NULL;
	}

	go_bomb_dropper->timer = game_create_timer(3000L);

	if (NULL == go_bomb_dropper->timer) {
		destroy_game_object(go_bomb_dropper);

		return NULL;
	}

	go_bomb_dropper->update = go_bomb_dropper_update;

	return go_bomb_dropper;
}

int megamaniac_create_standard_enemy_formation(game_level_p level, game_p megamaniac, int offset, int enemy_type, int row_count, int odd_count, int even_count, double horizontal_spacing, double vertical_spacing, game_object_update_f enemy_update_f) {
	assert(NULL != level);
	assert(NULL != megamaniac);
	assert(offset > -1);
	assert(offset < level->game_object_count);
	assert(megamaniac_type_is_enemy(enemy_type));
	assert(row_count > 0);
	assert(((odd_count > 0) || (even_count > 0)));
	assert(horizontal_spacing > 0);
	assert(vertical_spacing > 0);
	// TODO: assertion for offset, game object count, and proposed enemy count

	double enemy_x_odd = ((megamaniac->screen_width) / 2.) - ((horizontal_spacing + 1.) * (odd_count - 1.) / 2.);
	double enemy_x_even = ((megamaniac->screen_width) / 2.) - ((horizontal_spacing + 1.) * (even_count - 1.) / 2.);
	double enemy_y = 1.;
	int enemy_count = 0;

	for (int i = 1; i <= row_count; i++, enemy_y += vertical_spacing + 1.) {
		double x;
		int max;

		if ((i % 2) == 0) {
			x = enemy_x_even;
			max = even_count;
		}
		else {
			x = enemy_x_odd;
			max = odd_count;
		}

		for (int c = 0; c < max; c++, x += horizontal_spacing + 1.) {
			level->game_objects[offset] = megamaniac_create_enemy(megamaniac, x, enemy_y, enemy_type, enemy_update_f);

			if (NULL != level->game_objects[offset]) {
				offset++;
				enemy_count++;
			}
		}
	}

	return enemy_count;
}

//-------------------------------------------------------
// Game Object Update Methods
//-------------------------------------------------------

bool go_player_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	switch (update->key) {
		case 'a':
			if ((self->x - 1.) >= 0.) {
				self->x -= 1.;
			}

			return true;
		case 'd':
			if ((self->x + 1.) <= (game->screen_width - 1)) {
				self->x += 1.;
			}

			return true;
		case 's':
#ifndef GAME_DEBUG
			if (NULL != find_game_object_by_type(GO_TYPE_BULLET, game->current_level->game_objects, game->current_level->game_object_count, NULL)) {
				return false;
			}
#endif

			// TODO: error checks
			level_add_game_object(game->current_level, megamaniac_create_bullet(game, (int) round(self->x)));

			return true;
	}

	return false;
}

bool go_bullet_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	bool did_update = move_game_object(self);

	if (self->y < 0.) {
		// destroy bullet
		self->active = false;
		self->recycle = true;

		return true;
	}

	int enemy_counter = 0;
	for (int i = 0; i < game->current_level->game_object_count; i++) {
		if ((NULL != game->current_level->game_objects[i]) && megamaniac_game_object_is_enemy(game->current_level->game_objects[i]) && game->current_level->game_objects[i]->active) {
			enemy_counter++;

			if ((((int) round(self->x)) == ((int) round(game->current_level->game_objects[i]->x))) && (((int) round(self->y + 1)) == ((int) round(game->current_level->game_objects[i]->y)))) {
				// destroy enemy
				game->current_level->game_objects[i]->active = false;
				game->current_level->game_objects[i]->recycle = true;
				enemy_counter--;

				// TODO: error checking
				game_object_p go_score = find_game_object_by_type(GO_TYPE_SCORE, game->game_objects, game->game_object_count, NULL);
				go_additional_data_comparable_int_p go_score_data = (go_additional_data_comparable_int_p) go_score->additional_data;

				// destroy bullet
				self->active = false;
				self->recycle = true;

				go_score_data->current_value += POINTS_PER_ENEMY;

				if ((enemy_counter == 0) && (i < (game->current_level->game_object_count))) {
					for (int j = i + 1; j < game->current_level->game_object_count; j++) {
						if (megamaniac_game_object_is_enemy(game->current_level->game_objects[j]) && game->current_level->game_objects[j]->active) {
							enemy_counter++;
							break;
						}
					}
				}

				if (enemy_counter == 0) {
					go_score_data->current_value += 500;

					game_object_p go_player = find_game_object_by_type(GO_TYPE_PLAYER, game->current_level->game_objects, game->current_level->game_object_count, NULL);
					int player_current_x = (int) round(go_player->x);

					// TODO: find actual level or just assume current level?
					game->current_level->unload(game->current_level, game);
					game->current_level->load(game->current_level, game);

					go_player = find_game_object_by_type(GO_TYPE_PLAYER, game->current_level->game_objects, game->current_level->game_object_count, NULL);
					go_player->x = player_current_x;
				}

				return true;
			}
		}
	}

	return did_update;
}

bool go_bomb_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	bool didMove = move_game_object(self);
	game_object_p go_player = find_game_object_by_type(GO_TYPE_PLAYER, game->current_level->game_objects, game->current_level->game_object_count, NULL);

	if ((((int) round(self->x)) == ((int) round(go_player->x))) && (((int) round(self->y)) == ((int) round(go_player->y)))) {
		self->active = false;
		self->recycle = true;

		game_object_p go_lives = find_game_object_by_type(GO_TYPE_LIVES, game->game_objects, game->game_object_count, NULL);
		go_additional_data_comparable_int_p go_lives_data = (go_additional_data_comparable_int_p) go_lives->additional_data;

		go_lives_data->current_value--;

		if (go_lives_data->current_value < 1) {
			// TODO: error checking
			level_add_game_object(game->current_level, megamaniac_create_lost_screen(game));

			go_player->active = false;
			go_player->recycle = true;

			game->current_level->paused = true;
		}
		else {
			// TODO: error checking
			// level_add_game_object(level, level1_create_player(game));
			go_player->x = (game->screen_width) / 2;
		}

		for (int i = 0; i < game->current_level->game_object_count; i++) {
			if ((NULL != game->current_level->game_objects[i]) && game->current_level->game_objects[i]->active) {
				switch (game->current_level->game_objects[i]->type) {
					case GO_TYPE_BULLET:
					case GO_TYPE_BOMB:
						game->current_level->game_objects[i]->active = false;
						game->current_level->game_objects[i]->recycle = true;
						break;
				}
			}
		}

		return true;
	}

	if (((int) round(self->y)) >= (game->screen_height - 3)) {
		// destroy bomb
		self->active = false;
		self->recycle = true;

		return true;
	}

	return didMove;
}

bool go_bomb_dropper_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	int bomb_count = 0;
	int enemy_count = 0;
	int enemy_index = 0;

	for (int i = 0; i < game->current_level->game_object_count; i++) {
		if (NULL != game->current_level->game_objects[i]) {
			if (megamaniac_game_object_is_enemy(game->current_level->game_objects[i])) {
				enemy_count++;
				continue;
			}
			else if (game->current_level->game_objects[i]->type == GO_TYPE_BOMB) {
				bomb_count++;
				continue;
			}
		}
	}

	if ((bomb_count == 4) || (enemy_count == 0)) {
		return false;
	}

	enemy_index = rand() % (enemy_count + 1);

	for (int i = 0, c = 0; i < game->current_level->game_object_count; i++) {
		if ((NULL != game->current_level->game_objects[i]) && megamaniac_game_object_is_enemy(game->current_level->game_objects[i])) {
			if (c == enemy_index) {
				// TODO: error checks
				level_add_game_object(game->current_level, megamaniac_create_bomb(game, (int) round(game->current_level->game_objects[i]->x), (int) round(game->current_level->game_objects[i]->y + 1)));

				return true;
			}
			else {
				c++;
			}
		}
	}

	return false;
}


//-------------------------------------------------------
// Helper Functions
//-------------------------------------------------------

bool megamaniac_game_object_is_enemy(game_object_p game_object) {
	if (NULL == game_object) {
		return false;
	}

	return megamaniac_type_is_enemy(game_object->type);
}

bool megamaniac_type_is_enemy(int game_object_type) {
	assert(game_object_type > GO_TYPE_NONE);

	switch (game_object_type) {
		case GO_TYPE_ENEMY1:
		case GO_TYPE_ENEMY2:
		case GO_TYPE_ENEMY3:
		case GO_TYPE_ENEMY4:
		case GO_TYPE_ENEMY5:
			return true;
		default:
			return false;
	}
}
