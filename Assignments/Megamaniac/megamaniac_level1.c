#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "game_types.h"
#include "game.h"
#include "megamaniac.h"
#include "megamaniac_go_data.h"
#include "megamaniac_go_types.h"
#include "megamaniac_game_objects.h"
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

void megamaniac_level1_load(game_level_p self, game_p megamaniac);

void megamaniac_level1_resize(game_level_p self, game_resize_descriptor_p resize_descriptor, game_p game);

//-------------------------------------------------------
// Game Object Update Methods Forward Declarations
//-------------------------------------------------------

bool go_enemy1_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level);


//-------------------------------------------------------
// Level Creation Functions
//-------------------------------------------------------

// TODO: null checks for game objects
game_level_p megamaniac_create_level1(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_level_p level1 = create_level(14);

	if (NULL == level1) {
		return NULL;
	}

	level1->load = megamaniac_level1_load;
	level1->resize = megamaniac_level1_resize;
	level1->unload = level_default_unload;

	return level1;
}


//-------------------------------------------------------
// Level Loading Methods
//------------------------------------------------------

void megamaniac_level1_load(game_level_p self, game_p megamaniac) {
	assert(NULL != self);
	assert(NULL != megamaniac);

	int i = 0;

	self->game_objects[i++] = megamaniac_create_go_level_name(megamaniac, LEVEL1_LEVEL_NAME);
	self->game_objects[i++] = megamaniac_create_go_player(megamaniac);
	self->game_objects[i++] = megamaniac_create_go_enemy_mover(megamaniac, 500L, go_enemy1_mover_update);

	i += megamaniac_create_standard_enemy_formation(self, megamaniac, i, GO_TYPE_ENEMY1, ENEMY_ROW_COUNT, ENEMY_ROW_ODD_COUNT, ENEMY_ROW_EVEN_COUNT, ENEMY_HORIZONTAL_SPACING, ENEMY_VERTICAL_SPACING, NULL);

	self->game_objects[i++] = megamaniac_create_go_bomb_dropper(megamaniac);

	self->paused = false;
}

void megamaniac_level1_resize(game_level_p self, game_resize_descriptor_p resize_descriptor, game_p game) {
	assert(NULL != self);
	assert(NULL != resize_descriptor);
	assert(NULL != game);

	game_object_p go_player = find_game_object_by_type(GO_TYPE_PLAYER, self->game_objects, self->game_object_count, NULL);
	game_object_p go_level_name = find_game_object_by_type(GO_TYPE_LEVEL_NAME, self->game_objects, self->game_object_count, NULL);
	game_object_p go_enemy = find_game_object_by_type(GO_TYPE_ENEMY1, self->game_objects, self->game_object_count, NULL);
	
	go_player->x = (int) round(go_player->x * resize_descriptor->width_ratio);
	go_player->y = resize_descriptor->new_height - 4;

	go_level_name->x = (resize_descriptor->new_width - strlen(go_level_name->bitmap)) / 2;
	go_level_name->y = resize_descriptor->new_height - 1;
	
	double delta_enemy_x = (go_enemy->x * resize_descriptor->width_ratio) - go_enemy->x;
	double delta_enemy_y = (go_enemy->y * resize_descriptor->height_ratio) - go_enemy->y;

	for (int i = 0; i < self->game_object_count; ++i) {
		if ((NULL != self->game_objects[i]) && !(self->game_objects[i]->recycle)) {
			if (megamaniac_go_is_enemy(self->game_objects[i])) {
				self->game_objects[i]->x += delta_enemy_x;
				self->game_objects[i]->y += delta_enemy_y;
			}
			else if ((self->game_objects[i]->type == GO_TYPE_BOMB) || (self->game_objects[i]->type == GO_TYPE_BULLET)) {
				self->game_objects[i]->x *= resize_descriptor->width_ratio;
				self->game_objects[i]->y *= resize_descriptor->height_ratio;
			}
		}
	}
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
				game_objects[i]->x -= game->screen_width;

				didMove = true;
			}
		}
	}

	return didMove;
}
