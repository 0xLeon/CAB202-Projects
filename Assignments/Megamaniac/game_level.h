#ifndef LEVEL_H_
#define LEVEL_H_

#include <stdbool.h>
#include "game_types.h"
#include "game_object.h"

//-------------------------------------------
// Type Definitions
//-------------------------------------------

typedef void (*void_level_func_p)(game_level_p self, game_p game);
typedef bool (*update_level_func_p)(game_level_p self, game_update_p update, game_p game);
typedef struct game_level {
	int game_object_count;
	game_object_p* game_objects;

	// methods
	void_level_func_p load;
	update_level_func_p update;
	void_level_func_p unload;
} game_level_t;
typedef game_level_t* game_level_p;


//-------------------------------------------
// Game Level Functions
//-------------------------------------------

game_level_p create_level(int initial_game_object_count);

bool level_set_game_object_count(game_level_p level, int new_game_object_count);

bool level_add_game_object(game_level_p level, game_object_p game_object);

bool update_level(game_level_p level, game_update_p update, game_p game);

void draw_level(game_level_p game_level);

void recycle_level(game_level_p game_level);

void destroy_level(game_level_p game_level);


void level_default_unload(game_level_p self, game_p game);

#endif
