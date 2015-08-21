#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"

#define	CYCLE_DELTA	70
#define ZOMBIE_START_X	50.0
#define ZOMBIE_START_Y	4.0
#define ZOMBIE_DELTA_X	0.8
#define ZOMBIE_DELTA_Y	0
#define MAX_LAPS	6

// Structure to hold game data.
typedef struct Game {
	timer_id timer;
	sprite_id zombie;
	int laps;
	bool over;
	// int screen_width;
	// int screen_height;
} Game;

void setup_zombie(Game* game) {
	assert(NULL != game);
	
	char* buf = malloc(sizeof(char));
	
	if (NULL == buf) {
		return;
	}
	
	memset(buf, 'Z', 1);
	
	// get_screen_size(game->screen_width, game->screen_height);
	game->laps = 0;
	game->over = false;
	game->timer = create_timer(CYCLE_DELTA);
	game->zombie = create_sprite(ZOMBIE_START_X, ZOMBIE_START_Y, 1, 1, buf);
	game->zombie->dx = ZOMBIE_DELTA_X;
	game->zombie->dy = ZOMBIE_DELTA_Y;
}

bool update_zombie(Game* game) {
	assert(NULL != game);
	
	if (game->over || !timer_expired(game->timer)) {
		return false;
	}
	
	int zombie_current_x;
	int zombie_new_x;
	
	zombie_current_x = (int) round(game->zombie->x);
	game->zombie->x += game->zombie->dx;
	zombie_new_x = (int) round(game->zombie->x);
	
	// if (game->screen_width < zombie_new_x) {
	// if (zombie_new_x >= game->screen_width) {
	if (zombie_new_x >= screen_width()) {
		zombie_new_x = 0;
		game->zombie->x = 0;
		game->laps++;
		
		// game->over = (MAX_LAPS < game->laps);
		if (game->laps >= MAX_LAPS) {
			game->over = true;
		}
	}
	
	return (zombie_current_x != zombie_new_x);
}

void display_zombie(Game* game) {
	assert(NULL != game);
	
	draw_sprite(game->zombie);
}

// --------- Do not submit the following functions --------- // 
void zombie_wrap() {
	Game game;
	setup_zombie(&game);
	
	while (!game.over) {
		if (update_zombie(&game)) {
			clear_screen();
			display_zombie(&game);
			show_screen();
		}
		
		timer_pause(25);
	}
	
	timer_pause(1500);
}

int main() {
	setup_screen();
	zombie_wrap();
	cleanup_screen();
	
	return 0;
}
