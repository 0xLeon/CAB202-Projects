#include <avr/io.h>
#include <stdio.h>
#include "hardware.h"
#include "graphics.h"

unsigned char checkButtons(void);
void handleLED(void);
void displayPressCount(unsigned int *press_count);

int main(void) {
	initializeHardware();

	unsigned int press_count = 0U;
	unsigned int *press_count_ptr = &press_count;

	while (1) {
		debounceBtn0();
		debounceBtn1();

		clear_screen();

		press_count += checkButtons();
		displayPressCount(press_count_ptr);

		handleLED();

		show_screen();

		btn0Changed = 0;
		btn1Changed = 0;
	}

	return 0;
}

unsigned char checkButtons(void) {
	unsigned char c = 0;

	if (btn0Pressed || btn1Pressed) {
		draw_string(0, 0, "Pressed");
	}
	else {
		draw_string(0, 0, "Released");
	}


	if (btn0Changed && !btn0Pressed) {
		++c;
	}

	if (btn1Changed && !btn1Pressed) {
		++c;
	}

	return c;
}

void handleLED(void) {
	if (btn0Pressed && btn1Pressed) {
		LED2_ON;
	}
	else {
		LED2_OFF;
	}

	if (btn0Changed) {
		LED0_TOGGLE;
	}

	if (btn1Changed) {
		LED1_TOGGLE;
	}
}

void displayPressCount(unsigned int *press_count) {
	static char buffer[20];

	snprintf(buffer, 20, "Press Count:% 4d", *press_count);
	draw_string(0, 10, buffer);
}
