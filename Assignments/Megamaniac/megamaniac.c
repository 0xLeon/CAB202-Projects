#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <curses.h>
#include "cab202_graphics.h"
#include "game_types.h"
#include "game.h"
#include "megamaniac.h"
#include "megamaniac_go_types.h"
#include "megamaniac_go_data.h"
#include "megamaniac_game_objects.h"
#include "megamaniac_level1.h"
#include "megamaniac_level2.h"
#include "megamaniac_level3.h"

#define MEGAMANIAC_CREDITS	"Stefan Hahn (n9529977)"
#define MEGAMANIAC_START_SCORE	0
#define MEGAMANIAC_START_LIVES	3

#ifdef ENDLESS_LIVES
#undef MEGAMANIAC_START_LIVES
#define MEGAMANIAC_START_LIVES	9999
#endif

void megamaniac_resize(game_p self, game_resize_descriptor_p resize_descriptor);


int main(int argc, char* argv[]) {
	setup_screen();
	clear_screen();

	start_color();
	init_color(COLOR_GREEN, 518, 875, 31);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	bkgd(COLOR_PAIR(1));
	attron(A_BOLD);

	//---------------------------------------------
	start_megamaniac();
	//---------------------------------------------
	
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

	megamaniac->resize = megamaniac_resize;

	megamaniac->game_objects[i++] = megamaniac_create_go_quit_checker(megamaniac);
	megamaniac->game_objects[i++] = megamaniac_create_go_restarter(megamaniac);
	megamaniac->game_objects[i++] = megamaniac_create_go_pauser(megamaniac);
	megamaniac->game_objects[i++] = megamaniac_create_go_level_changer(megamaniac);
	megamaniac->game_objects[i++] = megamaniac_create_go_line(megamaniac);
	megamaniac->game_objects[i++] = megamaniac_create_go_credits(megamaniac, MEGAMANIAC_CREDITS);
	megamaniac->game_objects[i++] = megamaniac_create_go_score(megamaniac, MEGAMANIAC_START_SCORE);
	megamaniac->game_objects[i++] = megamaniac_create_go_lives(megamaniac, MEGAMANIAC_START_LIVES);
	// megamaniac->game_objects[i++] = setup_go_score_updater(megamaniac);

	i = 0;
	megamaniac->levels[i++] = megamaniac_create_level1(megamaniac);
	megamaniac->levels[i++] = megamaniac_create_level2(megamaniac);
	megamaniac->levels[i++] = megamaniac_create_level3(megamaniac);

	megamaniac->current_level = megamaniac->levels[0];
	
	if (NULL != megamaniac->current_level->load) {
		megamaniac->current_level->load(megamaniac->current_level, megamaniac);
	}
	
	return megamaniac;
}

void megamaniac_resize(game_p self, game_resize_descriptor_p resize_descriptor) {
	assert(NULL != self);

	game_object_descriptor_t go_line_descriptor;
	game_object_p go_line = find_game_object_by_type(GO_TYPE_LINE, self->game_objects, self->game_object_count, &go_line_descriptor);
	game_object_p go_credits = find_game_object_by_type(GO_TYPE_CREDITS, self->game_objects, self->game_object_count, NULL);
	game_object_p go_score = find_game_object_by_type(GO_TYPE_SCORE, self->game_objects, self->game_object_count, NULL);
	game_object_p go_lives = find_game_object_by_type(GO_TYPE_LIVES, self->game_objects, self->game_object_count, NULL);


	destroy_game_object(go_line);
	self->game_objects[go_line_descriptor.index] = megamaniac_create_go_line(self);

	go_credits->y = resize_descriptor->new_height - 2;

	go_score->x = resize_descriptor->new_width - go_score->width - 1;
	go_score->y = resize_descriptor->new_height - 2;

	go_lives->x = resize_descriptor->new_width - go_lives->width - 1;
	go_lives->y = resize_descriptor->new_height - 1;
}


void megamaniac_level_default_resize(game_level_p self, game_resize_descriptor_p resize_descriptor, game_p game) {
	assert(NULL != self);
	assert(NULL != resize_descriptor);
	assert(NULL != game);

	game_object_p go_player = find_game_object_by_type(GO_TYPE_PLAYER, self->game_objects, self->game_object_count, NULL);
	game_object_p go_level_name = find_game_object_by_type(GO_TYPE_LEVEL_NAME, self->game_objects, self->game_object_count, NULL);

	go_player->x = (int) round(go_player->x * resize_descriptor->width_ratio);
	go_player->y = resize_descriptor->new_height - 4;

	go_level_name->x = (resize_descriptor->new_width - strlen(go_level_name->bitmap)) / 2;
	go_level_name->y = resize_descriptor->new_height - 1;

	for (int i = 0; i < self->game_object_count; ++i) {
		if ((NULL != self->game_objects[i]) && !(self->game_objects[i]->recycle)) {
			if (megamaniac_go_is_enemy(self->game_objects[i]) || (self->game_objects[i]->type == GO_TYPE_BOMB) || (self->game_objects[i]->type == GO_TYPE_BULLET)) {
				self->game_objects[i]->x *= resize_descriptor->width_ratio;
				self->game_objects[i]->y *= resize_descriptor->height_ratio;
			}
		}
	}
}
