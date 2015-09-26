/*
*  CAB202 Tutorial 10: Communicating over USB Serial
*	Question 2 & 3 - Template
*
*	B.Talbot, September 2015
*	Queensland University of Technology
*/
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
* Defines used in system time calculations
*/
#define FREQUENCY	((float) F_CPU)
#define PRESCALER_0	1024.f
#define PRESCALER_1	1024.f

/**
* Default buffer length
*/
#define BUFF_LENGTH	20

/**
* Global overflow count for TIMER1 - keeps time increasing rather than wrapping
*/
volatile unsigned long ovf_count = 0;

volatile unsigned char in_breakpoint = 0;

/**
* Function implementations
*/
// Series of functions used in debugging
void send_debug_string(char* string);
float get_system_time(void);
void enter_breakpoint(unsigned long line_num);

// Other helper functions
void init_hardware(void);
void draw_centred(unsigned char y, char* string);
void send_line(char* string);
void recv_line(char* buff, unsigned char max_length);

/**
* Main - Run the main program which prints the system time and flashes the LEDs
* if certain conditions are met
*/
int main(void) {
	init_hardware();

	// Wait until the 'debugger' is attached...
	draw_centred(17, "Waiting for");
	draw_centred(24, "debugger...");
	show_screen();
	while (!usb_configured() || !usb_serial_get_control());
	send_line("Debugger initialized. Debugging strings will appear below:");

	// Run the main loop displaying the system time @ ~10Hz...
	char buff[BUFF_LENGTH];
	unsigned long count = 0;

	send_debug_string("Entering main loop");
	while (1) {

		// Draw the current system time on the screen
		clear_screen();
		sprintf(buff, "%7.4f", get_system_time());
		draw_centred(21, buff);

		// send_debug_string("Calling show_screen()");
		show_screen();
		// send_debug_string("Finished show_screen()");

		_delay_ms(100);

		// Toggle LEDs if the conditions are met
		if ((count % 25) == 0) {
			LED0_TOGGLE;
			send_debug_string("LED0 was toggled.");
		}

		if ((count % 50) == 10) {
			LED1_TOGGLE;
			send_debug_string("LED1 was toggled.");
		}

		if ((count % 100) == 0) {
			enter_breakpoint(__LINE__);
		}

		// Increment the loop count
		count++;
	}

	// We'll never get here...
	return 0;
}

/**
* Function implementations
*/
void send_debug_string(char* string) {
	if (in_breakpoint) {
		return;
	}

	char b[16];
	snprintf(b, 16, "[DBG @ %06.2f] ", get_system_time());
	usb_serial_write(((unsigned char*) (b)), 16);

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

float get_system_time(void) {
	return (ovf_count * 65536 + TCNT1) * PRESCALER_1 / FREQUENCY;
}

void enter_breakpoint(unsigned long line_num) {
	// Implement this so that it correctly pauses the code and displays the message
	// over serial. NOTHING except the serial connection and the incrementing of
	// ovf_count should still be happening.
	// TODO

	char b[60];
	snprintf(b, 60, "Entered breakpoint @ line #%06lu. Press 'b' to continue...", line_num);
	send_debug_string(b);

	in_breakpoint = 1;

	while ('b' != usb_serial_getchar());

	in_breakpoint = 0;

	send_line("");
	send_debug_string("Left breakpoint.");
}

void init_hardware(void) {
	initializeHardware();

	// Initialise the USB serial
	usb_init();

	// Setup two timers with overflow interrupts:
	// 0 - button press checking @ ~30Hz
	// 1 - system time overflowing every 8.3s
	TCCR0B |= ((1 << CS02) | (1 << CS00));
	TCCR1B |= ((1 << CS12) | (1 << CS10));
	TIMSK0 |= (1 << TOIE0);
	TIMSK1 |= (1 << TOIE1);

	// Enable interrupts globally
	sei();
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

/**
* Interrupt service routines
*/
ISR(TIMER0_OVF_vect) {
	// De-bounced button press checking
	if (BTN0_PRESSED) {
		send_debug_string("Press of SW0 detected.");
		_delay_ms(50);
		while (BTN0_PRESSED);
	}
}

ISR(TIMER1_OVF_vect) {
	++ovf_count;

	send_debug_string("TIMER1 overflowed.");
}
