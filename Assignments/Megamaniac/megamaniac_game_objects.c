#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "game_types.h"
#include "game.h"
#include "game_object.h"
#include "megamaniac_go_data.h"
#include "megamaniac_go_types.h"
#include "megamaniac_game_objects.h"
#include "megamaniac.h"

//-------------------------------------------------------
// Game Object Update Methods Forward Declarations
//-------------------------------------------------------

bool go_quit_checker_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_restarter_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_pauser_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_level_changer_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_score_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_lives_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_score_cheater_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_player_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_bullet_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_bomb_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_bomb_dropper_update(game_object_p self, game_update_p update, game_p game, game_level_p level);


//-------------------------------------------------------
// Game Object Creation Functions
//-------------------------------------------------------

game_object_p megamaniac_create_go_quit_checker(game_p megamaniac) {
	assert(NULL != megamaniac);

	return create_null_game_object(GO_TYPE_QUIT, 0L, go_quit_checker_update);
}

game_object_p megamaniac_create_go_restarter(game_p megamaniac) {
	assert(NULL != megamaniac);

	return create_null_game_object(GO_TYPE_RESTART, 0L, go_restarter_update);
}

game_object_p megamaniac_create_go_pauser(game_p megamaniac) {
	assert(NULL != megamaniac);

	return create_null_game_object(GO_TYPE_PAUSER, 0L, go_pauser_update);
}

game_object_p megamaniac_create_go_level_changer(game_p megamaniac) {
	assert(NULL != megamaniac);

	return create_null_game_object(GO_TYPE_LEVEL_CHANGER, 0L, go_level_changer_update);
}

game_object_p megamaniac_create_go_line(game_p megamaniac) {
	assert(NULL != megamaniac);

	char* buf = malloc(megamaniac->screen_width * sizeof(char));

	if (NULL == buf) {
		return NULL;
	}

	memset(buf, '-', megamaniac->screen_width);

	return create_game_object(GO_TYPE_LINE, 0., megamaniac->screen_height - 3, megamaniac->screen_width, 1, 0., 0., buf, 0, NULL);
}

game_object_p megamaniac_create_go_credits(game_p megamaniac, char* credits) {
	assert(NULL != megamaniac);
	assert(NULL != credits);
	assert(strlen(credits) > 0);

	return create_static_string_game_object(GO_TYPE_CREDITS, 1., megamaniac->screen_height - 2, 0., 0., 0L, NULL, credits);
}

game_object_p megamaniac_create_go_score(game_p megamaniac, int initial_score) {
	assert(NULL != megamaniac);

	int buffer_size = 12;
	game_object_p go_score = create_dynamic_string_game_object(GO_TYPE_SCORE, megamaniac->screen_width - buffer_size, megamaniac->screen_height - 2, 0., 0., 0L, go_score_update, buffer_size, "Score:% 5d", ((initial_score > 9999) ? 9999 : initial_score));
	go_additional_data_comparable_int_p go_score_data = malloc(sizeof(go_additional_data_comparable_int_t));

	if (NULL == go_score) {
		return NULL;
	}

	if (NULL == go_score_data) {
		destroy_game_object(go_score);

		return NULL;
	}

	go_score_data->initial_value = initial_score;
	go_score_data->previous_value = initial_score;
	go_score_data->current_value = initial_score;
	go_score->additional_data = (void*) go_score_data;

	return go_score;
}

game_object_p megamaniac_create_go_lives(game_p megamaniac, int initial_lives) {
	assert(NULL != megamaniac);
	assert(initial_lives > -1);

	int buffer_size = 12;
	game_object_p go_lives = create_dynamic_string_game_object(GO_TYPE_LIVES, megamaniac->screen_width - buffer_size, megamaniac->screen_height - 1, 0., 0., 0L, go_lives_update, buffer_size, "Lives:% 5d", ((initial_lives > 9999) ? 9999 : initial_lives));
	go_additional_data_comparable_int_p go_lives_data = malloc(sizeof(go_additional_data_comparable_int_t));

	if (NULL == go_lives) {
		return NULL;
	}

	if (NULL == go_lives_data) {
		destroy_game_object(go_lives);

		return NULL;
	}

	go_lives_data->initial_value = initial_lives;
	go_lives_data->current_value = initial_lives;
	go_lives_data->previous_value = initial_lives;
	go_lives->additional_data = (void*) go_lives_data;

	return go_lives;
}

game_object_p megamaniac_create_go_score_cheater(game_p megamaniac) {
	assert(NULL != megamaniac);

	return create_null_game_object(GO_TYPE_SCORE_CHEATER, 0L, go_score_cheater_update);
}

game_object_p megamaniac_create_go_level_name(game_p megamaniac, char* level_name) {
	assert(NULL != megamaniac);
	assert(NULL != level_name);
	assert(strlen(level_name) > 0);

	int x = (megamaniac->screen_width - strlen(level_name)) / 2;
	int y = megamaniac->screen_height - 1;

	return create_static_string_game_object(GO_TYPE_LEVEL_NAME, x, y, 0., 0., 0L, NULL, level_name);
}

game_object_p megamaniac_create_go_lost_screen(game_p megamaniac) {
	assert(NULL != megamaniac);

	char* text = "             You lost!              Press 'r' to restart or 'q' to quit.";
	game_object_p go_lost_screen = create_static_string_game_object(GO_TYPE_LOST_SCREEN, 0., 0., 0., 0., 0L, NULL, text);

	if (NULL == go_lost_screen) {
		return NULL;
	}

	go_lost_screen->width = 36;
	go_lost_screen->height = 2;
	go_lost_screen->x = (megamaniac->screen_width - go_lost_screen->width) / 2;
	go_lost_screen->y = (megamaniac->screen_height - go_lost_screen->height) / 2;

	return go_lost_screen;
}

game_object_p megamaniac_create_go_pause_screen(game_p megamaniac) {
	assert(NULL != megamaniac);

	char* text = "               Paused               Press 'r' to restart or 'q' to quit.        Press 'p' to continue.      ";
	game_object_p go_pause_screen = create_static_string_game_object(GO_TYPE_PAUSE_SCREEN, 0., 0., 0., 0., 0L, NULL, text);

	if (NULL == go_pause_screen) {
		return NULL;
	}

	go_pause_screen->width = 36;
	go_pause_screen->height = 3;
	go_pause_screen->x = (megamaniac->screen_width - go_pause_screen->width) / 2;
	go_pause_screen->y = (megamaniac->screen_height - go_pause_screen->height) / 2;

	return go_pause_screen;
}

game_object_p megamaniac_create_go_player(game_p megamaniac) {
	assert(NULL != megamaniac);

	return create_static_string_game_object(GO_TYPE_PLAYER, (megamaniac->screen_width) / 2, megamaniac->screen_height - 4, 1., 0., 0L, go_player_update, "$");
}

game_object_p megamaniac_create_go_bullet(game_p megamaniac, double x) {
	assert(NULL != megamaniac);
	// TODO: assertion for screen location

	return create_static_string_game_object(GO_TYPE_BULLET, x, megamaniac->screen_height - 5, 0., -1., 70L, go_bullet_update, "|");
}

game_object_p megamaniac_create_go_enemy(game_p megamaniac, double x, double y, int enemy_type, game_object_update_f enemy_update_f) {
	assert(NULL != megamaniac);
	// TODO: assertion for enemy_type
	// TODO: assertion for screen location

	return create_static_string_game_object(enemy_type, x, y, 1., 0., 0L, enemy_update_f, "@");
}

game_object_p megamaniac_create_go_enemy_mover(game_p megamaniac, long interval, game_object_update_f enemy_mover_update_f) {
	assert(NULL != megamaniac);
	assert(interval > -1L);
	assert(NULL != enemy_mover_update_f);

	return create_game_object(GO_TYPE_ENEMY_MOVER, 0., 0., 0, 0, 0., 0., NULL, interval, enemy_mover_update_f);
}

game_object_p megamaniac_create_go_bomb(game_p megamaniac, double x, double y) {
	assert(NULL != megamaniac);
	// TODO: assertion for screen location

	return create_static_string_game_object(GO_TYPE_BOMB, x, y, 0., 1., 200L, go_bomb_update, "*");
}

game_object_p megamaniac_create_go_bomb_dropper(game_p megamaniac) {
	assert(NULL != megamaniac);

	return create_null_game_object(GO_TYPE_BOMB_DROPPER, 3000L, go_bomb_dropper_update);;
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
			level->game_objects[offset] = megamaniac_create_go_enemy(megamaniac, x, enemy_y, enemy_type, enemy_update_f);

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

bool go_quit_checker_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (update->key == 'q') {
		game->running = false;
	}

	return false;
}

bool go_restarter_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (update->key == 'r') {
		if ((NULL == game->current_level) || (NULL == game->current_level->load) || (NULL == game->current_level->unload)) {
			return false;
		}

		// TODO: error checking
		game_object_p go_lives = find_game_object_by_type(GO_TYPE_LIVES, game->game_objects, game->game_object_count, NULL);
		go_additional_data_comparable_int_p go_lives_data = (go_additional_data_comparable_int_p) go_lives->additional_data;

		// TODO: error checking
		game_object_p go_score = find_game_object_by_type(GO_TYPE_SCORE, game->game_objects, game->game_object_count, NULL);
		go_additional_data_comparable_int_p go_score_data = (go_additional_data_comparable_int_p) go_score->additional_data;

		if (go_lives_data->current_value < 1) {
			go_lives_data->current_value = go_lives_data->initial_value;
		}

		// TODO: reset score only after all lives lost
		go_score_data->current_value = go_score_data->initial_value;

		game->current_level->unload(game->current_level, game);
		game->current_level->load(game->current_level, game);

		return true;
	}

	return false;
}

bool go_pauser_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	bool didUpdate = false;

	if (update->key == 'p') {
		if (NULL != game->current_level) {
			if (!game->current_level->paused) {
				level_add_game_object(game->current_level, megamaniac_create_go_pause_screen(game));
			}
			else {
				game_object_p go_lost_screen = find_game_object_by_type(GO_TYPE_PAUSE_SCREEN, game->current_level->game_objects, game->current_level->game_object_count, NULL);
				go_lost_screen->active = false;
				go_lost_screen->recycle = true;
			}

			game->current_level->paused = !(game->current_level->paused);
			didUpdate = true;
		}
	}

	return didUpdate;
}

bool go_level_changer_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if ((update->key == 'l') && !(game->current_level->paused)) {

		int current_level_index = -1;
		game_level_p next_level = NULL;

		for (int i = 0; i < game->level_count; i++) {
			if (game->levels[i] == game->current_level) {
				current_level_index = i;
				break;
			}
		}

		next_level = game->levels[((current_level_index + 1) % game->level_count)];

		for (int i = current_level_index + 1; next_level != game->current_level; i = (i + 1) % game->level_count) {
			if (NULL != next_level) {
				game_object_p go_player = NULL;
				double go_player_x = 0.;
				double go_player_y = 0.;

				go_player = find_game_object_by_type(GO_TYPE_PLAYER, game->current_level->game_objects, game->current_level->game_object_count, NULL);

				if (NULL != go_player) {
					go_player_x = go_player->x;
					go_player_y = go_player->y;
				}

				game->current_level->unload(game->current_level, game);
				game->current_level = next_level;
				game->current_level->load(game->current_level, game);

				if (NULL != go_player) {
					go_player = find_game_object_by_type(GO_TYPE_PLAYER, game->current_level->game_objects, game->current_level->game_object_count, NULL);
					go_player->x = go_player_x;
					go_player->y = go_player_y;
				}

				return true;
			}

			next_level = game->levels[((i + 1) % game->level_count)];
		}
	}

	return false;
}

bool go_score_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	go_additional_data_comparable_int_p go_score_data = (go_additional_data_comparable_int_p) self->additional_data;

	if (go_score_data->current_value != go_score_data->previous_value) {
		sprintf(self->bitmap, "Score:% 5d", ((go_score_data->current_value > 9999) ? 9999 : go_score_data->current_value));
		go_score_data->previous_value = go_score_data->current_value;

		return true;
	}

	return false;
}

bool go_lives_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	go_additional_data_comparable_int_p go_lives_data = (go_additional_data_comparable_int_p) self->additional_data;;

	bool didUpdate = false;

	if (go_lives_data->current_value != go_lives_data->previous_value) {
		sprintf(self->bitmap, "Lives:% 5d", ((go_lives_data->current_value > 9999) ? 9999 : go_lives_data->current_value));
		go_lives_data->previous_value = go_lives_data->current_value;

		if (go_lives_data->current_value < 1) {
			// TODO: error checking
			level_add_game_object(game->current_level, megamaniac_create_go_lost_screen(game));

			game_object_p go_player = find_game_object_by_type(GO_TYPE_PLAYER, game->current_level->game_objects, game->current_level->game_object_count, NULL);
			go_player->active = false;
			go_player->recycle = true;

			game->current_level->paused = true;
		}

		didUpdate = true;
	}

	return didUpdate;
}

bool go_score_cheater_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	game_object_p go_score = find_game_object_by_type(GO_TYPE_SCORE, game->game_objects, game->game_object_count, NULL);

	if ((NULL != go_score) && (update->key == 'f')) {
		((go_additional_data_comparable_int_p) go_score->additional_data)->current_value += 50;
	}

	return false;
}

bool go_player_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	bool didMove = false;

	switch (update->key) {
		case 'a':
			if ((self->x - 1.) >= 0.) {
				self->x -= self->dx;
				didMove = true;
			}

			break;
		case 'd':
			if ((self->x + 1.) <= (game->screen_width - 1)) {
				self->x += self->dx;
				didMove = true;
			}

			break;
		case 's':
#ifndef GAME_DEBUG
			if (NULL != find_game_object_by_type(GO_TYPE_BULLET, game->current_level->game_objects, game->current_level->game_object_count, NULL)) {
				return false;
			}
#endif

			// TODO: error checks
			level_add_game_object(game->current_level, megamaniac_create_go_bullet(game, (int) round(self->x)));

			return true;
	}

	if (didMove) {
		int player_x = (int) round(self->x);
		int player_y = (int) round(self->y);
		int enemy_x = 0;
		int enemy_y = 0;

		for (int i = 0; i < game->current_level->game_object_count; i++) {
			if ((NULL != game->current_level->game_objects[i]) && game->current_level->game_objects[i]->active && megamaniac_go_is_enemy(game->current_level->game_objects[i])) {
				enemy_x = (int) round(game->current_level->game_objects[i]->x);
				enemy_x = (int) round(game->current_level->game_objects[i]->y);

				if ((player_x == enemy_x) && (player_y == enemy_y)) {
					game->current_level->game_objects[i]->active = false;
					game->current_level->game_objects[i]->recycle = true;

					game_object_p go_lives = find_game_object_by_type(GO_TYPE_LIVES, game->game_objects, game->game_object_count, NULL);
					go_additional_data_comparable_int_p go_lives_data = (go_additional_data_comparable_int_p) go_lives->additional_data;

					go_lives_data->current_value--;

					if (!megamaniac_binary_find_save_player_location(game, self, game->current_level->game_objects, game->current_level->game_object_count, 0, game->screen_width)) {
						self->x = (game->screen_width) / 2;
					}

					megamaniac_wipe_projectiles(game->current_level->game_objects, game->current_level->game_object_count);

					break;
				}
			}
		}
	}

	return didMove;
}

bool go_bullet_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	bool did_update = move_game_object(self);

	if (did_update) {
		if (self->y < 0.) {
			// destroy bullet
			self->active = false;
			self->recycle = true;

			return true;
		}

		int enemy_counter = 0;
		for (int i = 0; i < game->current_level->game_object_count; i++) {
			if ((NULL != game->current_level->game_objects[i]) && megamaniac_go_is_enemy(game->current_level->game_objects[i]) && game->current_level->game_objects[i]->active) {
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
							if (megamaniac_go_is_enemy(game->current_level->game_objects[j]) && game->current_level->game_objects[j]->active) {
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

					did_update = true;
					break;
				}
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
		
		if (!megamaniac_binary_find_save_player_location(game, go_player, game->current_level->game_objects, game->current_level->game_object_count, 0, game->screen_width)) {
			go_player->x = (game->screen_width) / 2;
		}

		megamaniac_wipe_projectiles(game->current_level->game_objects, game->current_level->game_object_count);

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
			if (megamaniac_go_is_enemy(game->current_level->game_objects[i])) {
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
		if ((NULL != game->current_level->game_objects[i]) && megamaniac_go_is_enemy(game->current_level->game_objects[i])) {
			if (c == enemy_index) {
				// TODO: error checks
				level_add_game_object(game->current_level, megamaniac_create_go_bomb(game, (int) round(game->current_level->game_objects[i]->x), (int) round(game->current_level->game_objects[i]->y + 1)));

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

bool megamaniac_go_is_enemy(game_object_p game_object) {
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

// TODO: make more generic by inputting boundary box object instead of game object with hardcoded limits
bool megamaniac_move_enemy(game_object_p go_enemy, game_p megamaniac) {
	assert(NULL != go_enemy);

	int before_x = (int) round(go_enemy->x);
	int before_y = (int) round(go_enemy->y);
	int after_x = 0;
	int after_y = 0;

	go_enemy->x += go_enemy->dx;
	go_enemy->y += go_enemy->dy;
	after_x = (int) round(go_enemy->x);
	after_y = (int) round(go_enemy->y);

	// TODO: check the if statement, enemies wrap to late
	// if (after_x != before_x) {
		if (after_x >= megamaniac->screen_width) {
			go_enemy->x -= megamaniac->screen_width;
			after_x = (int) round(go_enemy->x);
		}
		else if (after_x < 0) {
			go_enemy->x += megamaniac->screen_width;
			after_x = (int) round(go_enemy->x);
		}
	// }

	// TODO: check the if statement, enemies wrap to late
	// if (after_y != before_y) {
		if (after_y >= (megamaniac->screen_height - 3)) {
			go_enemy->y -= (megamaniac->screen_height - 3);
			after_y = (int) round(go_enemy->y);
		}
		else if (after_y < 0) {
			go_enemy->y += (megamaniac->screen_height - 3);
			after_y = (int) round(go_enemy->y);
		}
	// }

	return ((after_x != before_x) || (after_y != before_y));
}

bool megamaniac_binary_find_save_player_location(game_p megamaniac, game_object_p go_player, game_object_p* game_objects, int game_object_count, int test_sector_x0, int test_sector_x1) {
	if (test_sector_x1 < test_sector_x0) {
		return false;
	}

	int current_test_x = test_sector_x0 + ((test_sector_x1 - test_sector_x0) / 2);

	if (megamaniac_is_safe_player_location(current_test_x, go_player->y, go_player, game_objects, game_object_count)) {
		go_player->x = current_test_x;
		return true;
	}
	else {
		int bias = rand() % 2;

		if (bias == 0) {
			// left, then right
			return	megamaniac_binary_find_save_player_location(megamaniac, go_player, game_objects, game_object_count, test_sector_x0, current_test_x - 1) ||
				megamaniac_binary_find_save_player_location(megamaniac, go_player, game_objects, game_object_count, current_test_x + 1, test_sector_x1);
		}
		else {
			// right, then left
			return	megamaniac_binary_find_save_player_location(megamaniac, go_player, game_objects, game_object_count, current_test_x + 1, test_sector_x1) ||
				megamaniac_binary_find_save_player_location(megamaniac, go_player, game_objects, game_object_count, test_sector_x0, current_test_x - 1);
		}
	}
}

// TODO: what about wrapping enemies (enemies at top edge with vecttor out of screen reappering on the bottom
bool megamaniac_is_safe_player_location(int current_test_x, int current_test_y, game_object_p go_player, game_object_p* game_objects, int game_object_count) {
	assert(NULL != go_player);
	assert(go_player->type == GO_TYPE_PLAYER);
	assert(NULL != game_objects);
	assert(game_object_count > 1);

	int enemy_count = 0;
	rect_t player_rect = {
		.x0 = (int) round(current_test_x - 10),
		.y0 = (int) round(current_test_y - 5),
		.x1 = (int) round(current_test_x + 10),
		.y1 = (int) round(current_test_y + 1)
	};
	rect_t enemy_rect = {0, 0, 0, 0};
	rect_p player_rect_ptr = &player_rect;
	rect_p enemy_rect_ptr = &enemy_rect;


	for (int i = 0; i < game_object_count; ++i) {
		if ((NULL != game_objects[i]) && !(game_objects[i]->recycle) && megamaniac_go_is_enemy(game_objects[i])) {
			enemy_count++;

			enemy_rect.x0 = (int) round(game_objects[i]->x);
			enemy_rect.y0 = (int) round(game_objects[i]->y);
			enemy_rect.x1 = enemy_rect.x0;
			enemy_rect.y1 = enemy_rect.y0;

			if ((player_rect_ptr->x0 <= enemy_rect_ptr->x0) && (enemy_rect_ptr->x0 <= player_rect_ptr->x1) && (player_rect_ptr->y0 <= enemy_rect_ptr->y0) && (enemy_rect_ptr->y0 <= player_rect_ptr->y1)) {
				return false;
			}
		}
	}

	return true;
}

void megamaniac_wipe_projectiles(game_object_p* game_objects, int game_object_count) {
	assert(NULL != game_objects);
	assert(game_object_count > -1);

	for (int i = 0; i < game_object_count; ++i) {
		if ((NULL != game_objects[i]) && !(game_objects[i]->recycle) && ((game_objects[i]->type == GO_TYPE_BULLET) || (game_objects[i]->type == GO_TYPE_BOMB))) {
			game_objects[i]->active = false;
			game_objects[i]->recycle = true;
		}
	}
}

// TODO: what if both enemy and player moved and passed over each other
bool megamaniac_test_enemy_player_collision(game_object_p go_enemy, game_object_p go_player, bool player_find_save_loc, bool wipe_projectiles, game_p megamaniac) {
	assert(NULL != go_enemy);
	assert(NULL != go_player);
	assert(NULL != megamaniac);
	assert(go_enemy != go_player);

	int enemy_x = (int) round(go_enemy->x);
	int enemy_y = (int) round(go_enemy->y);
	int player_x = (int) round(go_player->x);
	int player_y = (int) round(go_player->y);
	
	if ((enemy_x == player_x) && (enemy_y == player_y)) {
		go_enemy->active = false;
		go_enemy->recycle = true;

		game_object_p go_lives = find_game_object_by_type(GO_TYPE_LIVES, megamaniac->game_objects, megamaniac->game_object_count, NULL);
		go_additional_data_comparable_int_p go_lives_data = (go_additional_data_comparable_int_p) go_lives->additional_data;

		go_lives_data->current_value--;

		if (!player_find_save_loc || !megamaniac_binary_find_save_player_location(megamaniac, go_player, megamaniac->current_level->game_objects, megamaniac->current_level->game_object_count, 0, megamaniac->screen_width)) {
			go_player->x = (megamaniac->screen_width) / 2;
		}

		if (wipe_projectiles) {
			megamaniac_wipe_projectiles(megamaniac->current_level->game_objects, megamaniac->current_level->game_object_count);
		}

		return true;
	}

	return false;
}
