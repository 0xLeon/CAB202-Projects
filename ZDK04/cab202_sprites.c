#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"
#include "curses.h"


sprite_id create_sprite( double x, double y, int width, int height, char * image ) {
	assert( width > 0 );
	assert( height > 0 );
	assert( image != NULL );

	sprite_id sprite = malloc( sizeof( sprite_t ) );

	if ( sprite != NULL ) {
		sprite->is_visible = TRUE;
		sprite->x = x;
		sprite->y = y;
		sprite->width = width;
		sprite->height = height;
		sprite->dx = 0;
		sprite->dy = 0;
		sprite->bitmap = image;
	}

	return sprite;
}


void destroy_sprite( sprite_id sprite ) {
	if ( sprite != NULL ) {
		free( sprite );
	}
}


void draw_sprite( sprite_id sprite ) {
	assert( sprite != NULL );

	if ( !sprite->is_visible ) return;

	int x = (int)round( sprite->x );
	int y = (int)round( sprite->y );
	int offset = 0;

	for ( int row = 0; row < sprite->height; row++ ) {
		for ( int col = 0; col < sprite->width; col++ ) {
			char ch = sprite->bitmap[offset++] & 0xff;

			if ( ch != ' ' ) {
				draw_char( x + col, y + row, ch );
			}
		}
	}
}


/*
 *	turn_sprite:
 *
 *	Sets the internally stored direction. That is, the step that is taken when
 *	the sprite moves forward or backward.
 *
 *	The new direction is relative to the old one. If the old direction is 0,0 then
 *	the new one will also be 0,0.
 *
 *	Input:
 *		sprite: The ID of a sprite.
 *		degrees: The angle to turn.
 */

void turn_sprite( sprite_id sprite, double degrees ) {
	double radians = degrees * M_PI / 180;
	double s = sin( radians );
	double c = cos( radians );
	double dx = s * sprite->dx + c * sprite->dy;
	double dy = -c * sprite->dx + s * sprite->dy;
	sprite->dx = dx;
	sprite->dy = dy;
}
