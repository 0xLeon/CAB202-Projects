#include <stdlib.h>
#include <unistd.h>
#include "cab202_graphics.h"

int main( void ) {
	setup_screen();

	draw_string( 0, 0, "Hello CAB202." );
	
	show_screen();

	sleep( 10 );
	cleanup_screen();

	return 0;
}