#include "game.h"

int main() {
	setup_game();

	int turn_result = TURN_OK;

	while ( turn_result != TURN_GAME_OVER ) {
		turn_result = play_turn();

		if ( turn_result == TURN_SCREEN_CHANGED ) {
			draw_game();
		}

		timer_pause( 5 );
	}

	cleanup_game();

	return 0;
}
