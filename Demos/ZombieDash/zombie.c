#include <stdlib.h>
#include <math.h>

#include "zombie.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"
#include "cab202_graphics.h"

#define ZOMBIE_COUNT (25)
#define ZOMBIE_UPDATE_TIME (100)
#define SLEEP_TIME (10)

sprite_id zombies[ZOMBIE_COUNT];
timer_id zombie_timer;

void reset_zombies() {
	int width = screen_width();
	int height = screen_height();

	for ( int i = 0; i < ZOMBIE_COUNT; i++ ) {
		sprite_id zombie = zombies[i];

		zombie->x = rand() % width;
		zombie->y = rand() % height;

		double direction = ( rand() % 360 ) * M_PI / 180;
		zombie->dx = cos( direction );
		zombie->dy = sin( direction );

		zombie->is_visible = TRUE;


		// fprintf( stderr, "( zombie->x, zombie->y ) = ( %g, %g )\n", zombie->x, zombie->y );
		// fprintf( stderr, "( zombie->dx, zombie->dy ) = ( %g, %g )\n", zombie->x, zombie->y );
		draw_sprite( zombie );
	}
}


void create_zombies() {
	for ( int i = 0; i < ZOMBIE_COUNT; i++ ) {
		zombies[i] = create_sprite( 0, 0, 1, 1, "Z" );
	}

	reset_zombies();
}


void setup_zombies() {
	create_zombies();
	zombie_timer = create_timer( ZOMBIE_UPDATE_TIME );
}

bool update_zombies() {

	// fprintf( stderr, "update_zombies\n" );

	if ( ! timer_expired( zombie_timer ) ) {
		return FALSE;
	}

	// fprintf( stderr, "timer expired\n" );

	int width = screen_width();
	int height = screen_height();

	// fprintf( stderr, "( width, height ) = ( %d, %d )\n", width, height );

	for ( int i = 0; i < ZOMBIE_COUNT; i++ ) {
		sprite_id zombie = zombies[i];

		int new_x = (int) round( zombie->x + zombie->dx );
		int new_y = (int) round( zombie->y + zombie->dy );

		// fprintf( stderr, "( new_x, new_y ) = ( %d, %d )\n", new_x, new_y );

		if ( new_y < 0 || new_y >= height ) {
			zombie->dy = -zombie->dy;
		}

		if ( new_x < 0 || new_x >= width ) {
			zombie->dx = -zombie->dx;
		}

		zombie->x += zombie->dx;
		zombie->y += zombie->dy;
	}

	return TRUE;
}

sprite_id get_zombie_at( int x, int y ) {
	for ( int i = 0; i < ZOMBIE_COUNT; i++ ) {
		sprite_id zombie = zombies[i];
		int zx = (int) round( zombie->x );
		int zy = (int) round( zombie->y );

		if ( zx == x && zy == y ) {
			return zombie;
		}
	}

	return NULL;
}


/**
*	Draw the zombies.
*/

void draw_zombies( void ) {
	for ( int i = 0; i < ZOMBIE_COUNT; i++ ) {
		draw_sprite( zombies[i] );
	}
}

/**
*	Clean up the zombies.
*/

void cleanup_zombies( void ) {
	for ( int i = 0; i < ZOMBIE_COUNT; i++ ) {
		free( zombies[i] );
	}
}
