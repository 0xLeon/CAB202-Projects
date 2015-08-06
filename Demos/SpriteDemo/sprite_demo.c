/*
*	File:	sprite_demo.c
*	Author: Lawrence Buckingham, Queensland University of Technology.
*	Date:	27 April 2015.
*
*	Entry point for the sprite demo program.
*/

#include <cab202_graphics.h>
#include "cab202_timers.h"
#include "one_square.h"
#include "ten_squares.h"
#include "blockhead.h"

int main( void ) {
	setup_screen();

	draw_string( 10, 10, "Hello CAB202!!!" );
	show_screen();

	while ( 1 ) {
		play_one_square();
		play_ten_squares();
		walk_the_walk();
	}

	return 0;
}
