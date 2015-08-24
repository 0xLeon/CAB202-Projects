#ifndef MEGAMANIAC_LEVEL1_H_
#define MEGAMANIAC_LEVEL1_H_

#include "game_level.h"

game_level_p level1_create_level(game_p megamaniac);

game_object_p level1_create_level_name(game_p megamaniac);

game_object_p level1_create_player(game_p megamaniac);

game_object_p level1_create_enemy1(int x, int y, game_p megamaniac);

game_object_p level1_create_enemy1_mover(game_p megamaniac);

game_object_p level1_create_bullet(game_p megamaniac, int x);

game_object_p level1_create_bomb_dropper(game_p game);

game_object_p level1_create_bomb(game_p game, int x, int y);

game_object_p level1_create_lost_screen(game_p game);

#endif
