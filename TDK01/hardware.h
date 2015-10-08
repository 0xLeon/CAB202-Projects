#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <inttypes.h>
#include <avr/io.h>
#include "cpu_speed.h"
#include "lcd.h"
#include "graphics.h"
#include "buttons.h"

#if !defined(QUT_TEENSY_ONE) && !defined(QUT_TEENSY_TWO)
#warning No QUT Teensy Board version specified, will assume QUT Teensy One
#define QUT_TEENSY_ONE
#endif

#if defined(QUT_TEENSY_ONE)
#define BTN0_CONF	(DDRB &= ~(1 << PORTB0))
#define BTN0_PRESSED	((PINB & (1 << PORTB0)) >> PORTB0)
#define BTN1_CONF	(DDRB &= ~(1 << PORTB1))
#define BTN1_PRESSED	((PINB & (1 << PORTB1)) >> PORTB1)
#elif defined(QUT_TEENSY_TWO)
#define BTN0_CONF	(DDRF &= ~(1 << PORTF6))
#define BTN0_PRESSED	((PINF & (1 << PORTF6)) >> PORTF6)
#define BTN1_CONF	(DDRF &= ~(1 << PORTF5))
#define BTN1_PRESSED	((PINF & (1 << PORTF5)) >> PORTF5)

#define DPAD_CONFIG		((DDRD &= ~((1 << PORTD0) | (1 << PORTD1))), (DDRB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB7))))
#define DPAD_UP_PRESSED		((PIND & (1 << PORTD1)) >> PORTD1)
#define DPAD_DOWN_PRESSED	((PINB & (1 << PORTB7)) >> PORTB7)
#define DPAD_LEFT_PRESSED	((PINB & (1 << PORTB1)) >> PORTB1)
#define DPAD_RIGHT_PRESSED	((PIND & (1 << PORTD0)) >> PORTD0)
#define DPAD_CENTER_PRESSED	((PINB & (1 << PORTB0)) >> PORTB0)
#endif

#define LED0_CONF	(DDRB |= (1 << PORTB2))
#define LED0_ON		(PORTB |= (1 << PORTB2))
#define LED0_OFF	(PORTB &= ~(1 << PORTB2))
#define LED0_TOGGLE	(PORTB ^= (1 << PORTB2))

#define LED1_CONF	(DDRB |= (1 << PORTB3))
#define LED1_ON		(PORTB |= (1 << PORTB3))
#define LED1_OFF	(PORTB &= ~(1 << PORTB3))
#define LED1_TOGGLE	(PORTB ^= (1 << PORTB3))

#define	LED2_CONF	(DDRD |= (1 << PORTD6))
#define LED2_ON		(PORTD |= (1 << PORTD6))
#define LED2_OFF	(PORTD &= ~(1 << PORTD6))
#define LED2_TOGGLE	(PORTD ^= (1 << PORTD6))

volatile unsigned char btn0Pressed = 0;
volatile unsigned char btn0Changed = 0;
volatile unsigned char btn1Pressed = 0;
volatile unsigned char btn1Changed = 0;

#ifdef QUT_TEENSY_TWO
volatile unsigned char dpadUpPressed = 0;
volatile unsigned char dpadUpChanged = 0;

volatile unsigned char dpadDownPressed = 0;
volatile unsigned char dpadDownChanged = 0;

volatile unsigned char dpadLeftPressed = 0;
volatile unsigned char dpadLeftChanged = 0;

volatile unsigned char dpadRightPressed = 0;
volatile unsigned char dpadRightChanged = 0;

volatile unsigned char dpadCenterPressed = 0;
volatile unsigned char dpadCenterChanged = 0;
#endif

void initializeHardware(void);
void debounceBtn0(void);
void debounceBtn1(void);

#ifdef QUT_TEENSY_TWO
void debounceDpad(void);

void debounceDpadUp(void);
void debounceDpadDown(void);
void debounceDpadLeft(void);
void debounceDpadRight(void);
void debounceDpadCenter(void);
#endif

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
			btn0Changed = (button_state != raw_state) || btn0Changed;
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
			btn1Changed = (button_state != raw_state) || btn1Changed;
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

#endif
