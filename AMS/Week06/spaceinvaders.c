#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"
#include "game.h"

#define SHIP_INIT_X	6
#define SHIP_INIT_Y	10

#define BULLET_INIT_X	0.
#define BULLET_INIT_Y	0.
#define BULLET_TIMER	30L

#define ALIEN_TIMER	350L

// ---------------------------------------------------------------- //

void setup_game(Game* game) {
	game->over = false;
	game->score = 0;
	game->event_loop_delay = 10;
}

void draw_game(Game* game) {
	int width;
	int height;
	
	get_screen_size(width, height);
	
	draw_line(0, height - 2, width - 1, height - 2, '-');
	draw_formatted(width - 20, height - 1, "Score: %d", game->score);
}
// ---------------------------------------------------------------- //

void setup_ship(Game* game) {
	static char bitmap[] =
		"======  _  "
		"   \\   / \\ "
		"  ====| o |"
		"   /   \\_/ "
		"======     ";
	
	game->ship = create_sprite(0., 0., 11, 5, bitmap);
	// game->ship->x = SHIP_INIT_X - (game->ship->width / 2);
	// game->ship->y = SHIP_INIT_Y - (game->ship->height / 2);
	game->ship->x = 3.;
	game->ship->y = 4.;
}

bool update_ship(Game* game, int key_code) {
	int old_x;
	int old_y;
	int new_x;
	int new_y;
	
	old_x = (int) round(game->ship->x);
	old_y = (int) round(game->ship->y);
	
	switch (key_code) {
		case '1':
			if (game->ship->x > 0.) {
				game->ship->x -= 1.;
			}
			break;
		case '2':
			if ((game->ship->y + game->ship->height) < (screen_height() - 2)) {
				game->ship->y += 1.;
			}
			break;
		case '3':
			if ((game->ship->x + game->ship->width) < 30.) {
				game->ship->x += 1.;
			}
			break;
		case '5':
			if (game->ship->y > 0.) {
				game->ship->y -= 1.;
			}
			break;
		default:
			return false;
	}
	
	new_x = (int) round(game->ship->x);
	new_y = (int) round(game->ship->y);
	
	return (new_x != old_x) || (new_y != old_y);
}

void draw_ship(Game* game) {
	draw_sprite(game->ship);
}

void cleanup_ship(Game* game) {
	destroy_sprite(game->ship);
	game->ship = NULL;
}

// ---------------------------------------------------------------- //

void setup_bullet(Game* game) {
	static char bitmap[] = {'-'};
	
	game->bullet = create_sprite(BULLET_INIT_X, BULLET_INIT_Y, 1, 1, bitmap);
	game->bullet->is_visible = false;
	game->bullet_timer = create_timer(BULLET_TIMER);
}

void launch_bullet(Game* game) {
	if (game->bullet->is_visible) {
		return;
	}
	
	game->bullet->x = game->ship->x + game->ship->width;
	game->bullet->y = game->ship->y + (game->ship->height / 2);
	game->bullet->dx = 1.;
	game->bullet->dy = 0.;
	game->bullet->is_visible = true;
	
	reset_timer(game->bullet_timer);
}

bool inside_rect(int x, int y, int left, int top, int width, int height) {
	if (x >= left && x < left + width && y >= top && y < top + height) {
		return true;
	}
	else {
		return false;
	}
}

bool move_bullet(Game* game) {
	if (!game->bullet->is_visible || !timer_expired(game->bullet_timer)) {
		return false;
	}
	
	int old_x;
	int old_y;
	int new_x;
	int new_y;
	
	old_x = (int) round(game->bullet->x);
	old_y = (int) round(game->bullet->y);
	game->bullet->x += game->bullet->dx;
	game->bullet->y += game->bullet->dy;
	new_x = (int) round(game->bullet->x);
	new_y = (int) round(game->bullet->y);
	
	if ((new_x >= screen_width()) || (new_y >= screen_height())) {
		game->bullet->is_visible = false;
		
		return true;
	}
	
	for (int i = 0; i < N_ALIENS; i++) {
		if (game->aliens[i]->is_visible) {
			int alien_x = (int) round(game->aliens[i]->x);
			int alien_y = (int) round(game->aliens[i]->y);
			
			if ((new_x == alien_x) && (new_y == alien_y)) {
				game->bullet->is_visible = false;
				game->aliens[i]->is_visible = false;
				game->score++;
				
				return true;
			}
		}
	}
	
	return (old_x != new_x) || (old_y != new_y);
}

bool update_bullet(Game* game, int key_code) {
	if (game->bullet->is_visible) {
		return move_bullet(game);
	}
	else if (key_code == ' ') {
		launch_bullet(game);
		return true;
	}
	else {
		return false;
	}
}

void draw_bullet(Game* game) {
	draw_sprite(game->bullet);
}

void cleanup_bullet(Game * game) {
	destroy_sprite(game->bullet);
	game->bullet = NULL;
}

// ---------------------------------------------------------------- //

void setup_aliens(Game* game, int rock_x[], int rock_y[]) {
	static char bitmap[] = {'@'};
	
	for (int i = 0; i < N_ALIENS; i++) {
		game->aliens[i] = create_sprite(rock_x[i], rock_y[i], 1, 1, bitmap);
		game->aliens[i]->dy = 1.;
	}
	
	game->alien_timer = create_timer(ALIEN_TIMER);
}

void draw_aliens(Game * game) {
	for (int i = 0; i < N_ALIENS; i++) {
		draw_sprite(game->aliens[i]);
	}
}

bool move_alien(sprite_id alien) {
	if (!alien->is_visible) {
		return false;
	}
	
	int old_y = (int) round(alien->y);
	alien->y += alien->dy;
	int new_y = (int) round(alien->y);
	
	if (new_y >= (screen_height() - 2)) {
		alien->y = 0.;
		
		return true;
	}
	
	return (old_y != new_y);
}

bool update_aliens(Game* game) {
	if (!timer_expired(game->alien_timer)) {
		return false;
	}

	bool changed = false;

	for (int i = 0; i < N_ALIENS; i++) {
		sprite_id alien = game->aliens[i];
		changed = move_alien(alien) || changed;
	}

	return changed;
}

void cleanup_aliens(Game* game) {
	for (int i = 0; i < N_ALIENS; i++) {
		destroy_sprite(game->aliens[i]);
		game->aliens[i] = NULL;
	}
}

// --------- Do not submit the following functions --------- // 

/**
 *	Gets a random integer that is greater than or equal to
 *	first and less than last.
 *	Precondition: first < last;
 */
int rand_between(int first, int last) {
	return first + rand() % (last - first);
}

/**
 *	Sets up all components of the game.
 */
void setup_all(Game* game) {
	int alien_x[N_ALIENS];
	int alien_y[N_ALIENS];

	for (int i = 0; i < N_ALIENS; i++) {
		alien_x[i] = rand_between(30, screen_width());
		alien_y[i] = rand_between(2, screen_height() - 4);
	}

	setup_game(game);
	setup_ship(game);
	setup_bullet(game);
	setup_aliens(game, alien_x, alien_y);
}

void draw_all(Game* game) {
	clear_screen();
	draw_game(game);
	draw_ship(game);
	draw_bullet(game);
	draw_aliens(game);
	show_screen();
}

void cleanup_all(Game* game) {
	cleanup_ship(game);
	cleanup_bullet(game);
	cleanup_aliens(game);
}

void event_loop() {
	Game game;

	setup_all(&game);
	draw_all(&game);

	while (!game.over) {
		int key_code = get_char();

		if (key_code == 'q') {
			game.over = true;
		}
		else {
			bool show_ship = update_ship(&game, key_code);
			bool show_bullet = update_bullet(&game, key_code);
			bool show_aliens = update_aliens(&game);

			if (show_ship || show_bullet || show_aliens) {
				draw_all(&game);
			}

			if (game.score == N_ALIENS) {
				game.over = true;
			}
		}

		timer_pause(game.event_loop_delay);
	}

	cleanup_all(&game);
}

int main() {
	srand(time(NULL));
	setup_screen();
	event_loop();
	cleanup_screen();
	
	return 0;
}
