#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>
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
#define BTN0_CONF	(DDRB &= ~(1U << PORTB0))
#define BTN0_PRESSED	((PINB >> PORTB0) & 1U)
#define BTN1_CONF	(DDRB &= ~(1U << PORTB1))
#define BTN1_PRESSED	((PINB >> PORTB1) & 1U)
#elif defined(QUT_TEENSY_TWO)
#define BTN0_CONF	(DDRF &= ~(1U << PORTF6))
#define BTN0_PRESSED	((PINF >> PORTF6) & 1U)
#define BTN1_CONF	(DDRF &= ~(1U << PORTF5))
#define BTN1_PRESSED	((PINF >> PORTF5) & 1U)

#define DPAD_CONFIG		((DDRD &= ~((1U << PORTD0) | (1U << PORTD1))), (DDRB &= ~((1U << PORTB0) | (1U << PORTB1) | (1U << PORTB7))))
#define DPAD_UP_PRESSED		((PIND >> PORTD1) & 1U)
#define DPAD_DOWN_PRESSED	((PINB >> PORTB7) & 1U)
#define DPAD_LEFT_PRESSED	((PINB >> PORTB1) & 1U)
#define DPAD_RIGHT_PRESSED	((PIND >> PORTD0) & 1U)
#define DPAD_CENTER_PRESSED	((PINB >> PORTB0) & 1U)
#endif

#define LED0_CONF	(DDRB |= (1U << PORTB2))
#define LED0_ON		(PORTB |= (1U << PORTB2))
#define LED0_OFF	(PORTB &= ~(1U << PORTB2))
#define LED0_TOGGLE	(PORTB ^= (1U << PORTB2))

#define LED1_CONF	(DDRB |= (1U << PORTB3))
#define LED1_ON		(PORTB |= (1U << PORTB3))
#define LED1_OFF	(PORTB &= ~(1U << PORTB3))
#define LED1_TOGGLE	(PORTB ^= (1U << PORTB3))

#define	LED2_CONF	(DDRD |= (1U << PORTD6))
#define LED2_ON		(PORTD |= (1U << PORTD6))
#define LED2_OFF	(PORTD &= ~(1U << PORTD6))
#define LED2_TOGGLE	(PORTD ^= (1U << PORTD6))

volatile uint8_t btn0Pressed = 0U;
volatile uint8_t btn0Changed = 0U;
volatile uint8_t btn1Pressed = 0U;
volatile uint8_t btn1Changed = 0U;

#ifdef QUT_TEENSY_TWO
volatile uint8_t dpadUpPressed = 0U;
volatile uint8_t dpadUpChanged = 0U;

volatile uint8_t dpadDownPressed = 0U;
volatile uint8_t dpadDownChanged = 0U;

volatile uint8_t dpadLeftPressed = 0U;
volatile uint8_t dpadLeftChanged = 0U;

volatile uint8_t dpadRightPressed = 0U;
volatile uint8_t dpadRightChanged = 0U;

volatile uint8_t dpadCenterPressed = 0U;
volatile uint8_t dpadCenterChanged = 0U;
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

#ifdef QUT_TEENSY_TWO
	PORTC |= (1U << PORTC7);
#endif

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
	static uint8_t count = 0U;
	static uint8_t button_state = 0U;

	uint8_t raw_state = DPAD_UP_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0U) {
			dpadUpPressed = raw_state;
			dpadUpChanged = (button_state != raw_state) || dpadUpChanged;
			button_state = raw_state;

			count = 4U;
		}
		else {
			--count;
		}
	}
	else if (count > 0U) {
		--count;
	}
}

void debounceDpadDown(void) {
	static uint8_t count = 0U;
	static uint8_t button_state = 0U;

	uint8_t raw_state = DPAD_DOWN_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0U) {
			dpadDownPressed = raw_state;
			dpadDownChanged = (button_state != raw_state) || dpadDownChanged;
			button_state = raw_state;

			count = 4U;
		}
		else {
			--count;
		}
	}
	else if (count > 0U) {
		--count;
	}
}

void debounceDpadLeft(void) {
	static uint8_t count = 0U;
	static uint8_t button_state = 0U;

	uint8_t raw_state = DPAD_LEFT_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0U) {
			dpadLeftPressed = raw_state;
			dpadLeftChanged = (button_state != raw_state) || dpadLeftChanged;
			button_state = raw_state;

			count = 4U;
		}
		else {
			--count;
		}
	}
	else if (count > 0U) {
		--count;
	}
}

void debounceDpadRight(void) {
	static uint8_t count = 0U;
	static uint8_t button_state = 0U;

	uint8_t raw_state = DPAD_RIGHT_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0U) {
			dpadRightPressed = raw_state;
			dpadRightChanged = (button_state != raw_state) || dpadRightChanged;
			button_state = raw_state;

			count = 4U;
		}
		else {
			--count;
		}
	}
	else if (count > 0U) {
		--count;
	}
}

void debounceDpadCenter(void) {
	static uint8_t count = 0U;
	static uint8_t button_state = 0U;

	uint8_t raw_state = DPAD_CENTER_PRESSED;

	if (raw_state != button_state) {
		if (count <= 0U) {
			dpadCenterPressed = raw_state;
			dpadCenterChanged = (button_state != raw_state) || dpadCenterChanged;
			button_state = raw_state;

			count = 4U;
		}
		else {
			--count;
		}
	}
	else if (count > 0U) {
		--count;
	}
}
#endif

#endif
