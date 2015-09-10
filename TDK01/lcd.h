/*
 *  CAB202 Teensy Library (cab202_teensy)
 *	lcd.h
 *
 *	Michael, 32/13/2015 12:34:56 AM
 *
 */
#ifndef LCD_H_
#define LCD_H_

// What pins on PORTB did we connect D/C and RST to
#define DCPIN		5 //PORTB
#define RSTPIN		4 //PORTB

// What PORTB pins are the SPI lines on
#define DINPIN		6 // PORTB
#define SCKPIN		7 // PORTF
#define SCEPIN		7 // PORTD

// LCD Command and Data
#define LCD_C		0
#define LCD_D		1

// LCD Contrast levels, you may have to change these for your display
#define LCD_LOW_CONTRAST		0x2F
#define LCD_DEFAULT_CONTRAST	0x3F
#define LCD_HIGH_CONTRAST		0x4F

// Dimensions of the LCD Screen
#define LCD_X		84
#define LCD_Y		48

// CPU Frequency Defines
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_8MHz        0x01

// LCD General Functions
void LCDInitialise(unsigned char contrast);
void LCDWrite(unsigned char dc, unsigned char data);
void LCDClear(void);

// LCD Text functions
void LCDCharacter(unsigned char character);
void LCDString(unsigned char *characters);

// LCD Memory Functions
void LCDPosition(unsigned char x, unsigned char y);


#endif /* LCD_H_ */
