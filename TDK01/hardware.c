#include <inttypes.h>
#include <avr/io.h>
#include "cpu_speed.h"
#include "lcd.h"
#include "graphics.h"
#include "hardware.h"

void initializeHardware(void) {
	set_clock_speed(CPU_8MHz);
	LCDInitialise(LCD_DEFAULT_CONTRAST);
	clear_screen();

	BTN0_CONF;
	LED0_CONF;
	LED1_CONF;
	LED2_CONF;
}

void debounceBtn0(void) {
	static uint8_t count = 0;
	static uint8_t button_state = 0;

	uint8_t raw_state = BTN0_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0) {
			btn0Pressed = raw_state;
			btn0Changed = (button_state != raw_state);
			button_state = raw_state;

			count = 4;
		}
		else {
			--count;
		}
	}
	else if (count > 0) {
		--count;
	}
}

void debounceBtn1(void) {
	static uint8_t count = 0;
	static uint8_t button_state = 0;

	uint8_t raw_state = BTN1_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0) {
			btn1Pressed = raw_state;
			btn1Changed = (button_state != raw_state);
			button_state = raw_state;

			count = 4;
		}
		else {
			--count;
		}
	}
	else if (count > 0) {
		--count;
	}
}
