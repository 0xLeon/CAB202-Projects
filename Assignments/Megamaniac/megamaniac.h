#ifndef MEGAMANIC_H_
#define MEGAMANIC_H_

#include "game.h"

#define POINTS_PER_ENEMY	30
#define POINTS_BONUS		500

void start_megamaniac();

game_p setup_megamaniac();

game_object_p setup_go_quit_checker(game_p megamaniac);

game_object_p setup_go_restart(game_p megamaniac);

game_object_p setup_go_line(game_p megamaniac);

game_object_p setup_go_credits(game_p megamaniac, char* credits);

game_object_p setup_go_score(game_p megamaniac, int initial_score);

game_object_p setup_go_lives(game_p megamaniac, int initial_lives);

game_object_p setup_go_score_updater(game_p megamaniac);

#endif
