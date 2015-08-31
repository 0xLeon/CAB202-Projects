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
#include "megamaniac_level3.h"

#define LEVEL3_LEVEL_NAME			"Level 3 - Falling"
#define LEVEL3_ENEMY_ROW_COUNT			3
#define LEVEL3_ENEMY_ROW_ODD_COUNT		3
#define LEVEL3_ENEMY_ROW_EVEN_COUNT		4
#define LEVEL3_ENEMY_VERTICAL_SPACING		2
#define LEVEL3_ENEMY_HORIZONTAL_SPACING		5

//-------------------------------------------------------
// Level Loading Methods Forward Declarations
//-------------------------------------------------------

void megamaniac_level3_load(game_level_p self, game_p megamaniac);


//-------------------------------------------------------
// Game Object Creation Functions Forward Declarations
//-------------------------------------------------------

int megamaniac_level3_create_go_enemy3_mover(game_level_p self, game_p megamaniac, int offset);


//-------------------------------------------------------
// Game Object Update Methods Forward Declarations
//-------------------------------------------------------

bool go_enemy3_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level);


//-------------------------------------------------------
// Level Creation Functions
//-------------------------------------------------------

// TODO: null checks for game objects
game_level_p megamaniac_create_level3(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_level_p level3 = create_level(14);

	if (NULL == level3) {
		return NULL;
	}

	level3->load = megamaniac_level3_load;
	level3->resize = megamaniac_level_default_resize;
	level3->unload = level_default_unload;

	return level3;
}


//-------------------------------------------------------
// Level Loading Methods
//------------------------------------------------------

void megamaniac_level3_load(game_level_p self, game_p megamaniac) {
	assert(NULL != self);
	assert(NULL != megamaniac);

	int i = 0;

	self->game_objects[i++] = megamaniac_create_go_level_name(megamaniac, LEVEL3_LEVEL_NAME);
	self->game_objects[i++] = megamaniac_create_go_player(megamaniac);
	
	i += megamaniac_level3_create_go_enemy3_mover(self, megamaniac, i);
	i += megamaniac_create_standard_enemy_formation(self, megamaniac, i, GO_TYPE_ENEMY3, LEVEL3_ENEMY_ROW_COUNT, LEVEL3_ENEMY_ROW_ODD_COUNT, LEVEL3_ENEMY_ROW_EVEN_COUNT, LEVEL3_ENEMY_HORIZONTAL_SPACING, LEVEL3_ENEMY_VERTICAL_SPACING, NULL);

	self->game_objects[i++] = megamaniac_create_go_bomb_dropper(megamaniac);

	self->paused = false;
}


//-------------------------------------------------------
// Game Object Creation Functions
//-------------------------------------------------------

int megamaniac_level3_create_go_enemy3_mover(game_level_p self, game_p megamaniac, int offset) {
	assert(NULL != self);
	assert(NULL != megamaniac);
	assert(offset > -1);
	assert(offset < self->game_object_count);

	game_object_p go_enemy_mover = megamaniac_create_go_enemy_mover(megamaniac, 300L, go_enemy3_mover_update);
	go_additional_data_enemy2_mover_p go_enemy_mover_data = malloc(sizeof(go_additional_data_enemy2_mover_t));

	if (NULL == go_enemy_mover) {
		return 0;
	}

	if (NULL == go_enemy_mover_data) {
		destroy_game_object(go_enemy_mover);

		return 0;
	}

	go_enemy_mover_data->theta = 0.;
	go_enemy_mover_data->dtheta = 7.;
	go_enemy_mover->additional_data = (void*) go_enemy_mover_data;

	self->game_objects[offset++] = go_enemy_mover;

	return 1;
}


//-------------------------------------------------------
// Game Object Update Methods
//-------------------------------------------------------

bool go_enemy3_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	bool didMove = false;
	go_additional_data_enemy2_mover_p go_enemy_mover_data = (go_additional_data_enemy2_mover_p) self->additional_data;


	for (int i = 0; i < game->current_level->game_object_count; i++) {
		if ((NULL != game->current_level->game_objects[i]) && (game->current_level->game_objects[i]->type == GO_TYPE_ENEMY3)) {
			double dcos = (-1. * cos(1.1875 * go_enemy_mover_data->theta * M_PI / 180.) + 1.) - (-1. * cos(1.1875 * (go_enemy_mover_data->theta - go_enemy_mover_data->dtheta) * M_PI / 180.) + 1.);
			double dcos_amp = round(game->screen_height / 8.);

			game->current_level->game_objects[i]->dy = dcos_amp * dcos + (25. / game->screen_width);

			// TODO: formation is lost when wrapping at top or bottom edge
			didMove = megamaniac_move_enemy(game->current_level->game_objects[i], game) || didMove;

			game_object_p go_player = find_game_object_by_type(GO_TYPE_PLAYER, game->current_level->game_objects, game->current_level->game_object_count, NULL);
			
			if ((((int) round(game->current_level->game_objects[i]->x)) == ((int) round(go_player->x))) && (((int) round(game->current_level->game_objects[i]->y)) == ((int) round(go_player->y)))) {
				game->current_level->game_objects[i]->active = false;
				game->current_level->game_objects[i]->recycle = true;

				game_object_p go_lives = find_game_object_by_type(GO_TYPE_LIVES, game->game_objects, game->game_object_count, NULL);
				go_additional_data_comparable_int_p go_lives_data = (go_additional_data_comparable_int_p) go_lives->additional_data;

				go_lives_data->current_value--;
				
				if (!megamaniac_binary_find_save_player_location(game, go_player, game->current_level->game_objects, game->current_level->game_object_count, 0, game->screen_width)) {
					go_player->x = (game->screen_width) / 2;
				}

				megamaniac_wipe_projectiles(game->current_level->game_objects, game->current_level->game_object_count);

				return true;
			}
		}
	}

	go_enemy_mover_data->theta += go_enemy_mover_data->dtheta;

	return didMove;
}
