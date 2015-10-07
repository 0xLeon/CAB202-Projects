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
			break;
		}
	}
}

uint8_t sprites_collided(psprite_p face, psprite_p player) {
	uint8_t face_x0 = (uint8_t) face->x;
	uint8_t face_y0 = (uint8_t) face->y;
	uint8_t face_x1 = face_x0 + face->width - 1U;
	uint8_t face_y1 = face_y0 + face->height - 1U;

	uint8_t player_x0 = (uint8_t) player->x;
	uint8_t player_y0 = (uint8_t) player->y;
	uint8_t player_x1 = player_x0 + player->width - 1U;
	uint8_t player_y1 = player_y0 + player->height - 1U;

	uint8_t inx =
		isect(player_x0, face_x0, face_x1) ||
		isect(player_x1, face_x0, face_x1) ||
		inside(player_x0, player_x1, face_x0, face_x1) ||
		inside(face_x0, face_x1, player_x0, player_x1);

	uint8_t iny =
		isect(player_y0, face_y0, face_y1) ||
		isect(player_y1, face_y0, face_y1) ||
		inside(player_y0, player_y1, face_y0, face_y1) ||
		inside(face_y0, face_y1, player_y0, player_y1);

	return (inx && iny);
}
