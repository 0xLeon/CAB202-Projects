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

#ifdef QUT_TEENSY_TWO
	DPAD_CONFIG;
#endif

	BTN0_CONF;
	BTN1_CONF;
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
#ifdef QUT_TEENSY_TWO
void debounceDpad(void) {
	debounceDpadUp();
	debounceDpadDown();
	debounceDpadLeft();
	debounceDpadRight();
	debounceDpadCenter();
}

void debounceDpadUp(void) {
	static uint8_t count = 0;
	static uint8_t button_state = 0;

	uint8_t raw_state = DPAD_UP_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0) {
			dpadUpPressed = raw_state;
			dpadUpChanged = (button_state != raw_state) || dpadUpChanged;
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

void debounceDpadDown(void) {
	static uint8_t count = 0;
	static uint8_t button_state = 0;

	uint8_t raw_state = DPAD_DOWN_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0) {
			dpadDownPressed = raw_state;
			dpadDownChanged = (button_state != raw_state) || dpadDownChanged;
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

void debounceDpadLeft(void) {
	static uint8_t count = 0;
	static uint8_t button_state = 0;

	uint8_t raw_state = DPAD_LEFT_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0) {
			dpadLeftPressed = raw_state;
			dpadLeftChanged = (button_state != raw_state) || dpadLeftChanged;
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

void debounceDpadRight(void) {
	static uint8_t count = 0;
	static uint8_t button_state = 0;

	uint8_t raw_state = DPAD_RIGHT_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0) {
			dpadRightPressed = raw_state;
			dpadRightChanged = (button_state != raw_state) || dpadRightChanged;
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

void debounceDpadCenter(void) {
	static uint8_t count = 0;
	static uint8_t button_state = 0;

	uint8_t raw_state = DPAD_CENTER_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0) {
			dpadCenterPressed = raw_state;
			dpadCenterChanged = (button_state != raw_state) || dpadCenterChanged;
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
#endif
