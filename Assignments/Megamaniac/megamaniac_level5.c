#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "game_types.h"
#include "game.h"
#include "game_object.h"
#include "megamaniac_MAIN.h"
#include "megamaniac_go_data.h"
#include "megamaniac_go_types.h"
#include "megamaniac_game_objects.h"
#include "megamaniac_level5.h"

#define LEVEL5_LEVEL_NAME			"Level 5 - Aggressive"
#define LEVEL5_ENEMY_ROW_COUNT			3
#define LEVEL5_ENEMY_ROW_ODD_COUNT		3
#define LEVEL5_ENEMY_ROW_EVEN_COUNT		4
#define LEVEL5_ENEMY_VERTICAL_SPACING		2
#define LEVEL5_ENEMY_HORIZONTAL_SPACING		5

#define LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MIN		1000
#define LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MAX		7000
#define LEVEL5_ENEMY_PARABOLIC_DISPLACEMENT_MIN		10
#define LEVEL5_ENEMY_PARABOLIC_DISPLACEMENT_MAX		30


//-------------------------------------------------------
// Level Loading Methods Forward Declarations
//-------------------------------------------------------

void megamaniac_level5_load(game_level_p self, game_p megamaniac);

// void megamaniac_level5_resize(game_level_p self, game_resize_descriptor_p resize_descriptor, game_p game);


//-------------------------------------------------------
// Game Object Creation Forward Declarations
//-------------------------------------------------------

game_object_p megamaniac_level5_create_go_enemy_indep_controller(game_p megamaniac);

game_object_p megamaniac_level5_create_go_enemy_indep_choser(game_p megamaniac);

game_object_p megamaniac_level5_create_go_bullet_controller(game_p megamaniac);


//-------------------------------------------------------
// Game Objects Object Methods Forward Declarations
//-------------------------------------------------------

bool go_enemy5_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_enemy_indep_controller_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_enemy_indep_choser_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

bool go_bullet_controller_update(game_object_p self, game_update_p update, game_p game, game_level_p level);


//-------------------------------------------------------
// Helper Functions Forward Declarations
//-------------------------------------------------------

void go_enemy_indep_controller_reset_data(go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data, game_object_p go_enemy);

void go_enemy_indep_controller_calc_curve(go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data, game_p game);

bool go_enemy_indep_controller_did_player_move(go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data);

bool go_enemy_indep_controller_move_enemy(go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data, game_p game);

bool go_enemy_indep_controller_reached_group_limit(go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data, game_p game);


//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------
// Level Creation Functions
//-------------------------------------------------------

// TODO: null checks for game objects
game_level_p megamaniac_create_level5(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_level_p level5 = create_level(17);

	if (NULL == level5) {
		return NULL;
	}

	level5->load = megamaniac_level5_load;
	// level5->resize = megamaniac_level5_resize;
	level5->resize = megamaniac_level_default_resize;
	level5->unload = level_default_unload;

	return level5;
}


//-------------------------------------------------------
// Level Loading Methods
//------------------------------------------------------

void megamaniac_level5_load(game_level_p self, game_p megamaniac) {
	assert(NULL != self);
	assert(NULL != megamaniac);

	int i = 0;

	self->game_objects[i++] = megamaniac_create_go_level_name(megamaniac, LEVEL5_LEVEL_NAME);
	self->game_objects[i++] = megamaniac_create_go_player(megamaniac);
	self->game_objects[i++] = megamaniac_create_go_enemy_mover(megamaniac, 300L, go_enemy5_mover_update);
	self->game_objects[i++] = megamaniac_level5_create_go_enemy_indep_choser(megamaniac);
	self->game_objects[i++] = megamaniac_level5_create_go_enemy_indep_controller(megamaniac);

	i += megamaniac_create_standard_enemy_formation(self, megamaniac, i, GO_TYPE_ENEMY5, LEVEL5_ENEMY_ROW_COUNT, LEVEL5_ENEMY_ROW_ODD_COUNT, LEVEL5_ENEMY_ROW_EVEN_COUNT, LEVEL5_ENEMY_HORIZONTAL_SPACING, LEVEL5_ENEMY_VERTICAL_SPACING, true, NULL);

	self->game_objects[i++] = megamaniac_create_go_bomb_dropper(megamaniac);
	self->game_objects[i++] = megamaniac_level5_create_go_bullet_controller(megamaniac);

	self->paused = false;
}


//-------------------------------------------------------
// Game Object Creation Functions
//-------------------------------------------------------

game_object_p megamaniac_level5_create_go_enemy_indep_controller(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_object_p go_enemy_indep_controller = create_null_game_object(GO_TYPE_ENEMY5_INDEP_CONTROLLER, 300L, go_enemy_indep_controller_update);

	if (NULL == go_enemy_indep_controller) {
		return NULL;
	}

	go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data = malloc(sizeof(go_additional_data_enemy_indep_controller_t));

	if (NULL == go_enemy_indep_controller_data) {
		destroy_game_object(go_enemy_indep_controller);
		go_enemy_indep_controller = NULL;

		return NULL;
	}

	go_enemy_indep_controller_data->player = find_game_object_by_type(GO_TYPE_PLAYER, megamaniac->current_level->game_objects, megamaniac->current_level->game_object_count, NULL);
	go_enemy_indep_controller_reset_data(go_enemy_indep_controller_data, NULL);

	go_enemy_indep_controller->additional_data = go_enemy_indep_controller_data;
	go_enemy_indep_controller->active = false;

	return go_enemy_indep_controller;
}

game_object_p megamaniac_level5_create_go_enemy_indep_choser(game_p megamaniac) {
	assert(NULL != megamaniac);

	int interval = LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MIN + (rand() % (LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MAX - LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MIN + 1));

	return create_null_game_object(GO_TYPE_ENEMY5_INDEP_CHOSER, interval, go_enemy_indep_choser_update);
}

game_object_p megamaniac_level5_create_go_bullet_controller(game_p megamaniac) {
	assert(NULL != megamaniac);

	return create_null_game_object(GO_TYPE_BULLET_CONTROLLER, 0L, go_bullet_controller_update);
}


//-------------------------------------------------------
// Game Objects Object Methods
//-------------------------------------------------------

bool go_enemy5_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	bool did_update = false;
	game_object_p go_current = NULL;

	for (int i = 0; i < game->current_level->game_object_count; ++i) {
		go_current = game->current_level->game_objects[i];

		if ((NULL != go_current) && !(go_current->recycle) && (GO_TYPE_ENEMY5 == go_current->type)) {
			did_update = megamaniac_move_enemy(go_current, game) || did_update;
		}
	}

	return did_update;
}

bool go_enemy_indep_choser_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);
	
	if (!timer_expired(self->timer)) {
		return false;
	}
	
	game_object_p go_enemy_indep_controller = find_game_object_by_type(GO_TYPE_ENEMY5_INDEP_CONTROLLER, game->current_level->game_objects, game->current_level->game_object_count, NULL);
	go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data = (go_additional_data_enemy_indep_controller_p) go_enemy_indep_controller->additional_data;

	if ((NULL != go_enemy_indep_controller_data->enemy) && go_enemy_indep_controller_data->enemy->active) {
		// active independent enemy
		// choser deactivates
		self->active = false;

		return false;
	}

	int buffer_size = 10;
	int enemy_count = 0;
	game_object_p* enemies = malloc(buffer_size * sizeof(game_object_p));

	for (int i = 0, offset = 0; i < game->current_level->game_object_count; ++i) {
		if ((NULL != game->current_level->game_objects[i]) && !(game->current_level->game_objects[i]->recycle) && megamaniac_go_is_enemy(game->current_level->game_objects[i])) {
			++enemy_count;

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

	go_enemy_indep_controller_reset_data(go_enemy_indep_controller_data, enemies[rand() % enemy_count]);
	go_enemy_indep_controller_calc_curve(go_enemy_indep_controller_data, game);
	go_enemy_indep_controller_data->enemy->type = GO_TYPE_ENEMY5_INDEP;

	self->active = false;
	go_enemy_indep_controller->active = true;
	reset_timer(go_enemy_indep_controller->timer);

	free(enemies);

	return false;
}

bool go_enemy_indep_controller_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (!timer_expired(self->timer)) {
		return false;
	}

	bool did_update = false;
	go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data = (go_additional_data_enemy_indep_controller_p) self->additional_data;

	// running, but no enemy set
	// reset data, activate choser, decativate controller
	if ((NULL == go_enemy_indep_controller_data->enemy) || !(go_enemy_indep_controller_data->enemy->active)) {
		go_enemy_indep_controller_reset_data(go_enemy_indep_controller_data, NULL);

		game_object_p go_enemy_indep_choser = find_game_object_by_type(GO_TYPE_ENEMY5_INDEP_CHOSER, game->current_level->game_objects, game->current_level->game_object_count, NULL);
		go_enemy_indep_choser->active = true;
		go_enemy_indep_choser->timer->milliseconds = LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MIN + (rand() % (LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MAX - LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MIN + 1));
		reset_timer(go_enemy_indep_choser->timer);

		self->active = false;

		return false;
	}

	// running, enemy set, group limit not reached
	if (!(go_enemy_indep_controller_data->reached_group_limit)) {
		// running, enemy set, group limit not reached, lane not left
		if (!(go_enemy_indep_controller_data->left_lane)) {
			go_enemy_indep_controller_data->enemy->y += 1;
			go_enemy_indep_controller_data->left_lane = true;

			did_update = true;
		}
		else {
			// continue reaching group limit
			go_enemy_indep_controller_data->enemy->dx = -1.;
			go_enemy_indep_controller_data->enemy->dy = 0.;

			did_update = megamaniac_move_enemy(go_enemy_indep_controller_data->enemy, game);

			if (did_update) {
				// check if reached group limit
				go_enemy_indep_controller_data->reached_group_limit = go_enemy_indep_controller_reached_group_limit(go_enemy_indep_controller_data, game);

				if (go_enemy_indep_controller_data->reached_group_limit) {
					// start bezier movement
					go_enemy_indep_controller_calc_curve(go_enemy_indep_controller_data, game);
				}
			}
		}
	}
	else {
		// running, enemy set, group limit reached, do bezier movement
		// first, check if player moved
		// if so, generate new bezier to new player location
		if (go_enemy_indep_controller_did_player_move(go_enemy_indep_controller_data)) {
			go_enemy_indep_controller_data->player_previous_x = go_enemy_indep_controller_data->player->x;
			go_enemy_indep_controller_data->player_previous_y = go_enemy_indep_controller_data->player->y;
			go_enemy_indep_controller_data->player_position_did_change = true;
			go_enemy_indep_controller_data->player_position_last_change_time = get_current_time();
		}
		
		if (go_enemy_indep_controller_data->player_position_did_change && ((get_current_time() - go_enemy_indep_controller_data->player_position_last_change_time) >= 1.)) {
			go_enemy_indep_controller_calc_curve(go_enemy_indep_controller_data, game);
			go_enemy_indep_controller_data->player_position_did_change = false;
		}

		did_update = go_enemy_indep_controller_move_enemy(go_enemy_indep_controller_data, game);
	}

	if (did_update) {
		// check player collision
		game_object_p go_player = find_game_object_by_type(GO_TYPE_PLAYER, game->current_level->game_objects, game->current_level->game_object_count, NULL);

		if (megamaniac_test_enemy_player_collision(go_enemy_indep_controller_data->enemy, go_player, false, true, game)) {
			go_enemy_indep_controller_reset_data(go_enemy_indep_controller_data, NULL);
			
			game_object_p go_enemy_indep_choser = find_game_object_by_type(GO_TYPE_ENEMY5_INDEP_CHOSER, game->current_level->game_objects, game->current_level->game_object_count, NULL);
			go_enemy_indep_choser->active = true;
			go_enemy_indep_choser->timer->milliseconds = LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MIN + (rand() % (LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MAX - LEVEL5_ENEMY_START_INDIVIDUAL_DELAY_MIN + 1));
			reset_timer(go_enemy_indep_choser->timer);

			self->active = false;

			did_update = true;
		}
	}

	return did_update;
}

bool go_bullet_controller_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

#ifdef GAME_DEBUG
	// enable 'y' key as alternative for 'z' in debug mode, because 'Deutsche Tastatur'!
	bool bullet_control_pressed = ('y' == update->key) || ('z' == update->key) || ('c' == update->key);
#else
	bool bullet_control_pressed = ('z' == update->key) || ('c' == update->key);
#endif

	if (bullet_control_pressed) {
		game_object_p go_top_bullet = NULL;

#ifdef GAME_DEBUG
		// find top bullet
		// only in debug mode because otherwise you can only shot one bullet
		double top_x = 0;

		for (int i = 0; i < game->current_level->game_object_count; ++i) {
			if ((NULL != game->current_level->game_objects[i]) && !(game->current_level->game_objects[i]->recycle) && (GO_TYPE_BULLET == game->current_level->game_objects[i]->type)) {
				if (game->current_level->game_objects[i]->x > top_x) {
					top_x = game->current_level->game_objects[i]->x;
					go_top_bullet = game->current_level->game_objects[i];
				}
			}
		}
#else
		go_top_bullet = find_game_object_by_type(GO_TYPE_BULLET, game->current_level->game_objects, game->current_level->game_object_count, NULL);
#endif

		if (NULL == go_top_bullet) {
			return false;
		}

		double angle = atan2(-1 * go_top_bullet->dy, go_top_bullet->dx);
		double s = go_top_bullet->dy / sin(angle);

		
		if ('c' == update->key) {
			angle -= .1745329;

			// TODO: workaround, doesn't work with 0 as limit
			if (angle < 0.1) {
				angle = 0.01;
			}
		}
		else {
			angle += .1745329;

			if (angle > M_PI) {
				angle = M_PI;
			}
		}

		go_top_bullet->dx = -1 * s * cos(angle);
		go_top_bullet->dy = s * sin(angle);
	}

	return false;
}


//-------------------------------------------------------
// Helper Functions
//-------------------------------------------------------

void go_enemy_indep_controller_reset_data(go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data, game_object_p go_enemy) {
	assert(NULL != go_enemy_indep_controller_data);
	assert((NULL == go_enemy) || (GO_TYPE_ENEMY5 == go_enemy->type));

	go_enemy_indep_controller_data->enemy = go_enemy;

	go_enemy_indep_controller_data->left_lane = false;
	go_enemy_indep_controller_data->reached_group_limit = false;

	go_enemy_indep_controller_data->player_previous_x = go_enemy_indep_controller_data->player->x;
	go_enemy_indep_controller_data->player_previous_y = go_enemy_indep_controller_data->player->y;
	go_enemy_indep_controller_data->player_position_did_change = false;
	go_enemy_indep_controller_data->player_position_last_change_time = get_current_time();

	go_enemy_indep_controller_data->start_x = 0.;
	go_enemy_indep_controller_data->start_y = 0.;
	go_enemy_indep_controller_data->control_x = 0.;
	go_enemy_indep_controller_data->control_y = 0.;
	go_enemy_indep_controller_data->target_x = go_enemy_indep_controller_data->player->x;
	go_enemy_indep_controller_data->target_y = go_enemy_indep_controller_data->player->y;
	go_enemy_indep_controller_data->t = 0;
}

void go_enemy_indep_controller_calc_curve(go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data, game_p game) {
	assert(NULL != go_enemy_indep_controller_data);
	assert(NULL != game);
	
	int start_x_100 = (int) round(go_enemy_indep_controller_data->enemy->x * 100);
	int start_y_100 = (int) round(go_enemy_indep_controller_data->enemy->y * 100);

	// int target_x_100 = (int) round(go_enemy_indep_controller_data->player->x * 100);
	int target_y_100 = (int) round(go_enemy_indep_controller_data->player->y * 100);

	double control_x = 0;
	double control_y = 0;

	char bias = rand() % 2;
	
	if (0 == bias) {
		// TODO: doesn't scale on resize
		control_x = (rand() % (GAME_MAX((start_x_100 - 3000), 100))) / 100.;
	}
	else {
		// TODO: doesn't scale on resize
		control_x = (GAME_MIN((start_x_100 + 3000), game->screen_width) + (rand() % ((game->screen_width * 100) - start_x_100))) / 100.;
	}

	control_y = (start_y_100 + (rand() % (GAME_MAX((target_y_100 - start_y_100 + 100), 0)))) / 100.;

	go_enemy_indep_controller_data->start_x = go_enemy_indep_controller_data->enemy->x;
	go_enemy_indep_controller_data->start_y = go_enemy_indep_controller_data->enemy->y;

	go_enemy_indep_controller_data->target_x = go_enemy_indep_controller_data->player->x;
	go_enemy_indep_controller_data->target_y = go_enemy_indep_controller_data->player->y;

	go_enemy_indep_controller_data->control_x = control_x;
	go_enemy_indep_controller_data->control_y = control_y;

	go_enemy_indep_controller_data->t = 0;
}

bool go_enemy_indep_controller_did_player_move(go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data) {
	assert(NULL != go_enemy_indep_controller_data);
	
	return  (go_enemy_indep_controller_data->player->x != go_enemy_indep_controller_data->target_x) ||
		(go_enemy_indep_controller_data->player->y != go_enemy_indep_controller_data->target_y);
}

bool go_enemy_indep_controller_move_enemy(go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data, game_p game) {
	assert(NULL != go_enemy_indep_controller_data);
	assert(NULL != go_enemy_indep_controller_data->enemy);

	bool did_move = false;

	if (go_enemy_indep_controller_data->t > 100) {
		// target reached but still active
		// calc new target
		go_enemy_indep_controller_calc_curve(go_enemy_indep_controller_data, game);
	}

	double current_x = go_enemy_indep_controller_data->enemy->x;
	double current_y = go_enemy_indep_controller_data->enemy->y;

	int current_screen_x = (int) round(current_x);
	int current_screen_y = (int) round(current_y);

	double t = go_enemy_indep_controller_data->t / 100.;
	double t_sqr = pow(t, 2);

	double p0_x = go_enemy_indep_controller_data->start_x;
	double p0_y = go_enemy_indep_controller_data->start_y;
	double p1_x = go_enemy_indep_controller_data->control_x;
	double p1_y = go_enemy_indep_controller_data->control_y;
	double p2_x = go_enemy_indep_controller_data->target_x;
	double p2_y = go_enemy_indep_controller_data->target_y;

	double new_x = (p0_x - 2 * p1_x + p2_x) * t_sqr + (-2 * p0_x + 2 * p1_x) * t + p0_x;
	double new_y = (p0_y - 2 * p1_y + p2_y) * t_sqr + (-2 * p0_y + 2 * p1_y) * t + p0_y;

	int new_screen_x = (int) round(new_x);
	int new_screen_y = (int) round(new_y);

	go_enemy_indep_controller_data->enemy->x = new_x;
	go_enemy_indep_controller_data->enemy->y = new_y;

	go_enemy_indep_controller_data->t += 2;

	did_move = (new_screen_x != current_screen_x) || (new_screen_y != current_screen_y);

	if (did_move) {
		megamaniac_test_enemy_player_collision(go_enemy_indep_controller_data->enemy, go_enemy_indep_controller_data->player, false, true, game);
	}

	return did_move;
}

bool go_enemy_indep_controller_reached_group_limit(go_additional_data_enemy_indep_controller_p go_enemy_indep_controller_data, game_p game) {
	assert(NULL != go_enemy_indep_controller_data);
	assert(NULL != game);

	int enemy_count = 0;
	rect_t indep_enemy_rect = {
		.x0 = (int) round(go_enemy_indep_controller_data->enemy->x - 5),
		.y0 = (int) round(go_enemy_indep_controller_data->enemy->y - 3),
		.x1 = (int) round(go_enemy_indep_controller_data->enemy->x + 5),
		.y1 = (int) round(go_enemy_indep_controller_data->enemy->y + 3)
	};
	rect_t other_enemy_rect = {0, 0, 0, 0};
	rect_p indep_enemy_rect_ptr = &indep_enemy_rect;
	rect_p other_enemy_rect_ptr = &other_enemy_rect;

	for (int i = 0; i < game->current_level->game_object_count; ++i) {
		if ((NULL != game->current_level->game_objects[i]) && (go_enemy_indep_controller_data->enemy != game->current_level->game_objects[i]) && !(game->current_level->game_objects[i]->recycle) && megamaniac_go_is_enemy(game->current_level->game_objects[i])) {
			++enemy_count;

			other_enemy_rect.x0 = (int) round(game->current_level->game_objects[i]->x);
			other_enemy_rect.y0 = (int) round(game->current_level->game_objects[i]->y);
			other_enemy_rect.x1 = other_enemy_rect.x0;
			other_enemy_rect.y1 = other_enemy_rect.y0;

			if ((indep_enemy_rect_ptr->x0 <= other_enemy_rect_ptr->x0) &&
				(other_enemy_rect_ptr->x0 <= indep_enemy_rect_ptr->x1) &&
				(indep_enemy_rect_ptr->y0 <= other_enemy_rect_ptr->y0) &&
				(other_enemy_rect_ptr->y0 <= indep_enemy_rect_ptr->y1)) {
				return false;
			}
		}
	}

	return true;
}
