#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include "hardware.h"
#include "graphics.h"
#include "sprite.h"

#define TIMER0_TIME		(0.5f)
#define TIMER0_FREQUENCY	((float) F_CPU)
#define TIMER0_PRESCALER	(256.f)
#define TIMER0_TICK_COUNT	((TIMER0_TIME) * ((TIMER0_FREQUENCY) / (TIMER0_PRESCALER)))
#define TIMER0_CURRENT_TIME(x)	((x) * ((TIMER0_PRESCALER) / (TIMER0_FREQUENCY)))

#define TIMER1_TIME		(8.3f)
#define TIMER1_FREQUENCY	((float) F_CPU)
#define TIMER1_PRESCALER	(1024.f)
#define TIMER1_TICK_COUNT	((TIMER1_TIME) * ((TIMER1_FREQUENCY) / (TIMER1_PRESCALER)))
#define TIMER1_CURRENT_TIME(x)	((x) * ((TIMER1_PRESCALER) / (TIMER1_FREQUENCY)))

#define BYTES_PER_FACE	32
unsigned char bm_face[BYTES_PER_FACE] = {
	0b00000111, 0b11100000,
	0b00011000, 0b00011000,
	0b00100000, 0b00000100,
	0b01000000, 0b00000010,
	0b01011000, 0b00011010,
	0b10011000, 0b00011001,
	0b10000000, 0b00000001,
	0b10000000, 0b00000001,
	0b10010000, 0b00001001,
	0b10010000, 0b00001001,
	0b10001000, 0b00010001,
	0b01000111, 0b11100010,
	0b01000000, 0b00000010,
	0b00100000, 0b00000100,
	0b00011000, 0b00011000,
	0b00000111, 0b11100000
};
float face_x = 0;

void initializeTimer0(void);
void initializeTimer1(void);
void initButtonInterrupt(void);
void displayTime(float currentTime);

volatile unsigned char pause = 0;

unsigned int timer0Overflow = 0;

int main(void) {
	sei();
	initializeHardware();
	initializeTimer0();
	initializeTimer1();

	LED0_ON;
	LED1_OFF;

	Sprite face;
	Sprite *face_ptr = &face;

	init_sprite(face_ptr, 0.f, 16.f, 16, 16, bm_face);

	while (1) {
		debounceBtn0();
		debounceBtn1();

		if (TCNT1 >= TIMER1_TICK_COUNT) {
			TCNT1 = 0;
		}

		if (btn0Pressed || btn1Pressed) {
			pause = 1;
		}
		else {
			pause = 0;
		}

		if (!pause) {
			face_x += 1.f;

			if (face_x >= 68) {
				face_x = 0;
			}

			face_ptr->x = face_x;

			clear_screen();

			displayTime(TIMER1_CURRENT_TIME(TCNT1));
			draw_sprite(face_ptr);

			show_screen();
		}

		btn0Changed = 0;
		btn1Changed = 0;
	}

	return 0;
}

void initializeTimer0(void) {
	// set to normal mode
	TCCR0B &= ~(1 << WGM02);

	// set prescaler
	TCCR0B |= (1 << CS02);

	// enable interrupt
	TIMSK0 |= (1 << TOIE0);

	// init timer count
	TCNT0 = 0;
}

ISR(TIMER0_OVF_vect) {
	++timer0Overflow;

	if (timer0Overflow >= 61) {
		timer0Overflow = 0;

		if (!pause) {
			LED0_TOGGLE;
			LED1_TOGGLE;
		}
	}
}

void initializeTimer1(void) {
	// set to normal mode
	TCCR1B &= ~(1 << WGM12);

	// set prescaler
	TCCR1B |= ((1 << CS12) | (1 << CS10));

	// enable interrupt
	// TIMSK1 |= (1 << TOIE1);

	// init timer count
	TCNT1 = 0;
}

void displayTime(float currentTime) {
	static char buffer[20];

	snprintf(buffer, 20, "%.3f", currentTime);
	draw_string(58, 0, buffer);
}
