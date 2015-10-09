/* File: simple_adc.c
 * Description: C program for the ATMEL AVR microcontroller (ATmega32 chip)
 * read potentiometers and convert values into decimal
 * turn and LED on when values are lower than threshold
 * also display potentiometer value on the screen
 *
 *
 * Teensy Version 1 & 2
 */


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
 
#include "lcd.h"
#include "cpu_speed.h"
#include "sprite.h"
#include "graphics.h"

 
#define LTHRES 500
#define RTHRES 500
 
// initialize adc
void adc_init()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);
 
    // ADC Enable and pre-scaler of 128
    // 8000000/128 = 62500
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
 
// read adc value
uint16_t adc_read(uint8_t ch)
{
    // select the corresponding channel 0~7
    // ANDing with '7' will always keep the value
    // of 'ch' between 0 and 7
    ch &= 0b00000111;  // AND operation with 7
    ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing
 
    // start single conversion
    // write '1' to ADSC
    ADCSRA |= (1<<ADSC);
 
    // wait for conversion to complete
    // ADSC becomes '0' again
    // till then, run loop continuously
    while(ADCSRA & (1<<ADSC));
 
    return (ADC);
}
 
int main()
{
    uint16_t adc_result0, adc_result1;
    char int_buffer[10];
    char disp_buffer_r[32];
    char disp_buffer_l[32];
    
    DDRD |= (1<<PIND6);//|(1<<PINB3);	//Teensy LEDLED0 and LED1 as outputs
	PORTD = 0x00; 	// Turn everything off to start with
    
    
    //set clock speed to 8MhZ
    set_clock_speed(CPU_8MHz);
    
 
    // initialize adc and lcd
    adc_init();
    LCDInitialise(LCD_DEFAULT_CONTRAST);

    _delay_ms(50);
 
    while(1)
    {
        adc_result0 = adc_read(0);      // read adc value at PA0
        adc_result1 = adc_read(1);      // read adc value at PA1
 
 
 	  // condition for led to glow
        if (adc_result0 < LTHRES)
            PORTD |= 1<<PIND6;
        else
            PORTD = 0x00;
            
        //clear pixels
 		clear_screen();
 		            
 		// write the labels on an array
 		itoa(adc_result0, int_buffer, 10);
 		sprintf(disp_buffer_l,"left  ADC = %s",int_buffer);
 		
 		// write the labels on an array
 		itoa(adc_result1, int_buffer, 10);
 		sprintf(disp_buffer_r,"right  ADC = %s",int_buffer);
 
 		 // display the labels on the LCD
 		draw_string(0,7,disp_buffer_l); 		
        draw_string(0,35,disp_buffer_r);
        
        
        // write pixel values
        show_screen();
        
      
    }
}