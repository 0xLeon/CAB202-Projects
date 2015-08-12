#include <stdlib.h>
#include <time.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"

void demo( void );

int main( void ) {
	setup_screen();
	srand( time( NULL ) );
	demo();

	timer_pause( 5000 );
	cleanup_screen();
	return 0;
}

typedef char *STRING;

STRING bitmaps[] = {
	" | "
	" O "
	" | ",

	"  /"
	" O "
	"/  ",

	"   "
	"-O-"
	"   ",

	"\\  "
	" O "
	"  \\",

};

#define N_BITMAPS (4)

int random_x( void );
int random_y( void );
void bounce( sprite_id sprite );

void demo( void ) {
	timer_id prop_timer = create_timer( 125 );

	// Count which bitmap I'm using at the moment.
	int bitmap[10];
	sprite_id prop[10];

	for ( int i = 0; i < 10; i++ ) {
		bitmap[i] = rand() % N_BITMAPS;
		prop[i] = create_sprite( random_x(), random_y(), 3, 3, bitmaps[bitmap[i]] );
		prop[i]->dx = 1;
		turn_sprite( prop[i], rand() % 360 );
		draw_sprite( prop[i] );
	}

	show_screen();

	while ( 1 ) {
		if ( timer_expired( prop_timer ) ) {
			clear_screen();

			for ( int i = 0; i < 10; i++ ) {
				bitmap[i] = ( bitmap[i] + 1 ) % N_BITMAPS;
				prop[i]->bitmap = bitmaps[bitmap[i]];
				prop[i]->x += prop[i]->dx;
				prop[i]->y += prop[i]->dy;
				bounce( prop[i] );
				draw_sprite( prop[i] );
			}

			show_screen();
		}
		timer_pause( 20 );
	}
}

int random_x( void ) { return rand() % ( screen_width() - 3 ); }
int random_y( void ) { return rand() % ( screen_height() - 3 ); }

void bounce( sprite_id sprite ) {
	if ( sprite->x <= 0 ) { sprite->dx = -sprite->dx; }
	if ( sprite->y <= 0 ) { sprite->dy = -sprite->dy; }
	if ( sprite->x + sprite->width >= screen_width() ) { sprite->dx = -sprite->dx; }
	if ( sprite->y + sprite->height >= screen_height( ) ) { sprite->dy = -sprite->dy; }
}
