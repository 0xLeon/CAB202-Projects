#include <stdlib.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"

int main( ) {
	setup_screen( );

	sprite_id hero = create_sprite( 
		???, 
		???, 
		1, 2, "LB" );

	sprite_id zombie = create_sprite( 
		???, 
		???, 
		1, 1, "Z" );

	cleanup_screen( );
	return 0;
}