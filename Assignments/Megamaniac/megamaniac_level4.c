#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "game_types.h"
#include "game.h"
#include "megamaniac.h"
#include "megamaniac_go_data.h"
#include "megamaniac_go_types.h"
#include "megamaniac_game_objects.h"
#include "megamaniac_level4.h"

#define LEVEL4_LEVEL_NAME			"Level 4 - Drunken"
#define LEVEL4_ENEMY_ROW_COUNT			3
#define LEVEL4_ENEMY_ROW_ODD_COUNT		3
#define LEVEL4_ENEMY_ROW_EVEN_COUNT		4
#define LEVEL4_ENEMY_VERTICAL_SPACING		2
#define LEVEL4_ENEMY_HORIZONTAL_SPACING		5

#define LEVEL4_ENEMY_SPEED_CHANGE_LOWER		1000
#define LEVEL4_ENEMY_SPEED_CHANGE_UPPER		3000
#define LEVEL4_ENEMY_X_MIN_SPEED		5
#define LEVEL4_ENEMY_X_MAX_SPEED		14


//-------------------------------------------------------
// Level Loading Methods Forward Declarations
//-------------------------------------------------------

void megamaniac_level4_load(game_level_p self, game_p megamaniac);


//-------------------------------------------------------
// Game Object Creation Forward Declarations
//-------------------------------------------------------

int megamaniac_level4_create_enemies(game_level_p level, game_p megamaniac, int offset);


//-------------------------------------------------------
// Game Objects Object Methods Forward Declarations
//-------------------------------------------------------

bool go_enemy4_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

void go_enemy4_destroy(game_object_p self, game_p game, game_level_p level);


//-------------------------------------------------------
// Helper Functions Forward Declarations
//-------------------------------------------------------

void go_enemy4_change_speed(game_object_p go_enemy, go_additional_data_enemy4_p go_enemy_data);

bool go_enemy4_is_valid_enemy_position(game_object_p enemy, game_object_p* enemies, int enemy_count);


//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------
// Level Creation Functions
//-------------------------------------------------------

// TODO: null checks for game objects
game_level_p megamaniac_create_level4(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_level_p level4 = create_level(14);

	if (NULL == level4) {
		return NULL;
	}

	level4->load = megamaniac_level4_load;
	level4->resize = megamaniac_level_default_resize;
	level4->unload = level_default_unload;

	return level4;
}


//-------------------------------------------------------
// Level Loading Methods
//------------------------------------------------------

void megamaniac_level4_load(game_level_p self, game_p megamaniac) {
	assert(NULL != self);
	assert(NULL != megamaniac);

	int i = 0;

	self->game_objects[i++] = megamaniac_create_go_level_name(megamaniac, LEVEL4_LEVEL_NAME);
	self->game_objects[i++] = megamaniac_create_go_player(megamaniac);
	self->game_objects[i++] = megamaniac_create_go_enemy_mover(megamaniac, 300L, go_enemy4_mover_update);
	
	i += megamaniac_level4_create_enemies(self, megamaniac, i);

	self->game_objects[i++] = megamaniac_create_go_bomb_dropper(megamaniac);

	self->paused = false;
}


//-------------------------------------------------------
// Game Object Creation Functions
//-------------------------------------------------------

int megamaniac_level4_create_enemies(game_level_p level, game_p megamaniac, int offset) {
	assert(NULL != level);
	assert(NULL != megamaniac);
	assert(offset > -1);
	assert(offset < level->game_object_count);

	int enemy_count = megamaniac_create_standard_enemy_formation(level, megamaniac, offset, GO_TYPE_ENEMY4, LEVEL4_ENEMY_ROW_COUNT, LEVEL4_ENEMY_ROW_ODD_COUNT, LEVEL4_ENEMY_ROW_EVEN_COUNT, LEVEL4_ENEMY_HORIZONTAL_SPACING, LEVEL4_ENEMY_VERTICAL_SPACING, true, NULL);
	int successful_enemy_count = 0;
	int new_offset = offset + enemy_count;

	game_object_p go_enemy = NULL;
	go_additional_data_enemy4_p go_enemy_data = NULL;

	for (int i = offset; i < new_offset; ++i) {
		if ((NULL != level->game_objects[i]) && (GO_TYPE_ENEMY4 == level->game_objects[i]->type)) {
			go_enemy = level->game_objects[i];
			go_enemy_data = malloc(sizeof(go_additional_data_enemy4_t));

			if (NULL == go_enemy_data) {
				destroy_game_object(go_enemy);
				level->game_objects[i] = NULL;

				continue;
			}

			go_enemy_data->change_timer = create_timer(LEVEL4_ENEMY_SPEED_CHANGE_LOWER + (rand() % (LEVEL4_ENEMY_SPEED_CHANGE_UPPER - LEVEL4_ENEMY_SPEED_CHANGE_LOWER + 1)));

			go_enemy->dx = ((((rand() % 2) == 0) ? (1) : (-1)) * (LEVEL4_ENEMY_X_MIN_SPEED + (rand() % (LEVEL4_ENEMY_X_MAX_SPEED - LEVEL4_ENEMY_X_MIN_SPEED + 1)))) / 10.;
			go_enemy->additional_data = go_enemy_data;
			go_enemy->destroy = go_enemy4_destroy;

			++successful_enemy_count;
		}
	}

	return successful_enemy_count;
}


//-------------------------------------------------------
// Game Objects Object Methods
//-------------------------------------------------------

bool go_enemy4_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != level);

	bool did_update = false;
	int enemy_count = 0;

	int buffer_size = 10;
	game_object_p* enemies = malloc(buffer_size * sizeof(game_object_p));

	for (int i = 0, offset = 0; i < game->current_level->game_object_count; ++i) {
		if ((NULL != game->current_level->game_objects[i]) && !(game->current_level->game_objects[i]->recycle) && megamaniac_go_is_enemy(game->current_level->game_objects[i])) {
			enemy_count++;

			if (enemy_count >= buffer_size) {
				buffer_size += 1;

				game_object_p* new_go_enemies = realloc(enemies, buffer_size * sizeof(game_object_p));

				if (NULL == new_go_enemies) {
					continue;
				}

				enemies = new_go_enemies;
			}

			enemies[offset++] = game->current_level->game_objects[i];
		}
	}

	bool move_timer_expired = timer_expired(self->timer);

	game_object_p go_enemy = NULL;
	go_additional_data_enemy4_p go_enemy_data = NULL;
	bool moved;
	double enemy_x;

	for (int i = 0; i < enemy_count; ++i) {
		if (GO_TYPE_ENEMY4 == enemies[i]->type) {
			go_enemy = enemies[i];
			go_enemy_data = (go_additional_data_enemy4_p) go_enemy->additional_data;
			moved = false;

			if (timer_expired(go_enemy_data->change_timer)) {
				go_enemy4_change_speed(go_enemy, go_enemy_data);
			}

			if (!moved && move_timer_expired) {
				enemy_x = go_enemy->x;

				// TODO: do not calc this every enemy every update
				go_enemy->dy = 10. / game->screen_width;

				moved = megamaniac_move_enemy(go_enemy, game);

				if (moved) {
					game_object_p go_player = find_game_object_by_type(GO_TYPE_PLAYER, game->current_level->game_objects, game->current_level->game_object_count, NULL);

					if (!go_enemy4_is_valid_enemy_position(go_enemy, enemies, enemy_count)) {
						go_enemy->x = enemy_x;

						go_enemy4_change_speed(go_enemy, go_enemy_data);
					}

					megamaniac_test_enemy_player_collision(go_enemy, go_player, true, true, game);

					did_update = true;
				}

			}
		}
	}

	return did_update;
}

void go_enemy4_destroy(game_object_p self, game_p game, game_level_p level) {
	assert(NULL != self);
	// TODO: second and third parameter always NULL for now
	// assert(NULL != game);
	// assert(NULL != level);

	go_additional_data_enemy4_p go_enemy_data = (go_additional_data_enemy4_p) self->additional_data;

	free(go_enemy_data->change_timer);
	go_enemy_data->change_timer = NULL;
}


//-------------------------------------------------------
// Helper Functions
//-------------------------------------------------------

void go_enemy4_change_speed(game_object_p go_enemy, go_additional_data_enemy4_p go_enemy_data) {
	assert(NULL != go_enemy);
	assert(NULL != go_enemy_data);

	go_enemy->dx = (-1 * GAME_SIGNUM(go_enemy->dx) * (LEVEL4_ENEMY_X_MIN_SPEED + (rand() % (LEVEL4_ENEMY_X_MAX_SPEED - LEVEL4_ENEMY_X_MIN_SPEED + 1)))) / 10.;

	go_enemy_data->change_timer->milliseconds = LEVEL4_ENEMY_SPEED_CHANGE_LOWER + (rand() % (LEVEL4_ENEMY_SPEED_CHANGE_UPPER - LEVEL4_ENEMY_SPEED_CHANGE_LOWER + 1));
	reset_timer(go_enemy_data->change_timer);
}

bool go_enemy4_is_valid_enemy_position(game_object_p enemy, game_object_p* enemies, int enemy_count) {
	assert(NULL != enemy);
	assert(NULL != enemies);
	assert(enemy_count > 0);

	int looped_enemy_x = 0;
	int test_enemy_x0 = ((int) round(enemy->x - 1));
	int test_enemy_x1 = ((int) round(enemy->x + 1));

	for (int i = 0; i < enemy_count; ++i) {
		if (!(enemies[i]->recycle) && (enemy != enemies[i])) {
			// if looped enemy is inside viewed enemy's inside save volume, location is invalid
			looped_enemy_x = (int) round(enemies[i]->x);

			if ((test_enemy_x0 <= looped_enemy_x) && (looped_enemy_x <= test_enemy_x1)) {
				return false;
			}
		}
	}

	// no enemy was in viewed enemy's safe volume, location is valid
	return true;
}
