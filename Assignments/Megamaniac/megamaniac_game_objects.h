#ifndef MEGAMANIAC_GAME_OBJECTS_H_
#define MEGAMANIAC_GAME_OBJECTS_H_

#include <stdbool.h>
#include "game_types.h"
#include "game_object.h"


//-------------------------------------------------------
// Game Object Creation Functions Forward Declarations
//-------------------------------------------------------

game_object_p megamaniac_create_go_quit_checker(game_p megamaniac);

game_object_p megamaniac_create_go_restarter(game_p megamaniac);

game_object_p megamaniac_create_go_pauser(game_p megamaniac);

game_object_p megamaniac_create_go_level_changer(game_p megamaniac);

game_object_p megamaniac_create_go_line(game_p megamaniac);

game_object_p megamaniac_create_go_credits(game_p megamaniac, char* credits);

game_object_p megamaniac_create_go_score(game_p megamaniac, int initial_score);

game_object_p megamaniac_create_go_lives(game_p megamaniac, int initial_lives);

game_object_p megamaniac_create_go_score_cheater(game_p megamaniac);

game_object_p megamaniac_create_go_level_name(game_p megamaniac, char* level_name);

game_object_p megamaniac_create_go_lost_screen(game_p megamaniac);

game_object_p megamaniac_create_go_pause_screen(game_p megamaniac);

game_object_p megamaniac_create_go_player(game_p megamaniac);

game_object_p megamaniac_create_go_bullet(game_p megamaniac, double x);

game_object_p megamaniac_create_go_enemy(game_p megamaniac, double x, double y, int enemy_type, game_object_update_f enemy_update_f);

game_object_p megamaniac_create_go_enemy_mover(game_p megamaniac, long interval, game_object_update_f enemy_mover_update_f);

game_object_p megamaniac_create_go_bomb(game_p megamaniac, double x, double y);

game_object_p megamaniac_create_go_bomb_dropper(game_p megamaniac);

int megamaniac_create_standard_enemy_formation(game_level_p level, game_p megamaniac, int offset, int enemy_type, int row_count, int odd_count, int even_count, double horizontal_spacing, double vertical_spacing, game_object_update_f enemy_update_f);


//-------------------------------------------------------
// Helper Functions Forward Declarations
//-------------------------------------------------------

bool megamaniac_go_is_enemy(game_object_p game_object);

bool megamaniac_type_is_enemy(int game_object_type);

bool megamaniac_binary_find_save_player_location(game_p megamaniac, game_object_p go_player, game_object_p* game_objects, int game_object_count, int test_sector_x0, int test_sector_x1);

bool megamaniac_is_safe_player_location(int current_test_x, int current_test_y, game_object_p go_player, game_object_p* game_objects, int game_object_count);

#endif
