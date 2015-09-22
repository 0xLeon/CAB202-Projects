/*
* CAB202 Tutorial 8: Display
* frowny.c (Question4)
* 
* B.Talbot, September 2015
* Queensland University of Technology
*/
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#include "cpu_speed.h"
#include "lcd.h"
#include "graphics.h"
#include "sprite.h"

// Useful defines
#define NUM_FACES	5
#define BYTES_PER_FACE	32

// Array of bitmaps (i.e. an array of unsigned char arrays) that are used as the
// different sprite emotions
unsigned char bm_faces[NUM_FACES][BYTES_PER_FACE] = {
	{
		0b00000111, 0b11100000,
		0b00011000, 0b00011000,
		0b00100000, 0b00000100,
		0b01000000, 0b00000010,
		0b01011000, 0b00011010,
		0b10011000, 0b00011001,
		0b10000000, 0b00000001,
		0b10000000, 0b00000001,
		0b10010000, 0b00001001,
		0b10010000, 0b00001001,
		0b10001000, 0b00010001,
		0b01000111, 0b11100010,
		0b01000000, 0b00000010,
		0b00100000, 0b00000100,
		0b00011000, 0b00011000,
		0b00000111, 0b11100000
	},
	{
		0b00000111, 0b11100000,
		0b00011000, 0b00011000,
		0b00100000, 0b00000100,
		0b01000000, 0b00000010,
		0b01011000, 0b00011010,
		0b10011000, 0b00011001,
		0b10000000, 0b00000001,
		0b10000000, 0b00000001,
		0b10000000, 0b00000001,
		0b10110000, 0b00001101,
		0b10001111, 0b11110001,
		0b01000000, 0b00000010,
		0b01000000, 0b00000010,
		0b00100000, 0b00000100,
		0b00011000, 0b00011000,
		0b00000111, 0b11100000
	},
	{
		0b00000111, 0b11100000,
		0b00011000, 0b00011000,
		0b00100000, 0b00000100,
		0b01000000, 0b00000010,
		0b01011000, 0b00011010,
		0b10011000, 0b00011001,
		0b10000000, 0b00000001,
		0b10000000, 0b00000001,
		0b10000000, 0b00000001,
		0b10111111, 0b11111101,
		0b10000000, 0b00000001,
		0b01000000, 0b00000010,
		0b01000000, 0b00000010,
		0b00100000, 0b00000100,
		0b00011000, 0b00011000,
		0b00000111, 0b11100000
	},
	{
		0b00000111, 0b11100000,
		0b00011000, 0b00011000,
		0b00100000, 0b00000100,
		0b01000000, 0b00000010,
		0b01011000, 0b00011010,
		0b10011000, 0b00011001,
		0b10000000, 0b00000001,
		0b10000000, 0b00000001,
		0b10000000, 0b00000001,
		0b10001111, 0b11110001,
		0b10110000, 0b00001101,
		0b01000000, 0b00000010,
		0b01000000, 0b00000010,
		0b00100000, 0b00000100,
		0b00011000, 0b00011000,
		0b00000111, 0b11100000
	},
	{
		0b00000111, 0b11100000,
		0b00011000, 0b00011000,
		0b00100000, 0b00000100,
		0b01000000, 0b00000010,
		0b01011000, 0b00011010,
		0b10011000, 0b00011001,
		0b10000000, 0b00000001,
		0b10000000, 0b00000001,
		0b10000111, 0b11100001,
		0b10001000, 0b00010001,
		0b10010000, 0b00001001,
		0b01010000, 0b00001010,
		0b01000000, 0b00000010,
		0b00100000, 0b00000100,
		0b00011000, 0b00011000,
		0b00000111, 0b11100000
	}
};

/**
* Main
*/
int main(void) {
	// Set the clock prescaler and initialise the screen
	set_clock_speed(CPU_8MHz);
	LCDInitialise(LCD_DEFAULT_CONTRAST);

	// Set the face Id, create the sprite, and initialise it (provide it a null
	// pointer and let the main loop set the bitmap image)
	int face_id = NUM_FACES - 1;
	Sprite faces;
	init_sprite(&faces, 34, 16, 16, 16, 0);

	// Run the face transition loop
	while (1) {
		// Clear the screen
		clear_screen();

		// Update the sprite face
		faces.bitmap = bm_faces[face_id % NUM_FACES];

		// Draw sprite, show screen, rest
		draw_sprite(&faces);
		show_screen();
		_delay_ms(500);

		// Increment the sprite face, and have a break if we are back to the start
		face_id = (face_id + NUM_FACES - 1) % NUM_FACES;
		if (4 == (face_id % NUM_FACES)) {
			clear_screen();
			show_screen();
			_delay_ms(3000);
		}
	}

	// We'll never make it here!
	return 0;
}
