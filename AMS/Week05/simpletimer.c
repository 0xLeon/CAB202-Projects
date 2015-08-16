#include <stdbool.h>
#include <stdlib.h>
#include "cab202_graphics.h"
#include "cab202_timers.h"

#define	INTERVAL	1200
#define MAX_SCORE	32
#define TEXT_X		7
#define TEXT_Y		20

typedef struct Game {
	timer_id timer;
	int score;
	bool over;
} Game;

void setup_score(Game* game) {
	if (NULL != game) {
		game->score = 0;
		game->over = false;
		game->timer = create_timer(INTERVAL);
	}
}

bool update_score(Game* game) {
	if (!timer_expired(game->timer)) {
		return false;
	}
	
	game->score++;
	game->over = (game->score == MAX_SCORE);
	
	return true;
}

void display_score(Game* game) {
	draw_formatted(TEXT_X, TEXT_Y, "Score: %d", game->score);
	draw_formatted(TEXT_X, TEXT_Y + 1, "Clock time: %f", get_current_time());
}

// --------- Do not submit the following functions --------- // 
void simple_timer() {
	Game game;
	setup_score(&game);
	
	while (!game.over) {
		if (update_score(&game)) {
			clear_screen();
			display_score(&game);
			show_screen();
		}
		
		timer_pause(100);
	}
	
	timer_pause(1500);
}

int main() {
	setup_screen();
	simple_timer();
	cleanup_screen();
	
	return 0;
}