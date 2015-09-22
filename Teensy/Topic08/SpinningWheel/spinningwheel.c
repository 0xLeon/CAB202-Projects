/*
* CAB202 Tutorial 8: Display
* spinning_wheel.c (Questions 2 and 3)
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

// Defines for parametrising our wheel creation
#define CIRCLE_NUM_STEPS	180
#define CIRCLE_R		10
#define CIRCLE_STEP_SIZE	2 * M_PI / CIRCLE_NUM_STEPS
#define NUM_SPOKES		4
#define SPOKE_STEP		M_PI / NUM_SPOKES

// Helper function for drawing a wheel at x,y with rotation theta
void draw_wheel(char center_x, char center_y, float theta);

/**
 * Main
 */
int main(void) {
	// Set the clock prescaler and initialise the screen
	set_clock_speed(CPU_8MHz);
	LCDInitialise(LCD_DEFAULT_CONTRAST);

	// Initialise the wheel starting position as the middle of the left edge
	unsigned char c_x = CIRCLE_R;
	unsigned char c_y = 24;
	float theta = 0.f;

	// Run the main loop indefinitely
	while (1) {
		// Clear the screen and draw the text
		clear_screen();
		draw_string(0, 0, "Hello Andy");
		draw_string(0, 40, "I'm n9529977");

		// Draw the wheel
		draw_wheel(c_x, c_y, theta);

		// Show the screen and don't nap - running at max speed
		show_screen();
		//_delay_ms(5);

		// Increase the position and rotate the wheel
		c_x += 2;
		theta += M_PI / 20;

		// Wrap the wheel back to the left edge of the screen if necessary
		if (c_x + CIRCLE_R > 83) {
			c_x = CIRCLE_R;
		}
	}

	// We'll never make it here!
	return 0;
}

// Helper function implementation - nothing much to see here besides some crude
// trigonometry. Consider what would happen if we tried to draw something off the
// screen with this function. Would it be safe? Or would it draw at random locations?
void draw_wheel(char center_x, char center_y, float theta) {
	// Draw the outer wheel (n steps, each 360/n degrees around the circle). Increasing
	// CIRCLE_NUM_STEPS makes a sharper circle - try changing CIRCLE_NUM_STEPS to 60...
	for (int i = 0; i < CIRCLE_NUM_STEPS; ++i) {
		set_pixel(
			center_x + round(CIRCLE_R * cos(i * CIRCLE_STEP_SIZE)),
			center_y + round(CIRCLE_R * sin(i * CIRCLE_STEP_SIZE)),
			1
		);
	}

	// Draw the spokes in the wheel (n spokes starting at 180/n degrees around the
	// circle. They then extend to the opposite side - i.e. +180 degrees)
	for (int i = 0; i < NUM_SPOKES; ++i) {
		draw_line(
			center_x + round(CIRCLE_R * cos(i * SPOKE_STEP + theta)),
			center_y + round(CIRCLE_R * sin(i * SPOKE_STEP + theta)),
			center_x + round(CIRCLE_R * cos(i * SPOKE_STEP + M_PI + theta)),
			center_y + round(CIRCLE_R * sin(i * SPOKE_STEP + M_PI + theta))
		);
	}
}
