

/* File: debouncingv2.c
 * Description: C program for the ATMEL AVR microcontroller (ATmega32 chip)
 * Toggle  ON/OFF LED in the board
 
 * Teensy Version 1
 
 * Includes (pretty much compulsory for using the Teensy this semester)
 * 	- avr/io.h: port and pin definitions (i.e. DDRB, PORTB, PB1, etc)
 *
 *	- cpu_speed.h: macros for forcing the CPU speed to 8MHz (nothing else should ever be used!)
 */

#include <avr/io.h> 
#include <util/delay.h> 
#include <stdio.h>


#include "sprite.h"
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"

void Init(void);
static inline void debounce(void);

// Variable to tell main that the button is pressed (and debounced).
// Main will clear it after a detected button press.
volatile uint8_t button_down;


int main(void) {
    

    
    //set clock speed to 8MhZ
    set_clock_speed(CPU_8MHz);

    //initialise ports and LCD screen
	Init();	

 
    //initial mesasge
    clear_screen();
    draw_string(4,4,"Debouncing v2");
    show_screen();
    
    //stop for 2 sec
    _delay_ms(2000);
    
    
    while (1)
    {
        debounce();
        
        if(button_down){
        
              //reset variable
                button_down = 0;
            
            
                //toggle the output between LED 0 and LED 1
                PORTB ^= 1 << PINB3;
                PORTB ^= 1 << PINB2;
            
            
            
            //clear screen before we draw on it
            clear_screen();
            
            int toggle_port_pin_2 = PORTB&0x04;
            int toggle_port_pin_3 = PORTB&0x08;
            
            //check whether pin 2 or 3 are active
            if(toggle_port_pin_2 == 4){
                //draw the characters on the screen
                draw_string(10,20,"LED0");
            }
            if(toggle_port_pin_3 == 8){
                //draw the characters on the screen
                draw_string(60,20,"LED1");
            }
            
            show_screen();
        }
        _delay_ms(10);
        
    }
    

	return 0;
}

void Init(){
	
    DDRB |= 1 << PINB3; //PIN 3 PORTB for LED1 = output
    PORTB ^= 1 << PINB3; // 
    DDRB |= 1 << PINB2;  // //PIN 2 PORTB for LED2 = output
    
    DDRB &= ~(1 << PINB1);// PIN 1 PORT B for Switch 1 = input
    PORTB |= 1 << PINB1; //
    
    
	LCDInitialise( LCD_DEFAULT_CONTRAST ); // initliase LCD screen
	clear_screen(); //clear screen
	
}

// Check button state and set the button_down variable if a debounced
// button down press is detected.
// Call this function about 100 times per second.
static inline void debounce(void)
{
    // Counter for number of equal states
    static uint8_t count = 0;
    // Keeps track of current (debounced) state
    static uint8_t button_state = 0;
    // Check if button is high or low for the moment
    uint8_t current_state = (~PINB & (1<<PINB1)) != 0;
    if (current_state != button_state) {
        // Button state is about to be changed, increase counter
        count++;
        if (count >= 4) {
            // The button have not bounced for four checks, change state
            button_state = current_state;
            // If the button was pressed (not released), tell main so
            if (current_state != 0) {
                button_down = 1;
            }
            count = 0;
        }
    } else {
        // Reset counter
        count = 0;
    }
}


