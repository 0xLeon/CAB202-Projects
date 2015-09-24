#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include "hardware.h"
#include "graphics.h"

#define TIMER1_TIME		(2.1f)
#define TIMER1_FREQUENCY	((float) F_CPU)
#define TIMER1_PRESCALER	(256.f)
#define TIMER1_TICK_COUNT	((TIMER1_TIME) * ((TIMER1_FREQUENCY) / (TIMER1_PRESCALER)))
#define TIMER1_CURRENT_TIME(x)	((x) * ((TIMER1_PRESCALER) / (TIMER1_FREQUENCY)))

void initializeTimer1(void);
void displayTime(float currentTime);
void displayTickCount(void);
void displayOverflowCount(void);

volatile unsigned int timer1Overflow = 0;

int main(void) {
	sei();
	initializeHardware();
	initializeTimer1();

	LED0_OFF;
	LED1_OFF;
	LED2_ON;

	while (1) {
		if (TCNT1 >= TIMER1_TICK_COUNT) {
			TCNT1 = 0;
		}

		clear_screen();

		displayTime(TIMER1_CURRENT_TIME(TCNT1));
		displayTickCount();
		displayOverflowCount();

		show_screen();
	}

	return 0;
}

void initializeTimer1(void) {
	TCCR1B &= ~(1 << WGM12);

	TCCR1B |= (1 << CS12);

	TIMSK1 |= (1 << TOIE1);

	TCNT1 = 0;
}

ISR(TIMER1_OVF_vect) {
	++timer1Overflow;

	LED0_TOGGLE;
	LED1_TOGGLE;
	LED2_TOGGLE;
}

void displayTime(float currentTime) {
	static char buffer[20];

	snprintf(buffer, 20, "SystemTime: %.2f", currentTime);
	draw_string(0, 0, buffer);
}

void displayTickCount(void) {
	static char buffer[20];

	snprintf(buffer, 20, "TCNT1:%6u", TCNT1);
	draw_string(0, 10, buffer);
}

void displayOverflowCount(void) {
	static char buffer[20];

	cli();
	snprintf(buffer, 20, "OverflowCnt:%4u", timer1Overflow);
	draw_string(0, 20, buffer);
	sei();
}
