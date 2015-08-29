#ifndef GAME_RESIZE_H_
#define GAME_RESIZE_H_

#include "game_types.h"

typedef struct game_resize_descriptor {
	int old_width;
	int old_height;

	int new_width;
	int new_height;

	double width_ratio;
	double height_ratio;
} game_resize_descriptor_t;
typedef game_resize_descriptor_t* game_resize_descriptor_p;

#endif
