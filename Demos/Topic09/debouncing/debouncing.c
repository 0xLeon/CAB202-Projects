


#include <avr/io.h> 
#include <util/delay.h> 
#include <stdio.h>


#include "sprite.h"
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"

void Init(void);
int checkButtons(void);


int main(void) {
    
    //to check button state
    int Pressed = 0;
    int Pressed_Confidence_Level = 0; //Measure button press cofidence
    int Released_Confidence_Level = 0; //Measure button release confidence

    
    //set clock speed to 8MhZ
    set_clock_speed(CPU_8MHz);

    //initialise ports and LCD screen
	Init();	

 
    //initial mesasge
    clear_screen();
    draw_string(4,4,"Debouncing");
    show_screen();
    
    //stop for 2 sec
    _delay_ms(2000);
    
    
    while (1)
    {
        
        if (checkButtons())
        {
            Pressed_Confidence_Level ++; //Increase Pressed Conficence
            Released_Confidence_Level = 0; //Reset released button confidence since there is a button press
            
         if(Pressed_Confidence_Level > 1000)
         {
             
          if (Pressed == 0)
                {
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
                    Pressed = 1;
               }
             //Zero it so a new pressed condition can be evaluated
             Pressed_Confidence_Level = 0;
         }
          
        }
        else
        {
            
            Released_Confidence_Level ++; //This works just like the pressed
            Pressed_Confidence_Level = 0; //Reset pressed button confidence since the button is released
            if (Released_Confidence_Level > 1000)
            {
                Pressed = 0;
                Released_Confidence_Level = 0;
            }
        }
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


//this function checks when the button has been pressed, but checking pin 1 in port B
int checkButtons(){
	int buttonPressed = 0;
    
    if(bit_is_clear(PINB, 1)){
		buttonPressed = 1;
	}
	
	return buttonPressed;
}

