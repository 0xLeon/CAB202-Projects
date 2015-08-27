#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <stdbool.h>
#include "cab202_timers.h"
#include "game_types.h"
#include "game_update.h"

#define GO_TYPE_NONE	-1
#define GO_TYPE_NULL	0

typedef bool (*game_object_update_f)(game_object_p self, game_update_p update, game_p game, game_level_p level);
typedef struct game_object {
	int type;
	double x;
	double y;
	int width;
	int height;
	double dx;
	double dy;
	bool active;
	bool visual;
	char* bitmap;
	timer_id timer;
	bool recycle;

	// methods
	game_object_update_f update;

	// additional data
	void* additional_data;
} game_object_t;
typedef game_object_t* game_object_p;

typedef struct game_object_descriptor {
	game_object_p game_object;
	int index;
} game_object_descriptor_t;
typedef game_object_descriptor_t* game_object_descriptor_p;

//-------------------------------------------
// Game Object Functions
//-------------------------------------------

game_object_p create_game_object(int type, double x, double y, int width, int height, double dx, double dy, char* bitmap, long interval, game_object_update_f go_update);

game_object_p create_null_game_object(int type, long interval, game_object_update_f go_update);

game_object_p create_dynamic_string_game_object(int type, double x, double y, double dx, double dy, long interval, game_object_update_f go_update, int buffer_size, const char* format, ...);

game_object_p create_static_string_game_object(int type, double x, double y, double dx, double dy, long interval, game_object_update_f go_update, char* s);

void draw_game_object(game_object_p game_object);

void destroy_game_object(game_object_p game_object);

game_object_p find_game_object_by_type(int type, game_object_p* game_objects, int game_object_count, game_object_descriptor_p go_descriptor);

bool move_game_object(game_object_p game_object);

#endif
