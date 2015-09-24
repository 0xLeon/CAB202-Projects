#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <inttypes.h>
#include <avr/io.h>
#include "cpu_speed.h"
#include "lcd.h"
#include "graphics.h"

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

void initializeHardware(void);
void debounceBtn0(void);
void debounceBtn1(void);

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
		++count;

		if (count >= 3) {
			btn0Pressed = raw_state;
			btn0Changed = (button_state != raw_state);
			button_state = raw_state;

			count = 0;
		}
	}
	else {
		count = 0;
	}
}

void debounceBtn1(void) {
	static uint8_t count = 0;
	static uint8_t button_state = 0;

	uint8_t raw_state = BTN1_PRESSED;

	if (raw_state != button_state) {
		++count;

		if (count >= 3) {
			btn1Pressed = raw_state;
			btn1Changed = (button_state != raw_state);
			button_state = raw_state;

			count = 0;
		}
	}
	else {
		count = 0;
	}
}

#endif
