#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "cab202_graphics.h"
#include "game.h"
#include "megamaniac.h"
#include "megamaniac_go_types.h"
#include "megamaniac_go_data.h"
#include "megamaniac_level1.h"

#define MEGAMANIAC_CREDITS	"Stefan Hahn (n9529977)"
#define MEGAMANIAC_START_SCORE	0
#define MEGAMANIAC_START_LIVES	3

int main(int argc, char* argv[]) {
	setup_screen();
	clear_screen();

	//---------------------------------------------
	start_megamaniac();
	//---------------------------------------------
	
	cleanup_screen();
	
	return 0;
}

void start_megamaniac() {
	game_p megamaniac = setup_megamaniac();

	start_game_loop(megamaniac);
	
	destroy_game(megamaniac);
}

game_p setup_megamaniac() {
	game_p megamaniac = create_game(10., 6, 5);
	int i = 0;

	megamaniac->game_objects[i++] = setup_go_quit_checker(megamaniac);
	megamaniac->game_objects[i++] = setup_go_restart(megamaniac);
	megamaniac->game_objects[i++] = setup_go_line(megamaniac);
	megamaniac->game_objects[i++] = setup_go_credits(megamaniac, MEGAMANIAC_CREDITS);
	megamaniac->game_objects[i++] = setup_go_score(megamaniac, MEGAMANIAC_START_SCORE);
	megamaniac->game_objects[i++] = setup_go_lives(megamaniac, MEGAMANIAC_START_LIVES);
	// megamaniac->game_objects[i++] = setup_go_score_updater(megamaniac);

	i = 0;
	megamaniac->levels[i++] = level1_create_level(megamaniac);

	megamaniac->current_level = megamaniac->levels[0];
	
	if (NULL != megamaniac->current_level->load) {
		megamaniac->current_level->load(megamaniac->current_level, megamaniac);
	}
	
	return megamaniac;
}


bool go_quit_checker_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (update->key == 'q') {
		game->running = false;
	}

	return false;
}

game_object_p setup_go_quit_checker(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_object_p go_quit_checker = create_null_game_object(GO_TYPE_QUIT);

	if (NULL == go_quit_checker) {
		return NULL;
	}

	go_quit_checker->update = go_quit_checker_update;

	return go_quit_checker;
}


bool go_restart_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	if (update->key == 'r') {
		if ((NULL == game->current_level) || (NULL ==  game->current_level->load) || (NULL == game->current_level->unload)) {
			return false;
		}
		
		game->drop_frame = true;
		game->current_level->unload(game->current_level, game);
		game->current_level->load(game->current_level, game);

		return true;
	}

	return false;
}

game_object_p setup_go_restart(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_object_p go_restart = create_null_game_object(GO_TYPE_RESTART);

	if (NULL == go_restart) {
		return NULL;
	}

	go_restart->update = go_restart_update;

	return go_restart;
}


game_object_p setup_go_line(game_p megamaniac) {
	assert(NULL != megamaniac);

	char* buf = malloc(megamaniac->screen_width * sizeof(char));

	if (NULL == buf) {
		return NULL;
	}

	memset(buf, '-', megamaniac->screen_width);

	return create_game_object(GO_TYPE_LINE, 0., megamaniac->screen_height - 3, megamaniac->screen_width, 1, 0., 0., buf, 0);
}

game_object_p setup_go_credits(game_p megamaniac, char* credits) {
	assert(NULL != megamaniac);

	return create_static_string_game_object(GO_TYPE_CREDITS, 1., megamaniac->screen_height - 2, 0., 0., 0L, credits);
}


bool go_score_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	go_additional_data_comparable_int_p go_score_data = (go_additional_data_comparable_int_p) self->additional_data;

	if (go_score_data->current_value != go_score_data->previous_value) {
		sprintf(self->bitmap, "Score:% 5d", ((go_score_data->current_value > 9999) ? 9999 : go_score_data->current_value));
		go_score_data->previous_value = go_score_data->current_value;

		return true;
	}

	return false;
}

game_object_p setup_go_score(game_p megamaniac, int initial_score) {
	assert(NULL != megamaniac);

	int buffer_size = 12;
	game_object_p go_score = create_dynamic_string_game_object(GO_TYPE_SCORE, megamaniac->screen_width - buffer_size, megamaniac->screen_height - 2, 0., 0., 0L, buffer_size, "Score:% 5d", ((initial_score > 9999) ? 9999 : initial_score));
	go_additional_data_comparable_int_p go_score_data = malloc(sizeof(go_additional_data_comparable_int_t));

	if (NULL == go_score) {
		return NULL;
	}

	if (NULL == go_score_data) {
		free(go_score);

		return NULL;
	}

	go_score_data->previous_value = initial_score;
	go_score_data->current_value = initial_score;
	go_score->additional_data = (void*) go_score_data;
	go_score->update = go_score_update;

	return go_score;
}


bool go_lives_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	go_additional_data_comparable_int_p go_lives_data = (go_additional_data_comparable_int_p) self->additional_data;;

	bool didUpdate = false;

	if (go_lives_data->current_value != go_lives_data->previous_value) {
		sprintf(self->bitmap, "Lives:% 5d", ((go_lives_data->current_value > 9999) ? 9999 : go_lives_data->current_value));
		go_lives_data->previous_value = go_lives_data->current_value;

		didUpdate = true;
	}

	if (go_lives_data->current_value < 1) {
		// TODO: disable input
		// TODO: show message

		didUpdate = true;
	}

	return didUpdate;
}

game_object_p setup_go_lives(game_p megamaniac, int initial_lives) {
	assert(NULL != megamaniac);

	int buffer_size = 12;
	game_object_p go_lives = create_dynamic_string_game_object(GO_TYPE_LIVES, megamaniac->screen_width - buffer_size, megamaniac->screen_height - 1, 0., 0., 0L, buffer_size, "Lives:% 5d", ((initial_lives > 9999) ? 9999 : initial_lives));
	go_additional_data_comparable_int_p go_lives_data = malloc(sizeof(go_additional_data_comparable_int_t));

	if (NULL == go_lives) {
		return NULL;
	}

	if (NULL == go_lives_data) {
		free(go_lives);

		return NULL;
	}

	go_lives_data->current_value = initial_lives;
	go_lives_data->previous_value = initial_lives;
	go_lives->additional_data = (void*) go_lives_data;
	go_lives->update = go_lives_update;

	return go_lives;
}

bool go_score_updater_update(game_object_p self, game_update_p update, game_p game, game_level_p level) {
	assert(NULL != self);
	assert(NULL != update);
	assert(NULL != game);

	game_object_p go_score = find_game_object_by_type(GO_TYPE_SCORE, game->game_objects, game->game_object_count, NULL);

	if ((NULL != go_score) && (update->key == 'f')) {
		((go_additional_data_comparable_int_p) go_score->additional_data)->current_value += 50;
	}

	return false;
}

game_object_p setup_go_score_updater(game_p megamaniac) {
	assert(NULL != megamaniac);

	game_object_p go_score_updater = create_null_game_object(GO_TYPE_SCORE_CHEATER);

	if (NULL == go_score_updater) {
		return NULL;
	}

	go_score_updater->update = go_score_updater_update;

	return go_score_updater;
}
