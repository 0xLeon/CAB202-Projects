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
#include "megamaniac_level6.h"

#define LEVEL6_LEVEL_NAME			"Level 6 - Crazy"
#define LEVEL6_ENEMY_ROW_COUNT			3
#define LEVEL6_ENEMY_ROW_ODD_COUNT		3
#define LEVEL6_ENEMY_ROW_EVEN_COUNT		4
#define LEVEL6_ENEMY_VERTICAL_SPACING		2
#define LEVEL6_ENEMY_HORIZONTAL_SPACING		5

#define LEVEL6_ENEMY_DIRECTION_CHANGE_TIMER_LOWER	750
#define LEVEL6_ENEMY_DIRECTION_CHANGE_TIMER_UPPER	3500
#define LEVEL6_ENEMY_MIN_DISTANCE			10.
#define LEVEL6_ENEMY_SPEED_CHANGE_TIMER_LOWER		1000
#define LEVEL6_ENEMY_SPEED_CHANGE_TIMER_UPPER		5000
#define LEVEL6_ENEMY_SPEED_MIN				3
#define LEVEL6_ENEMY_SPEED_MAX				14
#define LEVEL6_ENEMY_POSITION_EPSLION			.099609375

//-------------------------------------------------------
// Level Loading Methods Forward Declarations
//-------------------------------------------------------

void megamaniac_level6_load(game_level_p self, game_p megamaniac);


//-------------------------------------------------------
// Game Object Creation Functions Forward Declarations
//-------------------------------------------------------

int megamaniac_level6_create_enemies(game_level_p level, game_p megamaniac, int offset);


//-------------------------------------------------------
// Game Objects Object Methods Forward Declarations
//-------------------------------------------------------

bool go_enemy6_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level);

void go_enemy6_destroy(game_object_p self, game_p game, game_level_p level);


//-------------------------------------------------------
// Helper Functions Forward Declarations
//-------------------------------------------------------

void go_enemy6_find_next_waypoint(game_object_p go_enemy, game_p megamaniac);

bool go_enemy6_is_valid_enemy_position(game_object_p enemy, game_object_p* enemies, int enemy_count);

bool go_enemy6_reached_waypoint(game_object_p go_enemy, go_additional_data_enemy6_p go_enemy_data);

double go_enemy6_get_travelled_distance(go_additional_data_enemy6_p go_enemy_data);


//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------
// Level Creation Functions
//-------------------------------------------------------

// TODO: null checks for game objects
game_level_p megamaniac_create_level6(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_level_p level4 = create_level(14);

	if (NULL == level4) {
		return NULL;
	}

	level4->load = megamaniac_level6_load;
	level4->resize = megamaniac_level_default_resize;
	level4->unload = level_default_unload;

	return level4;
}


//-------------------------------------------------------
// Level Loading Methods
//------------------------------------------------------

void megamaniac_level6_load(game_level_p self, game_p megamaniac) {
	assert(NULL != self);
	assert(NULL != megamaniac);

	int i = 0;

	self->game_objects[i++] = megamaniac_create_go_level_name(megamaniac, LEVEL6_LEVEL_NAME);
	self->game_objects[i++] = megamaniac_create_go_player(megamaniac);
	self->game_objects[i++] = megamaniac_create_go_enemy_mover(megamaniac, 300L, go_enemy6_mover_update);
	
	i += megamaniac_level6_create_enemies(self, megamaniac, i);

	self->game_objects[i++] = megamaniac_create_go_bomb_dropper(megamaniac);

#ifdef ENABLE_TRACER
	level_add_game_object(self, megamaniac_create_go_trace_drawer(megamaniac, self->game_objects[i - 2]));
#endif

	self->paused = false;
}


//-------------------------------------------------------
// Game Object Creation Functions
//-------------------------------------------------------

int megamaniac_level6_create_enemies(game_level_p level, game_p megamaniac, int offset) {
	assert(NULL != level);
	assert(NULL != megamaniac);
	assert(offset > -1);
	assert(offset < level->game_object_count);

	int enemy_count = megamaniac_create_standard_enemy_formation(level, megamaniac, offset, GO_TYPE_ENEMY6, LEVEL6_ENEMY_ROW_COUNT, LEVEL6_ENEMY_ROW_ODD_COUNT, LEVEL6_ENEMY_ROW_EVEN_COUNT, LEVEL6_ENEMY_HORIZONTAL_SPACING, LEVEL6_ENEMY_VERTICAL_SPACING, true, NULL);
	int successful_enemy_count = 0;
	int new_offset = offset + enemy_count;

	game_object_p go_enemy = NULL;
	go_additional_data_enemy6_p go_enemy_data = NULL;

	for (int i = offset; i < new_offset; ++i) {
		if ((NULL != level->game_objects[i]) && (GO_TYPE_ENEMY6 == level->game_objects[i]->type)) {
			go_enemy = level->game_objects[i];
			go_enemy_data = malloc(sizeof(go_additional_data_enemy6_t));

			if (NULL == go_enemy_data) {
				destroy_game_object(go_enemy);
				level->game_objects[i] = NULL;

				continue;
			}

			go_enemy_data->direction_change_timer = create_timer(LEVEL6_ENEMY_DIRECTION_CHANGE_TIMER_LOWER + (rand() % (LEVEL6_ENEMY_DIRECTION_CHANGE_TIMER_UPPER - LEVEL6_ENEMY_DIRECTION_CHANGE_TIMER_LOWER + 1)));

			if (NULL == go_enemy_data->direction_change_timer) {
				destroy_game_object(go_enemy);
				free(go_enemy_data);
				level->game_objects[i] = NULL;

				continue;
			}

			go_enemy_data->speed_change_timer = create_timer(LEVEL6_ENEMY_SPEED_CHANGE_TIMER_LOWER + (rand() % (LEVEL6_ENEMY_SPEED_CHANGE_TIMER_UPPER - LEVEL6_ENEMY_SPEED_CHANGE_TIMER_LOWER + 1)));

			if (NULL == go_enemy_data->speed_change_timer) {
				destroy_game_object(go_enemy);
				free(go_enemy_data->direction_change_timer);
				free(go_enemy_data);
				level->game_objects[i] = NULL;

				continue;
			}

			go_enemy->additional_data = go_enemy_data;
			go_enemy->destroy = go_enemy6_destroy;

			go_enemy6_find_next_waypoint(go_enemy, megamaniac);

			successful_enemy_count++;
		}
	}

	return successful_enemy_count;
}


//-------------------------------------------------------
// Game Objects Object Methods
//-------------------------------------------------------

bool go_enemy6_mover_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);
	
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
	go_additional_data_enemy6_p go_enemy_data = NULL;
	bool moved;
	double enemy_x;
	double enemy_y;

	for (int i = 0; i < enemy_count; ++i) {
		if (GO_TYPE_ENEMY6 == enemies[i]->type) {
			go_enemy = enemies[i];
			go_enemy_data = (go_additional_data_enemy6_p) go_enemy->additional_data;
			moved = false;

			if (timer_expired(go_enemy_data->speed_change_timer)) {
				go_enemy_data->speed = GAME_SIGNUM(go_enemy_data->speed) * ((LEVEL6_ENEMY_SPEED_MIN + (rand() % (LEVEL6_ENEMY_SPEED_MAX - LEVEL6_ENEMY_SPEED_MIN + 1))) / 10.);
			}

			if (timer_expired(go_enemy_data->direction_change_timer) && (go_enemy6_get_travelled_distance(go_enemy_data) >= LEVEL6_ENEMY_MIN_DISTANCE)) {
				go_enemy6_find_next_waypoint(go_enemy, game);
				moved = true;
			}

			if (!moved && move_timer_expired) {
				enemy_x = go_enemy->x;
				enemy_y = go_enemy->y;
				moved = megamaniac_move_enemy(go_enemy, game);

				if (moved) {
					game_object_p go_player = find_game_object_by_type(GO_TYPE_PLAYER, game->current_level->game_objects, game->current_level->game_object_count, NULL);
					
					if (!go_enemy6_is_valid_enemy_position(go_enemy, enemies, enemy_count)) {
						// invalid, return to previous point and find new direction
						go_enemy->x = enemy_x;
						go_enemy->y = enemy_y;

						go_enemy6_find_next_waypoint(go_enemy, game);
					}
					else if (go_enemy6_reached_waypoint(go_enemy, go_enemy_data)) {
						go_enemy6_find_next_waypoint(go_enemy, game);
					}

					megamaniac_test_enemy_player_collision(go_enemy, go_player, true, true, game);

					did_update = true;
				}
			}
		}
	}

	free(enemies);

	return did_update;
}

void go_enemy6_destroy(game_object_p self, game_p game, game_level_p level) {
	assert(NULL != self);
	// TODO: second and third parameter always NULL for now
	// assert(NULL != game);
	// assert(NULL != level);

	go_additional_data_enemy6_p go_enemy_data = (go_additional_data_enemy6_p) self->additional_data;

	free(go_enemy_data->direction_change_timer);
	go_enemy_data->direction_change_timer = NULL;

	free(go_enemy_data->speed_change_timer);
	go_enemy_data->speed_change_timer = NULL;
}


//-------------------------------------------------------
// Helper Functions
//-------------------------------------------------------

void go_enemy6_find_next_waypoint(game_object_p go_enemy, game_p megamaniac) {
	assert(NULL != go_enemy);
	assert(GO_TYPE_ENEMY6 == go_enemy->type);
	assert(NULL != megamaniac);

	go_additional_data_enemy6_p go_enemy_data = (go_additional_data_enemy6_p) go_enemy->additional_data;
	double current_x = go_enemy->x;
	double current_y = go_enemy->y;
	double new_x;
	double new_y;

	// TODO: unlikely but possible infinite loop
	do {
		new_x = rand() % (megamaniac->screen_width);
		new_y = rand() % (megamaniac->screen_height - 3);
	}
	while (sqrt(pow(new_x - current_x, 2) + pow(new_y - current_y, 2)) < LEVEL6_ENEMY_MIN_DISTANCE);

	go_enemy_data->previous_x = current_x;
	go_enemy_data->previous_y = current_x;
	go_enemy_data->next_x = new_x;
	go_enemy_data->next_y = new_y;
	go_enemy_data->angle = atan((new_y - go_enemy->y) / (new_x - go_enemy->x));
	go_enemy_data->speed = (((rand() % 2) == 0) ? (1) : (-1)) * (LEVEL6_ENEMY_SPEED_MIN + (rand() % (LEVEL6_ENEMY_SPEED_MAX - LEVEL6_ENEMY_SPEED_MIN + 1))) / 10.;
	
	go_enemy->dx = go_enemy_data->speed * cos(go_enemy_data->angle);
	go_enemy->dy = go_enemy_data->speed * sin(go_enemy_data->angle);

	// TODO not only reset timers, generate new timer with new random interval
	reset_timer(go_enemy_data->direction_change_timer);
	reset_timer(go_enemy_data->speed_change_timer);
}

bool go_enemy6_is_valid_enemy_position(game_object_p enemy, game_object_p* enemies, int enemy_count) {
	assert(NULL != enemy);
	assert(NULL != enemies);
	assert(enemy_count > 0);

	int looped_enemy_x = 0;
	int looped_enemy_y = 0;
	int test_enemy_x0 = ((int) round(enemy->x - 1));
	int test_enemy_y0 = ((int) round(enemy->y - 1));
	int test_enemy_x1 = ((int) round(enemy->x + 1));
	int test_enemy_y1 = ((int) round(enemy->y + 1));

	for (int i = 0; i < enemy_count; ++i) {
		if (!(enemies[i]->recycle) && (enemy != enemies[i])) {
			// if looped enemy is inside viewed enemy's inside save volume, location is invalid
			looped_enemy_x = (int) round(enemies[i]->x);
			looped_enemy_y = (int) round(enemies[i]->y);

			if ((test_enemy_x0 <= looped_enemy_x) && (looped_enemy_x <= test_enemy_x1) && (test_enemy_y0 <= looped_enemy_y) && (looped_enemy_y <= test_enemy_y1)) {
				return false;
			}
		}
	}

	// no enemy was in viewed enemy's safe volume, location is valid
	return true;
}

bool go_enemy6_reached_waypoint(game_object_p go_enemy, go_additional_data_enemy6_p go_enemy_data) {
	assert(NULL != go_enemy);
	assert(NULL != go_enemy_data);

	return (sqrt(pow(go_enemy_data->next_x - go_enemy->x, 2) + pow(go_enemy_data->next_y - go_enemy->y, 2)) < LEVEL6_ENEMY_POSITION_EPSLION);
}

double go_enemy6_get_travelled_distance(go_additional_data_enemy6_p go_enemy_data) {
	assert(NULL != go_enemy_data);

	return sqrt(pow(go_enemy_data->next_x - go_enemy_data->previous_x, 2) + pow(go_enemy_data->next_y - go_enemy_data->previous_y, 2));
}
