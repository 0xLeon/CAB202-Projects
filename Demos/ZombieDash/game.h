#ifndef __GAME_H__
#define __GAME_H__

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "cab202_sprites.h"
#include "cab202_timers.h"
#include "cab202_graphics.h"
#include "player.h"
#include "zombie.h"

#define TURN_SCREEN_CHANGED 1
#define TURN_OK 0
#define TURN_GAME_OVER -1

/*
 *	setup_game:
 *
 *	Initialises the game environment.
 */

void setup_game( void );

/*
 *	reset_game:
 *
 *	restores the game to its initial appearance and behaviour, as if starting over.
 */

void reset_game( void );

/*
 *	Cleans up the game, releasgin any resources.
 */

void cleanup_game( void );

/*
 *	play_turn:
 *
 *	Carries out one game cycle:
 *	-	Test for input and timing events;
 *	-	Update the state of the player;
 *	-	Update the state of the non-player characters.
 *
 *	Returns:
 *	-	TURN_OK: if all is ok and play continues;
 *	-	TURN_SCREEN_CHANGED: if something happened which changes the view;
 *	-	TURN_GAME_OVER: if the game is over.
 */

int play_turn( void );

/**
*	Renders a new frame in the game view.
*/

void draw_game( void );

#endif
