#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "game.h"
#include "megamaniac.h"
#include "megamaniac_go_data.h"
#include "megamaniac_go_types.h"
#include "megamaniac_levels.h"
#include "megamaniac_level1.h"

#define LEVEL1_LEVEL_NAME		"Level 1 - Basic"
#define ENEMY_ROW_COUNT			3
#define ENEMY_ROW_ODD_COUNT		3
#define ENEMY_ROW_EVEN_COUNT		4
#define ENEMY_VERTICAL_SPACING		2
#define ENEMY_HORIZONTAL_SPACING	5


//-------------------------------------------------------
// Level Loading Methods Forward Declarations
//-------------------------------------------------------

void level1_load(game_level_p self, game_p megamaniac);

int level1_load_enemies(game_level_p self, game_p megamaniac, int offset);


//-------------------------------------------------------
// Game Object Update Methods Forward Declarations
//-------------------------------------------------------

bool go_enemy1_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level);


//-------------------------------------------------------
// Level Creation Functions
//-------------------------------------------------------

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


//-------------------------------------------------------
// Level Loading Methods
//------------------------------------------------------

void level1_load(game_level_p self, game_p megamaniac) {
	assert(NULL != self);
	assert(NULL != megamaniac);

	int i = 0;

	self->game_objects[i++] = megamaniac_create_level_name(megamaniac, LEVEL1_LEVEL_NAME);
	self->game_objects[i++] = megamaniac_create_player(megamaniac);
	self->game_objects[i++] = megamaniac_create_enemy_mover(megamaniac, 500L, go_enemy1_mover_update);

	i += level1_load_enemies(self, megamaniac, i);

	self->game_objects[i++] = megamaniac_create_bomb_dropper(megamaniac);

	self->paused = false;
}

int level1_load_enemies(game_level_p self, game_p megamaniac, int offset) {
	int enemy_x_odd = (int) round(((megamaniac->screen_width) / 2.) - ((ENEMY_HORIZONTAL_SPACING + 1) * (ENEMY_ROW_ODD_COUNT - 1) / 2));
	int enemy_x_even = (int) round(((megamaniac->screen_width) / 2.) - ((ENEMY_HORIZONTAL_SPACING + 1) * (ENEMY_ROW_EVEN_COUNT - 1) / 2));
	int enemy_y = 1;
	int enemy_count = 0;

	// row 1
	for (int x = enemy_x_odd, c = 0; c < ENEMY_ROW_ODD_COUNT; x += ENEMY_HORIZONTAL_SPACING + 1, c++) {
		self->game_objects[offset] = megamaniac_create_enemy(megamaniac, x, enemy_y, GO_TYPE_ENEMY1, NULL);

		if (NULL != self->game_objects[offset]) {
			offset++;
			enemy_count++;
		}
	}

	enemy_y += ENEMY_VERTICAL_SPACING + 1;

	// row 2
	for (int x = enemy_x_even, c = 0; c < ENEMY_ROW_EVEN_COUNT; x += ENEMY_HORIZONTAL_SPACING + 1, c++) {
		self->game_objects[offset] = megamaniac_create_enemy(megamaniac, x, enemy_y, GO_TYPE_ENEMY1, NULL);

		if (NULL != self->game_objects[offset]) {
			offset++;
			enemy_count++;
		}
	}

	enemy_y += ENEMY_VERTICAL_SPACING + 1;

	// row 3
	for (int x = enemy_x_odd, c = 0; c < ENEMY_ROW_ODD_COUNT; x += ENEMY_HORIZONTAL_SPACING + 1, c++) {
		self->game_objects[offset] = megamaniac_create_enemy(megamaniac, x, enemy_y, GO_TYPE_ENEMY1, NULL);

		if (NULL != self->game_objects[offset]) {
			offset++;
			enemy_count++;
		}
	}

	return enemy_count;
}


//-------------------------------------------------------
// Game Object Update Methods
//-------------------------------------------------------

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
