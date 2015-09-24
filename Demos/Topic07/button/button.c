/* File: button_solution.c
 * Description: C program for the ATMEL AVR microcontroller (ATmega32 chip)
 * Turn on Teensy LED with button

 * Teensy Version 1

 * Includes (pretty much compulsory for using the Teensy this semester)
 * 	- avr/io.h: port and pin definitions (i.e. DDRB, PORTB, PB1, etc)
 *
 *	- cpu_speed.h: macros for forcing the CPU speed to 8MHz (nothing else should ever be used!)
 */



// AVR header file for all registers/pins
#include <avr/io.h>

#include "cpu_speed.h"



int main(void){
    
    // Set the CPU speed to 8MHz (you must also be compiling at 8MHz)
    set_clock_speed(CPU_8MHz);
    
     // SW0 and SW1 are connected to pins B0 and B1
	 DDRB = 0b00000000;
    
    // Teensy LED is connected to D6
     DDRD = 0b01000000;
    
     // turn OFF LED initially
	 PORTD = 0b00000000;
	
    for (int i=0;i<100;i++){// while(1){
         // Read input from PORTB.
         // Read from Port B to check input
         if(PINB & 0b00000010 ){
			// Send output to PORTD.
			PORTD = 0b01000000;
		 }
         
        
     }
     return 0;
}
