#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "game_types.h"
#include "game_level.h"

game_level_p create_level(int initial_game_object_count) {
	assert(initial_game_object_count > -1);

	game_level_p level = malloc(sizeof(game_level_t));

	if (NULL == level) {
		return NULL;
	}

	memset(level, 0, sizeof(game_level_t));

	level->paused = false;
	level->load = NULL;
	level->update = NULL;
	level->unload = NULL;

	level->game_object_count = initial_game_object_count;
	level->game_objects = NULL;

	if (level->game_object_count > 0) {
		level->game_objects = malloc(level->game_object_count * sizeof(game_object_p));

		if (NULL == level->game_objects) {
			free(level);
			
			return NULL;
		}
		
		memset(level->game_objects, 0, level->game_object_count * sizeof(game_object_p));
	}

	return level;
}

bool level_set_game_object_count(game_level_p level, int new_game_object_count) {
	assert(NULL != level);
	assert(new_game_object_count > -1);

	game_object_p* new_game_objects = realloc(level->game_objects, new_game_object_count * sizeof(game_object_t));

	if (NULL == new_game_objects) {
		return false;
	}

	level->game_objects = new_game_objects;
	level->game_object_count = new_game_object_count;

	return true;
}

bool level_add_game_object(game_level_p level, game_object_p game_object) {
	assert(NULL != level);
	assert(NULL != game_object);

	if (level->game_object_count == 0) {
		if (!level_set_game_object_count(level, 1)) {
			return false;
		}

		level->game_objects[0] = game_object;

		return true;
	}
	else {
		for (int i = 0; i < level->game_object_count; i++) {
			if (NULL == level->game_objects[i]) {
				level->game_objects[i] = game_object;
				
				return true;
			}
		}

		if (!level_set_game_object_count(level, level->game_object_count + 1)) {
			return false;
		}

		level->game_objects[level->game_object_count - 1] = game_object;

		return true;
	}

	return false;
}

bool update_level(game_level_p level, game_update_p update, game_p game) {
	assert(NULL != level);
	assert(NULL != update);
	assert(NULL != game);
	
	bool didUpdate = false;

	if (NULL != level->update) {
		didUpdate = level->update(level, update, game) || didUpdate;
	}

	for (int i = 0; i < level->game_object_count; i++) {
		if (NULL != level->game_objects[i]) {
			if (level->game_objects[i]->recycle) {
				destroy_game_object(level->game_objects[i]);
				level->game_objects[i] = NULL;
			}
			else if ((NULL != level->game_objects[i]->update) && (level->game_objects[i]->active)) {
				didUpdate = level->game_objects[i]->update(level->game_objects[i], update, game, level) || didUpdate;
			}
		}
	}

	return didUpdate;
}

void draw_level(game_level_p level) {
	assert(NULL != level);

	// TODO: maybe parallelize this loop
	for (int i = 0; i < level->game_object_count; i++) {
		if ((NULL != level->game_objects[i]) && level->game_objects[i]->active && level->game_objects[i]->visual) {
			draw_game_object(level->game_objects[i]);
		}
	}
}

void recycle_level(game_level_p game_level) {
	assert(NULL != game_level);

	for (int i = 0; i < game_level->game_object_count; i++) {
		if ((NULL != game_level->game_objects[i]) && game_level->game_objects[i]->recycle) {
			destroy_game_object(game_level->game_objects[i]);
			game_level->game_objects[i] = NULL;
		}
	}
}

void destroy_level(game_level_p level) {
	assert(NULL != level);

	if (level->game_object_count > 0) {
		for (int i = 0; i < level->game_object_count; i++) {
			if (NULL != level->game_objects[i]) {
				destroy_game_object(level->game_objects[i]);
				level->game_objects[i] = NULL;
			}
		}

		free(level->game_objects);
		level->game_objects = NULL;
	}

	free(level);
	level = NULL;
}


void level_default_unload(game_level_p self, game_p game) {
	assert(NULL != self);
	assert(NULL != game);

	for (int i = 0; i < self->game_object_count; i++) {
		if (NULL != self->game_objects[i]) {
			destroy_game_object(self->game_objects[i]);
			self->game_objects[i] = NULL;
		}
	}
}
