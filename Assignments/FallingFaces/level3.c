#include <stdlib.h>
#include "usb_serial.h"
#include "lcd.h"
#include "pgraphics.h"
#include "level3.h"

static void level3_load(level_p self, game_p game);
static uint8_t level3_update(level_p self, game_p game);
static void level3_draw(level_p self, game_p game);
static void level3_unload(level_p self, game_p game);

static void spawn_player(game_p game);
static void spawn_faces(game_p game);

level_p create_level3(game_p game) {
	level_p level3 = calloc(1U, sizeof(level_t));

	if (NULL == level3) {
		// TODO: display error
		return NULL;
	}

	level3->load = level3_load;
	level3->draw = level3_draw;
	level3->update = level3_update;
	level3->unload = level3_unload;

	return level3;
}

static void level3_load(level_p self, game_p game) {
	spawn_player(game);
	spawn_faces(game);

	p_clear_screen();

	p_draw_string(14U, 15U, "Waiting for");
	p_draw_string(0U, 25U, "USB connection...");

	p_show_screen();

	usb_init();

	while (!usb_configured() || !usb_serial_get_control());

	usb_serial_flush_input();
	usb_serial_flush_output();
}

static uint8_t level3_update(level_p self, game_p game) {
	return 0U;
}

static void level3_draw(level_p self, game_p game) {
	game->levels[1U]->draw(self, game);
}

static void level3_unload(level_p self, game_p game) {
	return;
}

static void spawn_player(game_p game) {
	game->player->x = rand() % (LCD_X - game->player->width);
	game->player->y = 9U + (rand() % (LCD_Y - 9U - game->player->height));
}

static void spawn_faces(game_p game) {
	return;
}
