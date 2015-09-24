


#include <avr/io.h> 
#include <util/delay.h> 


#include "sprite.h"
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"

void Init(void);
int checkButtons(void);

//GLOBAL VARIABLES
unsigned char heroX = LCD_X/2;
unsigned char heroY = LCD_Y-7;
int button = 0;
int previousButton = 0;

int main(void) {
    
    //set clock speed to 8MhZ
    set_clock_speed(CPU_8MHz);

    //initialise ports and lcd screen
	Init();	
    
    //draw an initial character
	clear_screen();
	draw_string(heroX,heroY,"*");
	show_screen();

	while(1){
        
		button = checkButtons();
        
		if (button!=previousButton){
			if (button==1){
				heroX = heroX-3;
			} else if (button==2){
				heroY = heroY-3;
			}
		}
		previousButton = button;
		clear_screen();
		draw_string(heroX,heroY,"*");
		show_screen();
	}

	return 0;
}	

//setup  and initialise ports and LCD
void Init(){
	DDRB |= (1<<PINB2)|(1<<PINB3);	//LED0 and LED1 as outputs
	DDRB &= ~((1<<PINB0)|(1<<PINB1));	//Switch0 and Switch1 as input
	PORTB = 0x00; 	// Turn everything off to start with
	LCDInitialise( LCD_DEFAULT_CONTRAST );
	clear_screen();
	show_screen();
}


//this function checks whether a button has been pressed or not. No debouncing is implemeted
int checkButtons(){
	int buttonPressed = 0;
    
	if ((PINB>>PINB0)&1){
		buttonPressed = 1;
	}
	if ((PINB>>PINB1)&1){
		if (buttonPressed){
			buttonPressed = 3;
		} else {
			buttonPressed = 2;
		}
	}
	return buttonPressed;
}
