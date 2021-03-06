#include <stdint.h>
#include "pgraphics.h"
#include "game.h"

void default_collision_face_happy(game_p game) {
	game->score += 2U;

	if (game->score >= 20U) {
		game->ended = 1U;
	}
}

void default_collision_face_angry(game_p game) {
	--(game->lives);

	if (0U == game->lives) {
		game->ended = 1U;
	}
}

void default_collision_face_mad(game_p game) {
	for (uint8_t i = 0; i < game->face_count; ++i) {
		game->faces[i]->dy += .4f;

		if (game->faces[i]->dy >= 1.2f) {
			game->faces[i]->dy = 1.2f;
		}
	}
}

void check_face_player_collision(game_p game) {
	for (uint8_t i = 0; i < game->face_count; ++i) {
		if (game->faces[i]->is_visible && sprites_collided(game->faces[i], game->player)) {
			game->faces[i]->is_visible = 0U;
			game->face_collision_handlers[i](game);
			--(game->active_face_count);
			break;
		}
	}
}

uint8_t sprites_collided(psprite_p sprite_a, psprite_p sprite_b) {
	uint8_t sprite_a_x0 = (uint8_t) sprite_a->x;
	uint8_t sprite_a_y0 = (uint8_t) sprite_a->y;
	uint8_t sprite_a_x1 = sprite_a_x0 + sprite_a->width - 1U;
	uint8_t sprite_a_y1 = sprite_a_y0 + sprite_a->height - 1U;

	uint8_t sprite_b_x0 = (uint8_t) sprite_b->x;
	uint8_t sprite_b_y0 = (uint8_t) sprite_b->y;
	uint8_t sprite_b_x1 = sprite_b_x0 + sprite_b->width - 1U;
	uint8_t sprite_b_y1 = sprite_b_y0 + sprite_b->height - 1U;

	uint8_t inx =
		isect(sprite_b_x0, sprite_a_x0, sprite_a_x1) ||
		isect(sprite_b_x1, sprite_a_x0, sprite_a_x1) ||
		inside(sprite_b_x0, sprite_b_x1, sprite_a_x0, sprite_a_x1) ||
		inside(sprite_a_x0, sprite_a_x1, sprite_b_x0, sprite_b_x1);

	uint8_t iny =
		isect(sprite_b_y0, sprite_a_y0, sprite_a_y1) ||
		isect(sprite_b_y1, sprite_a_y0, sprite_a_y1) ||
		inside(sprite_b_y0, sprite_b_y1, sprite_a_y0, sprite_a_y1) ||
		inside(sprite_a_y0, sprite_a_y1, sprite_b_y0, sprite_b_y1);

	return (inx && iny);
}
