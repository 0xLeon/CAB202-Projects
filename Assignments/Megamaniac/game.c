#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "cab202_graphics.h"
#include "game_types.h"
#include "game.h"

//-------------------------------------------
// Game Functions
//-------------------------------------------

game_p create_game(double framerate, int initial_game_object_count, int initial_level_count) {
	assert(framerate > 0.);
	assert(initial_game_object_count > -1);
	
	game_p game = malloc(sizeof(game_t));
	
	if (NULL == game) {
		return NULL;
	}
	
	memset(game, 0, sizeof(game_t));
	
	
	get_screen_size(game->screen_width, game->screen_height);
	game->running = true;
	game->redraw = true;

	game->framerate_timer = game_create_timer((long) round(1000. / framerate));

	if (NULL == game->framerate_timer) {
		free(game);
		return NULL;
	}
	
	game->game_object_count = initial_game_object_count;
	game->game_objects = NULL;
	
	if (game->game_object_count > 0) {
		game->game_objects = malloc(game->game_object_count * sizeof(game_object_p));
		
		if (NULL == game->game_objects) {
			free(game->framerate_timer);
			free(game);
			
			return NULL;
		}
		
		memset(game->game_objects, 0, game->game_object_count * sizeof(game_object_p));
	}

	game->level_count = initial_level_count;
	game->levels = NULL;
	game->current_level = NULL;

	if (game->level_count > 0) {
		game->levels = malloc(game->level_count * sizeof(game_level_p));

		if (NULL == game->levels) {
			if (NULL != game->game_objects) {
				free(game->game_objects);
			}

			free(game->framerate_timer);
			free(game);

			return NULL;
		}

		memset(game->levels, 0, game->level_count * sizeof(game_level_p));
	}

	game->resize = NULL;

	srand(time(NULL));
	
	return game;
}

bool game_set_game_object_count(game_p game, int new_game_object_count) {
	assert(NULL != game);
	assert(new_game_object_count > 0);

	game_object_p* new_game_objects = realloc(game->game_objects, new_game_object_count * sizeof(game_object_p));

	if (NULL == new_game_objects) {
		return false;
	}

	// int game_object_count_delta = new_game_object_count - game->game_object_count;

	// TODO: implement to free game objects if decreasing game object count
	// if (game_object_count_delta < 0) {
	// 	for (int i = new_game_object_count - 1; l = game->game_object_count - 1; i < l; ++i) {
	//		if (NULL != game->game_objects[i]) {
	// 			destroy_game_object(game->game_objects[i]);
	//		}
	// 	}
	// }

	game->game_objects = new_game_objects;
	game->game_object_count = new_game_object_count;

	return true;
}

bool set_level_count(game_p game, int new_level_count) {
	assert(NULL != game);
	assert(new_level_count > -1);

	// find level by address


	// TODO: implement
	// get current level and save index, use new address
	return false;
}

void start_game_loop(game_p game) {
	assert(NULL != game);

	int new_screen_width = game->screen_width;
	int new_screen_height = game->screen_height;
	int* new_screen_width_ptr = &new_screen_width;
	int* new_screen_height_ptr = &new_screen_height;
	
	// TODO: separate and parallelize
	do {
		if (NULL != game->resize) {
			get_screen_size_(new_screen_width_ptr, new_screen_height_ptr);

			if ((new_screen_width != game->screen_width) || (new_screen_height != game->screen_height)) {
				game_resize_descriptor_t resize_descriptor = {
					.old_width = game->screen_width,
					.old_height = game->screen_height,
					.new_width = new_screen_width,
					.new_height = new_screen_height,
					.width_ratio = ((double) new_screen_width) / game->screen_width,
					.height_ratio = ((double) new_screen_height) / game->screen_height
				};
				game_resize_descriptor_p resize_descriptor_ptr = &resize_descriptor;

				game->screen_width = new_screen_width;
				game->screen_height = new_screen_height;

				game->resize(game, resize_descriptor_ptr);

				// TODO: resize inactive levels?
				// for (int i = 0; i < game->level_count; ++i) {
				// 	if ((NULL != game->levels[i]) && (NULL != game->levels[i]->resize)) {
				// 		game->levels[i]->resize(game->levels[i], resize_descriptor_ptr, game);
				// 	}
				// }

				game->current_level->resize(game->current_level, resize_descriptor_ptr, game);

				update_game(game);

				game->redraw = true;
			}
		}

		if (!timer_expired(game->framerate_timer)) {
			if (game->redraw) {
				clear_screen();
				draw_game(game);
				show_screen();
			}
		}

		game->redraw = update_game(game);

		timer_pause(1L);
	}
	while (game->running);
}

bool update_game(game_p game) {
	assert(NULL != game);
	
	bool didUpdate = false;
	game_update_t update;
	game_update_p update_ptr = &update;
	
	if (game->running) {
		update.key = get_char();
		
		// TODO: parallelize this loop
		for (int i = 0; i < game->game_object_count; ++i) {
			if (NULL != game->game_objects[i]) {
				if (game->game_objects[i]->recycle) {
					destroy_game_object(game->game_objects[i]);
					game->game_objects[i] = NULL;
				}
				else if ((NULL != game->game_objects[i]->update) && game->game_objects[i]->active) {
					didUpdate = game->game_objects[i]->update(game->game_objects[i], update_ptr, game, NULL) || didUpdate;
				}
			}
		}

		if (NULL != game->current_level) {
			if (!game->current_level->paused) {
				didUpdate = update_level(game->current_level, update_ptr, game) || didUpdate;
			}
			else {
				recycle_level(game->current_level);
			}
		}
	}
	
	return didUpdate;
}

void draw_game(game_p game) {
	assert(NULL != game);
	
	if (game->running) {
		// TODO: maybe parallelize this loop
		for (int i = 0; i < game->game_object_count; ++i) {
			if ((NULL != game->game_objects[i]) && game->game_objects[i]->active && !(game->game_objects[i]->recycle) && game->game_objects[i]->visual) {
				draw_game_object(game->game_objects[i]);
			}
		}

		if (NULL != game->current_level) {
			draw_level(game->current_level);
		}
	}
}

void recycle_game(game_p game) {
	assert(NULL != game);

	for (int i = 0; i < game->game_object_count; ++i) {
		if ((NULL != game->game_objects[i]) && game->game_objects[i]->recycle) {
			destroy_game_object(game->game_objects[i]);
			game->game_objects[i] = NULL;
		}
	}
}

void destroy_game(game_p game) {
	assert(NULL != game);
	
	if (game->game_object_count > 0) {
		for (int i = 0; i < game->game_object_count; ++i) {
			if (NULL != game->game_objects[i]) {
				destroy_game_object(game->game_objects[i]);
				game->game_objects[i] = NULL;
			}
		}
		
		free(game->game_objects);
		game->game_objects = NULL;
	}

	if (game->level_count > 0) {
		for (int i = 0; i < game->level_count; ++i) {
			if (NULL != game->levels[i]) {
				destroy_level(game->levels[i]);
				game->levels[i] = NULL;
			}
		}

		free(game->levels);
		game->levels = NULL;
	}

	free(game->framerate_timer);
	game->framerate_timer = NULL;

	free(game);
	game = NULL;
}


//-------------------------------------------
// Colliding Functions
//-------------------------------------------

bool did_collide_box_box(rect_p box_a, rect_p box_b) {
	return !((box_b->x0 > box_a->x1) || (box_b->x1 < box_a->x0) || (box_b->y0 > box_a->y1) || (box_b->y1 < box_a->y0));
}

bool did_collide_box_game_object(rect_p box, game_object_p game_object) {
	// TODO: implement
	
	return false;
}

bool did_collide_game_object_game_object(game_object_p game_object_a, game_object_p game_object_b) {
	// TODO: implement
	
	return false;
}


//-------------------------------------------
// Helper Functions
//-------------------------------------------

bool ibetween(int x, int lower, int upper) {
	return ((x - lower) <= (upper - lower));
}

bool dbetween(double x, double lower, double upper) {
	return ((x - lower) <= (upper - lower));
}
