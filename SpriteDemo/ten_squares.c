/*
*	File:	one_square.c
*	Author: Lawrence Buckingham, Queensland University of Technology.
*	Date:	27 April 2015.
*
*	Implementation of the functions provided by the one_square "game".
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ten_squares.h"
#include "cab202_sprites.h"
#include "cab202_graphics.h"
#include "cab202_timers.h"

void setup_ten_squares(
	sprite_id sprites[],
	char *bitmaps[],
	char widths[],
	char heights[],
	int sprite_count,
	int bitmap_count
	);

void update_ten_squares( sprite_id sprites[] );
void draw_ten_squares( sprite_id sprites[] );
void cleanup_ten_squares( sprite_id sprites[] );
void update_one_square( sprite_id sprite );

void play_ten_squares() {
	clear_screen();
	draw_string( 0, screen_height() - 1, "Press any key to continue..." );
	show_screen();
	wait_char();

	char bitmap1[] =
		"11111111"
		"1      1"
		"1      1"
		"1      1"
		"1      1"
		"1      1"
		"1      1"
		"11111111"
		;

	char bitmap2[] =
		"11111"
		"1   1"
		"1   1"
		"1   1"
		"11111"
		;

	char bitmap3[] = {
		"111111111111"
		"1          1"
		"1          1"
		"1          1"
		"1          1"
		"1          1"
		"1          1"
		"111111111111"
	};

	char *bitmaps[3] = { bitmap1, bitmap2, bitmap3 };
	char widths[] = { 8, 5, 12 };
	char heights[] = { 8, 5, 8 };

#define SPR_COUNT (10)

	sprite_id sprites[SPR_COUNT];

	setup_ten_squares(
		sprites, bitmaps, widths, heights,
		SPR_COUNT,
		sizeof( bitmaps ) / sizeof( bitmaps[0] )
		);

	int key = 0;

	while ( key != 'q' ) {
		clear_screen();

		update_ten_squares( sprites );
		draw_ten_squares( sprites );
		draw_string( 0, screen_height() - 1, "Press 'q' to exit..." );
		show_screen();

		timer_pause( 50 );
		key = get_char();
	}

	cleanup_ten_squares( sprites );
}

void setup_ten_squares(
	sprite_id sprites[],
	char *bitmaps[],
	char widths[],
	char heights[],
	int sprite_count,
	int bitmap_count
	) {
	for ( int i = 0; i < sprite_count; i++ ) {

		char * bitmap = bitmaps[i % bitmap_count];
		char width = widths[i % bitmap_count];
		char height = heights[i % bitmap_count];

		sprite_id sprite = create_sprite( 0, 0, width, height, bitmap );
		double angle = ( (double) random() ) * 2 * M_PI / RAND_MAX;
		sprite->dx = cos( angle );
		sprite->dy = sin( angle );

		sprites[i] = sprite;
	}
}

void update_ten_squares( sprite_id sprites[] ) {
	for ( int i = 0; i < SPR_COUNT; i++ ) {
		update_one_square( sprites[i] );
	}
}

void draw_ten_squares( sprite_id sprites[] ) {
	for ( int i = 0; i < SPR_COUNT; i++ ) {
		draw_sprite( sprites[i] );
	}
}

void cleanup_ten_squares( sprite_id sprites[] ) {
	for ( int i = 0; i < SPR_COUNT; i++ ) {
		destroy_sprite( sprites[i] );
	}
}
