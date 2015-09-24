#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#include <stdio.h>

#include "cpu_speed.h"
#include "graphics.h"
#include "sprite.h"

#include "lcd.h"

int main() {
    //set clock speed to 8MhZ
    set_clock_speed(CPU_8MHz);
    
    //initialise LCD screen
    LCDInitialise(LCD_DEFAULT_CONTRAST);

    
    /* first part Draw a line */
    
    // Draw a fancy line wheel...
    int l = 15;
    for (int i = 0; i < 16; i++) {
        clear_screen();
        draw_string(0, 0, "Clock is ticking!");
        draw_line(42, 24, round((double) 42+l*cos(((double) i)/8*M_PI)),
            round((double) 24+l*sin(((double) i)/8*M_PI)));
        show_screen();
        _delay_ms(500);
    }

    
    
    /* second part draw a sprite */
    
    // Create a sprite
    Sprite happy;
    
    //the bitmap we want to create/draw
    unsigned char bm[] = {
        0b00000111, 0b11100000,
        0b00011000, 0b00011000,
        0b00100000, 0b00000100,
        0b01000000, 0b00000010,
        0b01011000, 0b00011010,
        0b10011000, 0b00011001,
        0b10000000, 0b00000001,
        0b10000000, 0b00000001,
        0b10010000, 0b00001001,
        0b10010000, 0b00001001,
        0b10001000, 0b00010001,
        0b01000111, 0b11100010,
        0b01000000, 0b00000010,
        0b00100000, 0b00000100,
        0b00011000, 0b00011000,
        0b00000111, 0b11100000
    };
    
    
    //initialise the sprite at a given position and with a given size
    init_sprite(&happy, 34, 16, 16, 16, bm);

    //clear pixel values
    clear_screen();
    //draw a string in the position 0,0
    draw_string(0, 0, "Good Morning!");
    //draw the sprite
    draw_sprite(&happy);
    //refresh screen
    show_screen();

    while(1);

    return 0;
}
