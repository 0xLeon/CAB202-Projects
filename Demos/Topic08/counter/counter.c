
/*
 *   File: counter.c
 *   Description: C program for the ATMEL AVR microcontroller (ATmega32 chip)
 *   Tries to emulate a countdown timer
 *
 *
 *  Includes (pretty much compulsory for using the Teensy this semester)
 * 	- avr/io.h: port and pin definitions (i.e. DDRB, PORTB, PB1, etc)
 *	- util/delay.h: '_delay_ms()' function
 *	- cpu_speed.h: macros for forcing the CPU speed to 8MHz (nothing else should ever be used!)
 *  - graphics.h: library to interact with the LCD
 *  - stdio.h: used to include functions for std input and std output.
 */


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "graphics.h"
#include "cpu_speed.h"


//Global variable for the clock system
 int   clock_millisecond=0;

 int        clock_second=0;
 int        clock_minute=0;
 int        clock_hour=0;

//wirte the main file
int main(void) {
    
    // Set the CPU speed to 8MHz (you must also be compiling at 8MHz)
    set_clock_speed(CPU_8MHz);
    
    //let's use switch 1 and 2
    // in a single operation I set some of the pins as outputs and other as inputs
    // B0=1, B1=1, B2=0
    DDRB = 0b00000011;
    
    // Turn OFF LED initially
    // Output a 0 to B2
    PORTB = 0b00000000;
    
    
    //counter for the number of loops
    int counter = 0;
    
    //used to store characters to draw on the screen
    char buffer[32];
    
    
    
    //initialise the LCD screen
    LCDInitialise(LCD_DEFAULT_CONTRAST);
    
    //clear any characters that were written on the screen
    clear_screen();
    
    //fill a buffer (array) of charactes with the string "  ", in the position x,y
    draw_string(4,5, "Welcome CAB202");
    draw_string(12,19,"Timer Demo!");
    
    //write the string on the lcd
    show_screen();
    
    //used to increase/decrease the speed of the counter
    int prescaler = 40;
    
    
    while (1){
  
        //check the switch 0
        if(PINB & 0b00000001){
            prescaler--;// = prescaler - 1;
            
        }
        
        //avoid this variable from being zero
        if (prescaler < 1) {
            prescaler = 1;
        }
        
        // Read from Port B to check input - SW1
        // If the correct pin connected to the button is a 1
        // Then Send output to PORTB LED Pin. OFF
        if(PINB & 0b0000010){
            counter=clock_minute=clock_second=clock_hour=0;
            prescaler=40;
            
        }
        
        
       //increase seconds every time counter increases by 40
       if (counter%prescaler == (prescaler-1)){
            clock_second++;
        }
        
        //once seconds are 59, increase minutes
        if (clock_second > 59) {
            clock_second=0;
            clock_minute++;
        }
        
        //once minutes are 59, increase hours
        if (clock_minute > 59) {
            clock_minute=0;
            clock_hour++;
        }
    
        //fill buffer with characters
        sprintf(buffer, "%2.2d:%2.2d:%2.2d", clock_hour, clock_minute, clock_second);
        
        //draw the characters on the screen
        draw_string(20,30,buffer);
        
        //refresh pixels
        show_screen();
        
        //increase loop counter
     counter++;
        
    }
        
   
	return 0;
}



