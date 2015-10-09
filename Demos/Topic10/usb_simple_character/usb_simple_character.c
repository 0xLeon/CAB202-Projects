/* Simple example for Teensy USB Development Board
 * This program send and receives characters via usb port
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>

#include "usb_serial.h"

#include "sprite.h"
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"




//function declaration
void send_str(const char *s);
uint8_t recv_str(char *buf, uint8_t size);
void Init(void);


// Basic command interpreter for controlling port pins
int main(void)
{
	char rx_buf[8];// the buffer to store received characters
	uint8_t n = 0;  // to store the number of bytes read from the serial port
    // coordinates of the middle of the screen
    int x=LCD_X/2; 
    int y=LCD_Y/2;

	//set clock speed to 8MhZ
    set_clock_speed(CPU_8MHz);
    
    
    //initialise LCD and ports
	Init();

	// initialize the USB, and then wait for the host
	// to set configuration.  If the Teensy is powered
	// without a PC connected to the USB port, this 
	// will wait forever.
	usb_init();
	while (!usb_configured()) /* wait */ ;
	  _delay_ms(1000);


   //keep looping until the character 'q' is received
	while (rx_buf[0]!='q') {
        
		// wait for the user to run their terminal emulator program
		// which sets DTR to indicate it is ready to receive.
		while (!(usb_serial_get_control() & USB_SERIAL_DTR)) /* wait */ ;

		// discard anything that was received prior.  Sometimes the
		// operating system or other software will send a modem
		// "AT command", which can still be buffered.
		usb_serial_flush_input();


		// and then listen for commands and process them
		while (1) {
            
           
            
            if (usb_serial_available())
            {
			  n = recv_str(rx_buf, sizeof(rx_buf)); //read serial port
			   //send a character to the other side
              send_str(PSTR(">\n"));
            }
			else
			{
			  break;
			}	 
            
            clear_screen();
            draw_string(x-40,y,"receiving: ");
            show_screen();
            draw_char(x+20,y,rx_buf[0]);
            show_screen();
            
		}
		
	}	 
	return 0;
}


//initialise ports and LCD screen
void Init()
{
	DDRB |= (1<<PINB2)|(1<<PINB3);	//LED0 and LED1 as outputs
	DDRB &= ~((1<<PINB0)|(1<<PINB1));	//Switch0 and Switch1 as input
	PORTB = 0x00; 	// Turn everything off to start with
	LCDInitialise( LCD_DEFAULT_CONTRAST );
	clear_screen();
	show_screen();
}


// Send a string to the USB serial port.  The string must be in
// flash memory, using PSTR
//
void send_str(const char *s)
{
	char c;
	while (1) {
		c = pgm_read_byte(s++);
		if (!c) break;
		usb_serial_putchar(c);
	}
}

// Receive a string from the USB serial port.  The string is stored
// in the buffer and this function will not exceed the buffer size.
// A carriage return or newline completes the string, and is not
// stored into the buffer.
// The return value is the number of characters received, or -1 if
// the virtual serial connection was closed while waiting.
//
uint8_t recv_str(char *buf, uint8_t size)
{
	int16_t r;
	uint8_t count=0;

	while (count < size) {
		r = usb_serial_getchar();
		if (r != -1) {
			if (r == '\r' || r == '\n') return count;
			if (r >= ' ' && r <= '~') {
				*buf++ = r;
				count++;
			}
		} else {
			if (!usb_configured() ||
			  !(usb_serial_get_control() & USB_SERIAL_DTR)) {
				// user no longer connected
				return -1;
			}
			// just a normal timeout, keep waiting
		}
	}
	return count;
}