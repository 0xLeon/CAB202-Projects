#ifndef GAME_H_
#define GAME_H_

#include <stdint.h>
#include "pgraphics.h"

#define GAME_SIGNUM(x)		(((x) > 0U) - ((x) < 0U))
#define GAME_ABSOLUTE(x)	((x) * (GAME_SIGNUM(x)))

struct game;
typedef void (*collision_function_p)(struct game*);

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
	collision_function_p face_collision_handlers[3];
	uint8_t face_count;
	uint8_t active_face_count;

	uint16_t lives;
	uint16_t score;

	uint8_t ended;
} game_t;
typedef game_t* game_p;



void default_collision_face_happy(game_p game);

void default_collision_face_angry(game_p game);

void default_collision_face_mad(game_p game);

void check_face_player_collision(game_p game);

uint8_t sprites_collided(psprite_p face, psprite_p player);

inline uint8_t isect(uint8_t x, uint8_t lower, uint8_t upper) {
	return ((x >= lower) && (x <= upper));
}

inline uint8_t inside(uint8_t a0, uint8_t a1, uint8_t b0, uint8_t b1) {
	return ((a0 >= b0) && (a1 <= b1));
}

#endif
