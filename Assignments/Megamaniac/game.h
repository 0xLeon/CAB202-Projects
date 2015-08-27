#ifndef GAME_H_
#define GAME_H_

#ifdef GAME_DEBUG
#define NDEBUG
#else
#define _DEBUG
#endif

#include <stdbool.h>
#include "cab202_timers.h"
#include "game_types.h"
#include "game_timer.h"
#include "game_level.h"
#include "game_object.h"

//-------------------------------------------
// Type Definitions
//-------------------------------------------

typedef struct rect {
	int x0;
	int y0;
	int x1;
	int y1;
} rect_t;
typedef rect_t* rect_p;

typedef struct game {
	int screen_width;
	int screen_height;
	timer_id framerate_timer;
	bool running;
	bool redraw;

	int game_object_count;
	game_object_p* game_objects;

	int level_count;
	game_level_p* levels;
	game_level_p current_level;
} game_t;
typedef game_t* game_p;


//-------------------------------------------
// Game Functions
//-------------------------------------------

game_p create_game(double framerate, int initial_game_object_count, int initial_level_count);

bool game_set_game_object_count(game_p game, int new_game_object_count);

bool set_level_count(game_p game, int new_level_count);

void start_game_loop(game_p game);

bool update_game(game_p game);

void draw_game(game_p game);

void recycle_game(game_p game);

void destroy_game(game_p game);


//-------------------------------------------
// Colliding Functions
//-------------------------------------------

bool did_collide_box_box(rect_t box_a, rect_t box_b);

bool did_collide_box_game_object(rect_t box, game_object_p game_object);

bool did_collide_game_object_game_object(game_object_p game_object_a, game_object_p game_object_b);


//-------------------------------------------
// Helper Functions
//-------------------------------------------

bool ibetween(int x, int lower, int upper);

bool dbetween(double x, double lower, double upper);

#endif
