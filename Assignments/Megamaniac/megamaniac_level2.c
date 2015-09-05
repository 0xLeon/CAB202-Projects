#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "game_types.h"
#include "game.h"
#include "megamaniac_MAIN.h"
#include "megamaniac_go_data.h"
#include "megamaniac_go_types.h"
#include "megamaniac_game_objects.h"
#include "megamaniac_level2.h"

#define LEVEL2_LEVEL_NAME			"Level 2 - Harmonic"
#define LEVEL2_ENEMY_ROW_COUNT			3
#define LEVEL2_ENEMY_ROW_ODD_COUNT		3
#define LEVEL2_ENEMY_ROW_EVEN_COUNT		4
#define LEVEL2_ENEMY_VERTICAL_SPACING		2
#define LEVEL2_ENEMY_HORIZONTAL_SPACING		5


//-------------------------------------------------------
// Level Loading Methods Forward Declarations
//-------------------------------------------------------

void megamaniac_level2_load(game_level_p self, game_p megamaniac);


//-------------------------------------------------------
// Game Object Creation Functions Forward Declarations
//-------------------------------------------------------

game_object_p megamaniac_level2_create_go_enemy2_mover(game_p megamaniac);


//-------------------------------------------------------
// Game Objects Object Methods Forward Declarations
//-------------------------------------------------------

bool go_enemy2_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level);


//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------
// Level Creation Functions
//-------------------------------------------------------

// TODO: null checks for game objects
game_level_p megamaniac_create_level2(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_level_p level2 = create_level(14);

	if (NULL == level2) {
		return NULL;
	}

	level2->load = megamaniac_level2_load;
	level2->resize = megamaniac_level_default_resize;
	level2->unload = level_default_unload;

	return level2;
}


//-------------------------------------------------------
// Level Loading Methods
//------------------------------------------------------

void megamaniac_level2_load(game_level_p self, game_p megamaniac) {
	assert(NULL != self);
	assert(NULL != megamaniac);

	int i = 0;

	self->game_objects[i++] = megamaniac_create_go_level_name(megamaniac, LEVEL2_LEVEL_NAME);
	self->game_objects[i++] = megamaniac_create_go_player(megamaniac);
	self->game_objects[i++] = megamaniac_level2_create_go_enemy2_mover(megamaniac);

	i += megamaniac_create_standard_enemy_formation(self, megamaniac, i, GO_TYPE_ENEMY2, LEVEL2_ENEMY_ROW_COUNT, LEVEL2_ENEMY_ROW_ODD_COUNT, LEVEL2_ENEMY_ROW_EVEN_COUNT, LEVEL2_ENEMY_HORIZONTAL_SPACING, LEVEL2_ENEMY_VERTICAL_SPACING, true, NULL);

	self->game_objects[i++] = megamaniac_create_go_bomb_dropper(megamaniac);

	self->paused = false;
}


//-------------------------------------------------------
// Game Object Creation Functions
//-------------------------------------------------------

game_object_p megamaniac_level2_create_go_enemy2_mover(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_object_p go_enemy_mover = megamaniac_create_go_enemy_mover(megamaniac, 300L, go_enemy2_mover_update);
	go_additional_data_enemy2_mover_p go_enemy_mover_data = NULL;

	if (NULL == go_enemy_mover) {
		return NULL;
	}

	go_enemy_mover_data = malloc(sizeof(go_additional_data_enemy2_mover_t));

	if (NULL == go_enemy_mover_data) {
		destroy_game_object(go_enemy_mover);

		return NULL;
	}

	go_enemy_mover_data->theta = 0.;
	go_enemy_mover_data->dtheta = 7.;
	go_enemy_mover->additional_data = (void*) go_enemy_mover_data;

	return go_enemy_mover;
}


//-------------------------------------------------------
// Game Objects Object Methods
//-------------------------------------------------------

bool go_enemy2_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);


	if (!timer_expired(self->timer)) {
		return false;
	}

	bool didMove = false;
	go_additional_data_enemy2_mover_p go_enemy_mover_data = (go_additional_data_enemy2_mover_p) self->additional_data;


	for (int i = 0; i < game->current_level->game_object_count; ++i) {
		if ((NULL != game->current_level->game_objects[i]) && (GO_TYPE_ENEMY2 == game->current_level->game_objects[i]->type)) {
			double dcos = (-1. * cos(1.1875 * go_enemy_mover_data->theta * M_PI / 180.) + 1.) - (-1. * cos(1.1875 * (go_enemy_mover_data->theta - go_enemy_mover_data->dtheta) * M_PI / 180.) + 1.);
			double dcos_amp = round(game->screen_height / 8.);

			game->current_level->game_objects[i]->dy = dcos_amp * dcos;

			didMove = megamaniac_move_enemy(game->current_level->game_objects[i], game);
		}
	}

	go_enemy_mover_data->theta += go_enemy_mover_data->dtheta;

	return didMove;
}
