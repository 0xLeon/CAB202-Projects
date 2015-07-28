
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "player.h"
#include "cab202_sprites.h"
#include "cab202_graphics.h"
#include "zombie.h"

sprite_id player, dead_player_banner;
bool player_is_dead;
char * normal_image = "@";
char * dead_image = "_";
char * banner =
"+---------------------------------------------------------+"
"|       You are zombie food! Press 'r' to try again.      |"
"+---------------------------------------------------------+";


void setup_player() {
	player = create_sprite( 0, 0, 1, 1, normal_image );

	int banner_width = strlen( banner ) / 3;
	int banner_height = 3;

	dead_player_banner = create_sprite( 0, 0, banner_width, banner_height, banner 
	);

	reset_player();
}

void reset_player() {
	int width = screen_width();
	int height = screen_height();

	// Position the player's avatar in the centre of the screen.
	player->x = width / 2;
	player->y = height / 2;

	// Make the player look alive.
	player_is_dead = FALSE;
	player->is_visible = TRUE;
	player->bitmap = normal_image;

	// Restore the banner.

	int banner_width = strlen( banner ) / 3;
	int banner_height = 3;
	dead_player_banner->x = ( width - banner_width ) / 2;
	dead_player_banner->y = ( height - banner_height ) / 2;
	dead_player_banner->is_visible = FALSE;
}

void check_player_died() {
	sprite_id zombie = get_zombie_at( (int) round(player->x), (int) round(player->y) );

	if ( zombie == NULL ) {
		return;
	}

	player_is_dead = TRUE;
	player->bitmap = dead_image;
	dead_player_banner->is_visible = TRUE;
}

int player_dead() {
	return player_is_dead;
}


/**
*	Draw the player.
*/

void draw_player( void ) {
	draw_sprite( player );
	draw_sprite( dead_player_banner );
}


/**
*	Clean up the player.
*/

void cleanup_player( void ) {
	free( player );
}

// I put this last because I do not want the namespace to be contaminated 
// with curses garbage. curses is included puerly to gain access to the key 
// codes for the arrow keys.
#include "curses.h"

int update_player( int key_code ) {

	// fprintf( stderr, "update_player\n" );

	int player_moved = TRUE;

	int width, height;
	get_screen_size( width, height );

	if ( key_code == KEY_UP && player->y > 0 ) {
		player->y--;
	}
	else if ( key_code == KEY_DOWN && player->y < height - 1 ) {
		player->y++;
	}
	else if ( key_code == KEY_LEFT &&  player->x > 0 ) {
		player->x--;
	}
	else if ( key_code == KEY_RIGHT && player->x < width - 1 ) {
		player->x++;
	}
	else {
		player_moved = false;
	}

	return player_moved;
}
