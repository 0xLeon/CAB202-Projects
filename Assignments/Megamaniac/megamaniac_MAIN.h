#ifndef MEGAMANIC_H_
#define MEGAMANIC_H_

#include "game.h"

#define POINTS_PER_ENEMY	30
#define POINTS_BONUS		500

void start_megamaniac();

game_p setup_megamaniac();


void megamaniac_level_default_resize(game_level_p self, game_resize_descriptor_p resize_descriptor, game_p game);

#endif
