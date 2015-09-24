#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "sprite.h"
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"

void Init(void);
void PinChangeInit(void);

//GLOBAL VARIABLES, volatile because I would like to share these bewteen my interrupt routine and main program.
volatile int heroX = LCD_X/2;
volatile int heroY = LCD_Y-20;
volatile int buttonPressed = 0;

int previousButton = 0;

int main(void)
{
    //set clock speed to 8MhZ
    set_clock_speed(CPU_8MHz);
    
    
    //call initialisation routines
	Init();	
	PinChangeInit();

	clear_screen();
	draw_string(10,10,"Interrupt Demo");
	show_screen();

    //stops for 2 secs
    _delay_ms(2000);
    
    //do nothing. stop main program from exiting
    while(1);
	
	
	return 0;
}


//setup and initialise ports and LCD
void Init()
{
	DDRB |= (1<<PINB2)|(1<<PINB3);	//LED0 and LED1 as outputs
	DDRB &= ~((1<<PINB0)|(1<<PINB1));	//Switch0 and Switch1 as input
	PORTB = 0x00; 	// Turn everything off to start with
	LCDInitialise( LCD_DEFAULT_CONTRAST );
	clear_screen();
	show_screen();
}


//setup and initialise interrupts
void PinChangeInit(void)
{
	//Enable PCINT0 and PCINT1 (both buttons) in the PCMSK0 register
	PCMSK0 |= (1<<PCINT0);
	PCMSK0 |= (1<<PCINT1);

	//Enable Pin change interrupts in the Pin Change Interrupt Control Register
	PCICR |= (1<<PCIE0);
	
	//External interrups. Set the PCINT0 and PCINT1 interrupts to trigger on rising edge
	EICRA |= ((1<<ISC00)&&(1<<ISC01)&&(1<<ISC10)&&(1<<ISC11));
		
	//Ensure to enable global interrupts as well.
	sei();
}

//implements main code to be execute when interrupt is called
ISR(PCINT0_vect)
{
    //check pin change. in this case pin 0
	if ((PINB>>PINB0)&1){
		heroX = heroX-3;
	} else if ((PINB>>PINB1)&1){ //check state pin 1
		heroX = heroX+3;
	}
	clear_screen();
	draw_string(heroX,heroY,"H");
	show_screen();
}
