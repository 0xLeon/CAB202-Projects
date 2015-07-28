#ifndef __SIMPLE_SPRITE_H__
#define __SIMPLE_SPRITE_H__

/* ------------------------------------------------------------
 *	File: cab202_sprites.h
 *	Author: Lawrence Buckingham
 *	Copyright: Queensland University of Technology
 *	Date: 16-Mar-2015, 19-Jul-2015.
 *
 *	A simple curses sprite library.
 * ------------------------------------------------------------
 */

/*
 *	Data structure used to manage and render moving sprites.
 *
 *	Members:
 *		id: The unique ID of the sprite.
 *
 *		x, y:	The location of the sprite, represented as double to allow
 *				fractional positioning.
 *
 *		width:	The width of the sprite. This must be less than or equal to 
 *				SS_WIDTH.
 *
 *		height:	The height of the sprite. This must be less than or equal to 
 *				SS_HEIGHT.
 *
 *		dx, dy: A pair of floating point values that will update the sprite location 
 *				each time the sprite moves forward.
 *
 *		is_visible: Current visibility of the sprite. TRUE == visible; FALSE == invisible.
 *
 *		bitmap: an array of curses-attributed characters that represents the image. 0 is 
 *				treated as transparent.
 */

typedef struct sprite {
	int width;
	int height;
	double x, y, dx, dy;
	unsigned char is_visible;
	char * bitmap;
} sprite_t;

/* 
 *	Data type to uniquely identify all registered sprites. 
 */

typedef sprite_t * sprite_id;

/*
 *	create_simple_sprite:
 *
 *	Initialise a sprite, appending it into the z buffer and saving its initial background.
 *	The sprite is then displayed at the coordinates specified by (x,y).
 *
 *	Input:
 *		x, y:	The initial location of the sprite. Floating point values are
 *				used to allow sub-pixel resolution (in a chunky display),
 *				making it easier to achieve "smooth" movement.
 *
 *		width, height: The dimensions of the sprite.
 *
 *		bitmap:	The characters to show.
 *
 *	Output:
 *		Returns the ID of an initialised simple sprite object.
 */

sprite_id create_sprite( double x, double y, int width, int height, char * bitmap );

/**
 *	Releases the memory resources being used by a sprite.
 */

void destroy_sprite( sprite_id sprite );

/*
 *	draws the sprite image.
 *
 *	Turn an invisible sprite visible.
 *
 *	Input:
 *		id: The ID of the sprite which is to be made visible.
 *
 *	Output:
 *		n/a
 */

void draw_sprite( sprite_id id );

/*
 *	turn_sprite:
 *
 *	Sets the internally stored direction. This is the steps that is taken when 
 *	the sprite moves forward or backward.
 *
 *	The new direction is relative to the old one. If the old direction is 0,0 then
 *	the new one will also be 0,0.
 *
 *	Input:
 *		sprite_id: The ID of a sprite.
 *		degrees: The angle to turn.
 */

void turn_sprite( sprite_id sprite, double degrees );

#endif
