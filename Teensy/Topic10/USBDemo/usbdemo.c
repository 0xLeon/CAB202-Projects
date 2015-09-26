/*
*  CAB202 Tutorial 10: Communicating over USB Serial
*	Question 1 - Solution
*
*	B.Talbot, September 2015
*	Queensland University of Technology
*/
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "cpu_speed.h"
#include "hardware.h"
#include "usb_serial.h"
#include "lcd.h"
#include "graphics.h"

/**
* Default buffer length
*/
#define BUFF_LENGTH	20

/**
* Function implementations
*/
void init_hardware(void);
void draw_centred(unsigned char y, char* string);
void send_line(char* string);
void recv_line(char* buff, unsigned char max_length);

/**
* Main - Run through the steps of configuring, greeting, getting a name, thanking,
* and then quitting
*/
int main(void) {
	char buff[BUFF_LENGTH];

	// Setup the hardware
	init_hardware();

	// Wait until the USB port is configured and ready to go
	draw_centred(17, "Waiting for");
	draw_centred(24, "computer...");
	show_screen();
	while (!usb_configured() || !usb_serial_get_control());

	// Prompt the user for their name, and wait until they enter it
	clear_screen();
	draw_centred(17, "Waiting for");
	draw_centred(24, "username...");
	show_screen();
	send_line("Hello!");
	send_line("Could you please tell me your name:");
	recv_line(buff, BUFF_LENGTH);
	usb_serial_putchar('\n');

	// Display their name on the Teensy and prompt them to exit
	char buff2[BUFF_LENGTH + 8];
	sprintf(buff2, "Thanks %s!", buff);
	clear_screen();
	draw_centred(21, buff2);
	show_screen();
	send_line("Press 'q' to exit...");
	while (usb_serial_getchar() != 'q');

	// Display the finished information
	clear_screen();
	draw_centred(21, "Goodbye!");
	show_screen();
	send_line("\r");
	send_line("Done! Goodbye!");
	while (1);

	// We'll never get here...
	return 0;
}

/**
* Function implementations
*/
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

void recv_line(char* buff, unsigned char max_length) {
	// Loop over storing characters until the buffer is full or a newline character is received
	unsigned char char_count = 0;
	int16_t curr_char;
	do {
		// BLOCK here until a character is received
		do {
			curr_char = usb_serial_getchar();
		}
		while (curr_char == -1);

		// Add to the buffer if it wasn't a newline (accepts other gibberish that may not necessarily be a character!)
		if (curr_char != '\n' && curr_char != '\r') {
			buff[char_count] = curr_char;
			char_count++;
		}
	}
	while (curr_char != '\n' && curr_char != '\r' && char_count < max_length - 1);

	// Add the null terminator to the end of the string
	buff[char_count] = '\0';
}
