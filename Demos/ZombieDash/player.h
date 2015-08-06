#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <curses.h>

/*
 *	setup_player:
 *
 *	Initialises the player-related aspects of the game,
 *	creating the icon for the player and initialising other state related 
 *	to the player.
 */

void setup_player( void );

/*
 *	reset_player:
 *
 *	Restores the player to its original position and appearance.
 */

void reset_player( void );

/*
 *	Clean up the player module.
 */

void cleanup_player( void );

/*
 *	Interprets the key code and updates the position of the player sprite 
 *	accordingly. 
 *
 *	Input:
 *		key_code:	an integer that represents a key code.
 *
 *	Output:
 *		Returns TRUE if and only if the player moved, indicating that the screen
 *		may need to be refreshed.
 */

int update_player( int key_code );

/*  
 *	check_player_died.
 *	
 *	Tests to see if any of the monsters are in the same screen 
 *	location as the player. If yes, a message is displayed and subsequent calls 
 *	to player_dead() will return TRUE until the game is restarted.
 */

void check_player_died();

/* 
 *	player_dead:
 *
 *	Returns TRUE if and only if the player is dead.
 */

int player_dead();

/**
*	Draw the player.
*/

void draw_player( void );

#endif
