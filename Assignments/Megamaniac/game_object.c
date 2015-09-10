#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "cab202_graphics.h"
#include "game_types.h"
#include "game_object.h"

//-------------------------------------------
// Game Object Functions
//-------------------------------------------

game_object_p create_game_object(int type, double x, double y, int width, int height, double dx, double dy, char* bitmap, long interval, game_object_update_f go_update) {
	assert(type > GO_TYPE_NONE);
	assert(width > -1);
	assert(height > -1);

	game_object_p game_object = malloc(sizeof(game_object_t));

	if (NULL == game_object) {
		return NULL;
	}

	memset(game_object, 0, sizeof(game_object_t));

	game_object->type = type;
	game_object->x = x;
	game_object->y = y;
	game_object->width = width;
	game_object->height = height;
	game_object->dx = dx;
	game_object->dy = dy;
	game_object->active = true;
	game_object->visual = ((NULL != bitmap) && (width > 0) && (height > 0));
	game_object->bitmap = bitmap;
	game_object->timer = NULL;
	game_object->recycle = false;
	game_object->update = go_update;
	game_object->destroy = NULL;
	game_object->additional_data = NULL;

	if (interval > 0) {
		game_object->timer = create_timer(interval);

		if (NULL == game_object->timer) {
			destroy_game_object(game_object);

			return NULL;
		}
	}

	return game_object;
}

game_object_p create_null_game_object(int type, long interval, game_object_update_f go_update) {
	return create_game_object(type, 0., 0., 0, 0, 0., 0., NULL, interval, go_update);
}

game_object_p create_dynamic_string_game_object(int type, double x, double y, double dx, double dy, long interval, game_object_update_f go_update, int buffer_size, const char* format, ...) {
	assert(buffer_size > 0);
	assert(NULL != format);
	assert(strlen(format) > 0);

	char* buffer = malloc((buffer_size + 1) * sizeof(char));
	va_list args;

	va_start(args, format);

	if (NULL == buffer) {
		return NULL;
	}

	memset(buffer, ' ', buffer_size + 1);
	vsprintf(buffer, format, args);

	return create_game_object(type, x, y, buffer_size - 1, 1, dx, dy, buffer, interval, go_update);
}

game_object_p create_static_string_game_object(int type, double x, double y, double dx, double dy, long interval, game_object_update_f go_update, char* s) {
	assert(NULL != s);
	assert(strlen(s) > 0);

	int s_length = strlen(s);
	char* buffer = malloc(s_length * sizeof(char));

	if (NULL == buffer) {
		return NULL;
	}

	memset(buffer, ' ', s_length);
	memcpy(buffer, s, s_length);

	return create_game_object(type, x, y, s_length, 1, dx, dy, buffer, interval, go_update);
}

void draw_game_object(game_object_p game_object) {
	assert(NULL != game_object);

	if (!(game_object->active) || !(game_object->visual)) {
		return;
	}

	int x = (int) round(game_object->x);
	int y = (int) round(game_object->y);
	int offset = 0;
	char c;

	for (int row = 0; row < game_object->height; ++row) {
		for (int col = 0; col < game_object->width; ++col) {
			c = game_object->bitmap[offset++] & 0xff;

			if (c != ' ') {
				draw_char(x + col, y + row, c);
			}
		}
	}
}

void destroy_game_object(game_object_p game_object) {
	assert(NULL != game_object);

	if (NULL != game_object->destroy) {
		// TODO: game and level are always NULL 'cause we don't have them in this context
		game_object->destroy(game_object, NULL, NULL);
	}

	if (NULL != game_object->bitmap) {
		free(game_object->bitmap);
		game_object->bitmap = NULL;
	}

	if (NULL != game_object->timer) {
		free(game_object->timer);
		game_object->timer = NULL;
	}

	if (NULL != game_object->additional_data) {
		free(game_object->additional_data);
		game_object->additional_data = NULL;
	}

	free(game_object);
	game_object = NULL;
}


game_object_p find_game_object_by_type(int type, game_object_p* game_objects, int game_object_count, game_object_descriptor_p go_descriptor) {
	assert(type > GO_TYPE_NONE);
	assert(NULL != game_objects);
	assert(game_object_count > -1);

	for (int i = 0; i < game_object_count; ++i) {
		if ((NULL != game_objects[i]) && !(game_objects[i]->recycle) && (game_objects[i]->type == type)) {
			if (NULL != go_descriptor) {
				go_descriptor->game_object = game_objects[i];
				go_descriptor->index = i;
			}

			return game_objects[i];
		}
	}

	return NULL;
}

bool move_game_object(game_object_p game_object) {
	assert(NULL != game_object);

	int old_x;
	int old_y;
	int new_x;
	int new_y;

	old_x = (int) round(game_object->x);
	old_y = (int) round(game_object->y);
	game_object->x += game_object->dx;
	game_object->y += game_object->dy;
	new_x = (int) round(game_object->x);
	new_y = (int) round(game_object->y);

	return (old_x != new_x) || (old_y != new_y);
}
