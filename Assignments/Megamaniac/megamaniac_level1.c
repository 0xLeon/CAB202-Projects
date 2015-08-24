#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "game.h"
#include "game_timer.h"
#include "megamaniac.h"
#include "megamaniac_go_data.h"
#include "megamaniac_go_types.h"
#include "megamaniac_level1.h"

#define LEVEL1_LEVEL_NAME		"Level 1 - Basic"
#define ENEMY_ROW_COUNT			3
#define ENEMY_ROW_ODD_COUNT		3
#define ENEMY_ROW_EVEN_COUNT		4
#define ENEMY_VERTICAL_SPACING		2
#define ENEMY_HORIZONTAL_SPACING	5


int level1_load_enemies(game_level_p self, game_p megamaniac, int offset) {
	int enemy_x_odd = (int) round(((megamaniac->screen_width) / 2.) - ((ENEMY_HORIZONTAL_SPACING + 1) * (ENEMY_ROW_ODD_COUNT - 1) / 2));
	int enemy_x_even = (int) round(((megamaniac->screen_width) / 2.) - ((ENEMY_HORIZONTAL_SPACING + 1) * (ENEMY_ROW_EVEN_COUNT - 1) / 2));
	int enemy_y = 1;
	int enemy_count = 0;

	// row 1
	for (int x = enemy_x_odd, c = 0; c < ENEMY_ROW_ODD_COUNT; x += ENEMY_HORIZONTAL_SPACING + 1, c++) {
		self->game_objects[offset] = level1_create_enemy1(x, enemy_y, megamaniac);

		if (NULL != self->game_objects[offset]) {
			offset++;
			enemy_count++;
		}
	}

	enemy_y += ENEMY_VERTICAL_SPACING + 1;

	// row 2
	for (int x = enemy_x_even, c = 0; c < ENEMY_ROW_EVEN_COUNT; x += ENEMY_HORIZONTAL_SPACING + 1, c++) {
		self->game_objects[offset] = level1_create_enemy1(x, enemy_y, megamaniac);

		if (NULL != self->game_objects[offset]) {
			offset++;
			enemy_count++;
		}
	}

	enemy_y += ENEMY_VERTICAL_SPACING + 1;

	// row 3
	for (int x = enemy_x_odd, c = 0; c < ENEMY_ROW_ODD_COUNT; x += ENEMY_HORIZONTAL_SPACING + 1, c++) {
		self->game_objects[offset] = level1_create_enemy1(x, enemy_y, megamaniac);

		if (NULL != self->game_objects[offset]) {
			offset++;
			enemy_count++;
		}
	}

	return enemy_count;
}

void level1_load(game_level_p self, game_p megamaniac) {
	assert(NULL != self);
	assert(NULL != megamaniac);

	int i = 0;

	self->game_objects[i++] = level1_create_level_name(megamaniac);
	self->game_objects[i++] = level1_create_player(megamaniac);
	self->game_objects[i++] = level1_create_enemy1_mover(megamaniac);

	i += level1_load_enemies(self, megamaniac, i);

	self->game_objects[i++] = level1_create_bomb_dropper(megamaniac);

	self->paused = false;
}

// TODO: null checks for game objects
game_level_p level1_create_level(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_level_p level1 = create_level(14);

	if (NULL == level1) {
		return NULL;
	}

	level1->load = level1_load;
	level1->unload = level_default_unload;

	return level1;
}

game_object_p level1_create_level_name(game_p megamaniac) {
	int x = (megamaniac->screen_width - strlen(LEVEL1_LEVEL_NAME)) / 2;
	int y = megamaniac->screen_height - 1;

	return create_static_string_game_object(GO_TYPE_LEVEL_NAME, x, y, 0., 0., 0L, LEVEL1_LEVEL_NAME);
}


bool go_player_updater(game_object_p self, game_update_p update, game_p game, game_level_p level) {
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
			/*if (NULL != find_game_object_by_type(GO_TYPE_BULLET, level->game_objects, level->game_object_count, NULL)) {
				return false;
			}*/

			// TODO: error checks
			level_add_game_object(level, level1_create_bullet(game, (int) round(self->x)));

			return true;
	}

	return false;
}

game_object_p level1_create_player(game_p megamaniac) {
	game_object_p go_player = create_static_string_game_object(GO_TYPE_PLAYER, (megamaniac->screen_width) / 2, megamaniac->screen_height - 4, 0., 0., 0L, "$");

	if (NULL == go_player) {
		return NULL;
	}

	go_player->update = go_player_updater;

	return go_player;
}


bool go_bullet_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	bool didMove = move_game_object(self);
	int game_object_count;
	game_object_p* game_objects;

	if (NULL != level) {
		game_object_count = level->game_object_count;
		game_objects = level->game_objects;
	}
	else {
		game_object_count = game->game_object_count;
		game_objects = game->game_objects;
	}

	if (self->y < 0) {
		// destroy bullet
		self->active = false;
		self->recycle = true;

		return true;
	}

	int enemy_counter = 0;
	for (int i = 0; i < game_object_count; i++) {
		if ((NULL != game_objects[i]) && (game_objects[i]->type == GO_TYPE_ENEMY1) && game_objects[i]->active) {
			enemy_counter++;

			if ((((int) round(self->x)) == ((int) round(game_objects[i]->x))) && (((int) round(self->y + 1)) == ((int) round(game_objects[i]->y)))) {
				// destroy_game_object(game_objects[i]);
				// game_objects[i] = NULL;
				game_objects[i]->active = false;
				game_objects[i]->recycle = true;
				enemy_counter--;

				// TODO: null checks
				game_object_p go_score = find_game_object_by_type(GO_TYPE_SCORE, game->game_objects, game->game_object_count, NULL);
				go_additional_data_comparable_int_p go_score_data = (go_additional_data_comparable_int_p) go_score->additional_data;

				self->active = false;
				self->recycle = true;

				go_score_data->current_value += POINTS_PER_ENEMY;

				if ((enemy_counter == 0) && (i < (game_object_count))) {
					for (int j = i + 1; j < game_object_count; j++) {
						if ((NULL != game_objects[j]) && (game_objects[j]->type == GO_TYPE_ENEMY1) && game_objects[j]->active) {
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

	return didMove;
}

game_object_p level1_create_bullet(game_p megamaniac, int x) {
	game_object_p go_bullet = create_static_string_game_object(GO_TYPE_BULLET, x, megamaniac->screen_height - 5, 0., -1., 70L, "|");

	if (NULL == go_bullet) {
		return NULL;
	}

	go_bullet->update = go_bullet_update;

	return go_bullet;
}


game_object_p level1_create_enemy1(int x, int y, game_p megamaniac) {
	return create_static_string_game_object(GO_TYPE_ENEMY1, x, y, 1., 0., 0L, "@");
}


bool go_enemy1_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	int game_object_count;
	game_object_p* game_objects;
	bool didMove = false;

	if (NULL != level) {
		game_object_count = level->game_object_count;
		game_objects = level->game_objects;
	}
	else {
		game_object_count = game->game_object_count;
		game_objects = game->game_objects;
	}

	for (int i = 0; i < game_object_count; i++) {
		if ((NULL != game_objects[i]) && (game_objects[i]->type == GO_TYPE_ENEMY1)) {
			didMove = move_game_object(game_objects[i]) || didMove;

			if (game_objects[i]->x >= game->screen_width) {
				game_objects[i]->x = 0.;

				didMove = true;
			}
		}
	}

	return didMove;
}

game_object_p level1_create_enemy1_mover(game_p megamaniac) {
	game_object_p go_enemy_mover = create_null_game_object(GO_TYPE_ENEMY1_MOVER);

	if (NULL == go_enemy_mover) {
		return NULL;
	}

	go_enemy_mover->timer = game_create_timer(500L);

	if (NULL == go_enemy_mover->timer) {
		free(go_enemy_mover);

		return NULL;
	}

	go_enemy_mover->update = go_enemy1_mover_update;

	return go_enemy_mover;
}


bool go_bomb_dropper_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	int game_object_count;
	game_object_p* game_objects;

	if (NULL != level) {
		game_object_count = level->game_object_count;
		game_objects = level->game_objects;
	}
	else {
		game_object_count = game->game_object_count;
		game_objects = game->game_objects;
	}

	int bomb_count = 0;
	int enemy_count = 0;
	int enemy_index = 0;

	for (int i = 0; i < game_object_count; i++) {
		if (NULL != game_objects[i]) {
			switch (game_objects[i]->type) {
				case GO_TYPE_ENEMY1:
					enemy_count++;
					break;
				case GO_TYPE_BOMB:
					bomb_count++;
					break;
			}
		}
	}

	if ((bomb_count == 4) || (enemy_count == 0)) {
		return false;
	}

	enemy_index = rand() % (enemy_count + 1);

	for (int i = 0, c = 0; i < game_object_count; i++) {
		if ((NULL != game_objects[i]) && (game_objects[i]->type == GO_TYPE_ENEMY1)) {
			if (c == enemy_index) {
				// TODO: error checks
				level_add_game_object(level, level1_create_bomb(game, (int) round(game_objects[i]->x), (int) round(game_objects[i]->y + 1)));

				return true;
			}
			else {
				c++;
			}
		}
	}

	return false;
}

game_object_p level1_create_bomb_dropper(game_p game) {
	game_object_p go_bomb_dropper = create_null_game_object(GO_TYPE_BOMB_DROPPER);

	if (NULL == go_bomb_dropper) {
		return NULL;
	}

	go_bomb_dropper->timer = game_create_timer(3000L);

	if (NULL == go_bomb_dropper->timer) {
		free(go_bomb_dropper);

		return NULL;
	}

	go_bomb_dropper->update = go_bomb_dropper_update;

	return go_bomb_dropper;
}


bool go_bomb_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	bool didMove = move_game_object(self);

	if (self->y > (game->screen_height - 4)) {
		// destroy bomb
		self->active = false;
		self->recycle = true;

		return true;
	}

	return didMove;
}

game_object_p level1_create_bomb(game_p game, int x, int y) {
	assert(NULL != game);
	assert(x > -1);

	game_object_p go_bomb = create_static_string_game_object(GO_TYPE_BOMB, x, y, 0., 0.6, 300L, "*");

	if (NULL == go_bomb) {
		return NULL;
	}

	go_bomb->update = go_bomb_update;

	return go_bomb;
}


game_object_p level1_create_lost_screen(game_p game) {
	assert(NULL != game);

	char* text = "             You lost!              Press 'r' to restart or 'q' to quit.";
	game_object_p go_lost_screen = create_static_string_game_object(GO_TYPE_LEVEL_NAME, 0., 0, 0., 0., 0L, text);

	if (NULL == go_lost_screen) {
		return NULL;
	}

	go_lost_screen->width = 36;
	go_lost_screen->height = 2;
	go_lost_screen->x = (game->screen_width - go_lost_screen->width) / 2;
	go_lost_screen->y = (game->screen_height - go_lost_screen->height) / 2;

	return go_lost_screen;
}
