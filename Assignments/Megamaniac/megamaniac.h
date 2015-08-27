#ifndef MEGAMANIC_H_
#define MEGAMANIC_H_

#include "game.h"

#define POINTS_PER_ENEMY	30
#define POINTS_BONUS		500

void start_megamaniac();

game_p setup_megamaniac();

game_object_p megamaniac_create_quiter(game_p megamaniac);

game_object_p megamaniac_create_restarter(game_p megamaniac);

game_object_p megamaniac_create_pauser(game_p megamaniac);

game_object_p megamaniac_create_level_changer(game_p megamaniac);

game_object_p megamaniac_create_line(game_p megamaniac);

game_object_p megamaniac_create_credits(game_p megamaniac, char* credits);

game_object_p megamaniac_create_score(game_p megamaniac, int initial_score);

game_object_p megamaniac_create_lives(game_p megamaniac, int initial_lives);

game_object_p megamaniac_create_score_cheater(game_p megamaniac);

#endif
