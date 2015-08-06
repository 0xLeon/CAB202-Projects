#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "cab202_sprites.h"
#include <curses.h>

/*	
 *	setup_zombies:
 *
 *	Sets up the zombies in the game, creating sprites for each,
 *	placing them on the screen at their initial (random) position, and
 *	choosing their initial direction of travel.
 */

void setup_zombies( void );

/*	
 *	reset_zombies:
 *
 *	Reinitialises the zombies, placing them on the screen at new random 
 *	positions, and choosing their initial direction of travel.
 */

void reset_zombies( void );

/*
 *	Clean up the zombie module.
 */

void cleanup_zombies( void );

/*
 *	update_zombies:
 *
 *	Updates the location of all the monsters in the game, including
 *	collision detection with the walls. 
 *
 *	Output:
 *		Returns TRUE if and only if a zombie-related event has changed the 
 *		appearance of the screen, indicating that a refresh may be required.
 */

bool update_zombies( void );

/*
 *	get_zombie_at:
 *
 *	Gets the address of (one of) the zombies at a particular location.
 *
 *	Input:
 *		x, y:	The integer screen coordinates of the location.
 *
 *	Output:
 *		Returns the ID of the first zombie sprite (if any) at the specified
 *		location, or SS_ERROR if there is no zombie at the location.
 */

sprite_id get_zombie_at( int x, int y );

/**
 *	Draw the zombies.
 */

void draw_zombies( void );

#endif
