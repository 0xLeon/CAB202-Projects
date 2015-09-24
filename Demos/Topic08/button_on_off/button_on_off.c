/*
 *   File: button_on_off.c
 *   Description: C program for the ATMEL AVR microcontroller (ATmega32 chip)
 *   Turn LED ON/OFF using buttons
 *
 *
 *  Includes (pretty much compulsory for using the Teensy this semester)
 * 	- avr/io.h: port and pin definitions (i.e. DDRB, PORTB, PB1, etc)
 *	- cpu_speed.h: macros for forcing the CPU speed to 8MHz (nothing else should ever be used!)
 *
 */



// include AVR header file for all registers/pins
#include<avr/io.h>

#include "cpu_speed.h"

// main starts here
int main(void){
    
    
    // Set the CPU speed to 8MHz (you must also be compiling at 8MHz)
    set_clock_speed(CPU_8MHz);
    
	 // SW0 is connected to pin B0
	 // Set up B0 as an input.


     //SW1 is connected to pin B1
     // set up B1 as an input

	 // QUT LCD board LED is connected to B2. These are the LED at either side of the LCD screen
     // Set up B2 as an output

    // in a single operation I set some of the pins as outputs and other as inputs
    // B0=1, B1=1, B2=0
     DDRB = 0b00000011;

     // Turn OFF LED initially
	 // Output a 0 to B2
     PORTB = 0b00000000;

	
     // Check for a button press forever
    while(1){
    // Read from Port B to check input - SW0
			// If the correct pin connected to the button is a 1 
				// Then Send output to PORTB LED Pin. ON
        if(PINB & 0b00000001){
            PORTB = 0b00000100;
            
        }

        // Read from Port B to check input - SW1
            // If the correct pin connected to the button is a 1
            // Then Send output to PORTB LED Pin. OFF
        if(PINB & 0b0000010){
            PORTB = 0b00000000;
        
        }
        
    }

}
// end main
