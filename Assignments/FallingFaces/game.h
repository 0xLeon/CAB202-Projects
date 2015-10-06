#ifndef GAME_H_
#define GAME_H_

#include <stdint.h>
#include "pgraphics.h"

typedef struct level {
	void (*load)(level_p, game_p);
	void (*draw)(level_p, game_p);
	uint8_t (*update)(level_p, game_p);
	void (*unload)(level_p, game_p);
} level_t;
typedef level_t* level_p;

typedef struct game {
	level_p current_level;
	level_p levels[4];

	psprite_p player;
	psprite_p faces[3];
	uint8_t face_count;

	uint16_t lives;
	uint16_t score;
} game_t;
typedef game_t* game_p;

#endif
