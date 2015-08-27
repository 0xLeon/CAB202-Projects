#ifndef MEGAMANIAC_LEVELS_H_
#define MEGAMANIAC_LEVELS_H_

#include <stdbool.h>
#include "game_types.h"
#include "game_object.h"

game_object_p megamaniac_create_level_name(game_p megamaniac, char* level_name);

game_object_p megamaniac_create_lost_screen(game_p megamaniac);

game_object_p megamaniac_create_player(game_p megamaniac);

game_object_p megamaniac_create_bullet(game_p megamaniac, double x);

game_object_p megamaniac_create_enemy(game_p megamaniac, double x, double y, int enemy_type, game_object_update_f enemy_update_f);

game_object_p megamaniac_create_enemy_mover(game_p megamaniac, long interval, game_object_update_f enemy_mover_update_f);

game_object_p megamaniac_create_bomb(game_p megamaniac, double x, double y);

game_object_p megamaniac_create_bomb_dropper(game_p megamaniac);

int megamaniac_create_standard_enemy_formation(game_level_p level, game_p megamaniac, int offset, int enemy_type, int row_count, int odd_count, int even_count, double horizontal_spacing, double vertical_spacing, game_object_update_f enemy_update_f);


bool megamaniac_game_object_is_enemy(game_object_p game_object);

bool megamaniac_type_is_enemy(int game_object_type);

#endif
