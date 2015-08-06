/*
*	File:	one_square.c
*
*	Implementation of the functions provided by the one_square "game".
*
*	Author: Lawrence Buckingham, Queensland University of Technology.
*	Date:	27 April 2015.
*
*	Revision history:
*		19-Jul-2015	-	Adapted to run in linux/cygwin instead of Teensy.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "one_square.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"
#include "cab202_graphics.h"

void update_one_square( sprite_id sprite );

void play_one_square() {
	clear_screen();
	draw_string(0, 0,  "Press any key to play..." );
	show_screen();
	
	wait_char();

	char bitmap [] = 
		"11111111"
		"1      1"
		"1      1"
		"1      1"
		"1      1"
		"1      1"
		"1      1"
		"11111111"
	;

	sprite_id sprite = create_sprite( 0, 0, 8, 8, bitmap );

	double angle = ((double)random()) * 2 * M_PI / RAND_MAX;
	sprite->dx = cos( angle );
	sprite->dy = sin( angle );

	int key = 0;

	while ( key != 'q' ) {
		clear_screen();
		update_one_square( sprite );
		draw_sprite( sprite );
		draw_string( 0, screen_height() - 1, "Press 'q' to exit..." );
		show_screen();

		timer_pause( 50 );
		key = get_char();
	}
}

void update_one_square( sprite_id sprite ) {
	sprite->x += sprite->dx;
	sprite->y += sprite->dy;

	if ( sprite->x >= screen_width() - sprite->width || sprite->x < 0 ) {
		sprite->dx = -sprite->dx;
	}

	if ( sprite->y >= screen_height() - sprite->height || sprite->y < 0 ) {
		sprite->dy = -sprite->dy;
	}
}
