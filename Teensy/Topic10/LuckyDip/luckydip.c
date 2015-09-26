/*
*  CAB202 Tutorial 10: Communicating over USB Serial
*	Question 4 - Template
*
*	B.Talbot, September 2015
*	Queensland University of Technology
*/
#include <stdlib.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "cpu_speed.h"
#include "hardware.h"
#include "usb_serial.h"
#include "lcd.h"
#include "graphics.h"


/**
* Constants used in defining the box configuration
*/
#define BOX_RADIUS	6
#define BOXES_W		3
#define BOXES_H		3

/**
* Global variables for the status of the boxes (used as Booleans)
*/
unsigned char is_gold[BOXES_W * BOXES_H];
unsigned char is_open[BOXES_W * BOXES_H];

/**
* Function implementations
*/
// Completed functions used in the game
void bury_gold(void);
void draw_box(unsigned char box_num, unsigned char draw_open);

// Other helper functions
void init_hardware(void);
void draw_centred(unsigned char y, char* string);
void send(char* string);
void send_line(char* string);

/**
* Main - Run the game which repeatedly loops through trying to find the gold
*/
int main(void) {
	init_hardware();

	// Wait until the 'player' is attached...
	draw_centred(17, "Waiting for");
	draw_centred(24, "the player...");
	show_screen();
	while (!usb_configured() || !usb_serial_get_control());

	// Run the main game loop
	unsigned int seed = 0;
	while (1) {
		unsigned char opened = 0;

		// Game start screen
		clear_screen();
		draw_centred(16, "--- LUCKY DIP ---");
		draw_centred(25, "'s' to start...");
		show_screen();
		send_line("--- LUCKY DIP ---");
		send_line("Press 's' to start...");

		// Wait until the key has been pressed (perform seeding only if first run)
		unsigned int seed_temp = 0;
		int16_t curr_char;
		do {
			curr_char = usb_serial_getchar();
			seed_temp++;
		}
		while (curr_char != 's');

		if (seed == 0) {
			seed = seed_temp;
			srand(seed);
		}
		usb_serial_write("\r\n", 2);

		// Set the gold location
		bury_gold();

		// Present the 9 closed boxes
		clear_screen();
		for (unsigned char i = 0; i < BOXES_W * BOXES_H; i++) {
			draw_box(i, 0);
		}
		show_screen();

		// Prompt the user to pick a box by pressing a number key until they find
		// the gold
		// TODO

		int pick;
		while (opened < (BOXES_W * BOXES_H)) {
			send("Enter a number between 1 and 9.\r\n > ");

			while (1) {
				pick = usb_serial_getchar();

				if (-1 != pick) {
					send_line("");
					break;
				}
			};

			if (((pick - 49) >= 0) && ((pick - 49) <= 8)) {
				pick -= 49;

				if (is_open[pick]) {
					send_line("You already opened that box.");
					continue;
				}

				is_open[pick] = 1;
				++opened;

				clear_screen();
				for (unsigned char i = 0; i < BOXES_W * BOXES_H; i++) {
					draw_box(i, is_open[i]);
				}
				show_screen();

				if (is_gold[pick]) {
					break;
				}
			}
			else {
				send_line("Invalid key pressed.");
			}
		}

		_delay_ms(100);

		// Winner, winner, chicken dinner
		send_line("You found the $$$!");
		for (unsigned char i = 0; i < 10; i++) {
			PORTB ^= (1 << PB2);
			PORTB ^= (1 << PB3);
			_delay_ms(250);
		}
	}

	// We'll never get here...
	return 0;
}

/**
* Function implementations
*/
void bury_gold(void) {
	// Close all the boxes and 'bury' the gold
	unsigned char i, gold_loc = rand() % (BOXES_W * BOXES_H);

	for (i = 0; i< BOXES_W * BOXES_H; i++) {
		is_gold[i] = (i == gold_loc) ? 1 : 0;
		is_open[i] = 0;
	}
}

void draw_box(unsigned char box_num, unsigned char draw_open) {
	// Figure out box information
	unsigned char c_x, c_y, min_x, max_x, min_y, max_y;
	c_x = LCD_X * (box_num % BOXES_W + .5f) / ((float) BOXES_W);
	c_y = LCD_Y * (box_num / BOXES_W + .5f) / ((float) BOXES_H);
	min_x = c_x - BOX_RADIUS;
	max_x = c_x + BOX_RADIUS;
	min_y = c_y - BOX_RADIUS;
	max_y = c_y + BOX_RADIUS;

	// Draw the open or closed box
	if (draw_open) {
		// Draw the outlined box
		for (unsigned char x = min_x; x <= max_x; x++) {
			for (unsigned char y = min_y; y <= max_y; y++) {
				set_pixel(x, y, (x == min_x || x == max_x || y == min_y || y == max_y) ? 1 : 0);
			}
		}

		// Draw the character
		draw_char(c_x - 2, c_y - 3, (is_gold[box_num]) ? '$' : 'X');
	}
	else {
		// Draw the filled box
		for (unsigned char x = min_x; x <= max_x; x++) {
			for (unsigned char y = min_y; y <= max_y; y++) {
				set_pixel(x, y, 1);
			}
		}
	}
}

void init_hardware(void) {
	initializeHardware();

	// Initialise the USB serial
	usb_init();
}

void draw_centred(unsigned char y, char* string) {
	// Draw a string centred in the LCD when you don't know the string length
	unsigned char l = 0, i = 0;
	while (string[i] != '\0') {
		l++;
		i++;
	}
	char x = 42 - (l * 5 / 2);
	draw_string((x > 0) ? x : 0, y, string);
}

void send(char* string) {
	// Send all of the characters in the string
	unsigned char char_count = 0;
	while (*string != '\0') {
		usb_serial_putchar(*string);
		string++;
		char_count++;
	}
}

void send_line(char* string) {
	// Send all of the characters in the string
	unsigned char char_count = 0;
	while (*string != '\0') {
		usb_serial_putchar(*string);
		string++;
		char_count++;
	}

	// Go to a new line (force this to be the start of the line)
	usb_serial_putchar('\r');
	usb_serial_putchar('\n');
}
