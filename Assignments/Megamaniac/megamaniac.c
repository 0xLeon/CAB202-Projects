#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "cab202_graphics.h"
#include "game.h"
#include "megamaniac.h"
#include "megamaniac_go_types.h"
#include "megamaniac_go_data.h"
#include "megamaniac_game_objects.h"
#include "megamaniac_level1.h"
#include "megamaniac_level2.h"
#include "megamaniac_level3.h"
#include <curses.h>

#define MEGAMANIAC_CREDITS	"Stefan Hahn (n9529977)"
#define MEGAMANIAC_START_SCORE	0
#define MEGAMANIAC_START_LIVES	3

int main(int argc, char* argv[]) {
	setup_screen();
	clear_screen();

	start_color();
	init_color(COLOR_GREEN, 518, 875, 31);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);

	attron(A_BOLD);
	attron(COLOR_PAIR(1));
	//---------------------------------------------
	start_megamaniac();
	//---------------------------------------------
	attroff(COLOR_PAIR(1));
	attroff(A_BOLD);
	
	cleanup_screen();
	
	return 0;
}

void start_megamaniac() {
	game_p megamaniac = setup_megamaniac();

	start_game_loop(megamaniac);
	
	destroy_game(megamaniac);
}

game_p setup_megamaniac() {
	game_p megamaniac = create_game(10., 9, 5);
	int i = 0;

	megamaniac->game_objects[i++] = megamaniac_create_quit_checker(megamaniac);
	megamaniac->game_objects[i++] = megamaniac_create_restarter(megamaniac);
	megamaniac->game_objects[i++] = megamaniac_create_pauser(megamaniac);
	megamaniac->game_objects[i++] = megamaniac_create_level_changer(megamaniac);
	megamaniac->game_objects[i++] = megamaniac_create_line(megamaniac);
	megamaniac->game_objects[i++] = megamaniac_create_credits(megamaniac, MEGAMANIAC_CREDITS);
	megamaniac->game_objects[i++] = megamaniac_create_score(megamaniac, MEGAMANIAC_START_SCORE);
	megamaniac->game_objects[i++] = megamaniac_create_lives(megamaniac, MEGAMANIAC_START_LIVES);
	// megamaniac->game_objects[i++] = setup_go_score_updater(megamaniac);

	i = 0;
	megamaniac->levels[i++] = level1_create_level(megamaniac);
	megamaniac->levels[i++] = level2_create_level(megamaniac);
	megamaniac->levels[i++] = level3_create_level(megamaniac);

	megamaniac->current_level = megamaniac->levels[0];
	
	if (NULL != megamaniac->current_level->load) {
		megamaniac->current_level->load(megamaniac->current_level, megamaniac);
	}
	
	return megamaniac;
}
