#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include "cab202_sprites.h"
#include "cab202_timers.h"

#define N_ALIENS (10)

typedef struct Game {
	bool over;
	int score;
	int event_loop_delay;
	
	sprite_id ship;
	
	sprite_id bullet;
	timer_id bullet_timer;
	
	sprite_id aliens[N_ALIENS];
	timer_id alien_timer;
} Game;

#endif
