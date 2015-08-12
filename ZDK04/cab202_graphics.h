/*
* Graphics.h
*
* Base-level curses-based graphics library for CAB202, 2015 semester 1.
*/


#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdarg.h>
#include <stdbool.h>

/**
*	Set up the terminal display for curses-based graphics.
*/
void setup_screen( void );

/**
*	Restore the terminal to its normal operational state.
*/
void cleanup_screen( void );

/**
*	Clear the terminal window.
*/
void clear_screen( void );

/**
*	Make the current contents of the window visible.
*/
void show_screen( void );

/**
*	Draws the specified character at the prescibed location (x,y) on the window.
*
*	This supports curses attributes as well as regular 8-bit char values.
*/
void draw_char( int x, int y, char value );

/**
*	Draws a string at the specified location.
*/
void draw_string( int x, int y, char * text );

/**
*	Draws an integer value at the specified location.
*/
void draw_int( int x, int y, int value );

/**
*	Draws a floating point value at the specified location.
*/
void draw_double( int x, int y, double value );

/**
*	Draws a line from (x1,y1) to (x2,y2) using the specified character.
*/
void draw_line( int x1, int y1, int x2, int y2, char value );

/**
*	Gets the current dicmensions of the screen. 
*/
void get_screen_size_( int * width, int * height );

/**
*	Gets the current dimensions of the screen.
*/
#define get_screen_size(width,height) get_screen_size_( &(width), &(height) ) 

/**
*	Returns the current width of the screen.
*/
int screen_width( void );

/**
*	Returns the current height of the screen.
*/
int screen_height( void );

/**
*	Waits for and returns the next character from the standard input stream.
*/
int wait_char( void );

/**
*	Immediately returns the next character from the standard input stream 
*	if one is available, or ERR if none is present.
*/
int get_char( void );

/**
 *	The name of the text file in which screen shots are written. 	
 */

#define CAB202_SCREEN_NAME ("zdk_screen.txt")

/**
*	Saves a screen shot to an automatically named local file.
*/

void save_screen( void );

/*
 *	Automatically save a screen shot each time if this is non-zero.
 */

extern bool auto_save_screen;

/**
 *	This function is provided to support programmatic emulation
 *	of a resized terminal window.
 *	Subsequent calls to screen_width() and screen_height() will 
 *	return the supplied values of width and height.
 */

void override_screen_size( int width, int height );

/**
 *	This function is provided to support programmatic emulation
 *	of a resized terminal window. It undoes the effects of
 *	override_screen_size.
 *	Subsequent calls to screen_width() and screen_height() will 
 *	return the width and height of the current terminal window,\
 *	respectively.
 */

void use_default_screen_size( void );

/**
*	Draws formatted text at the specified location.
*	Total length of text must be less than 1000 characters.
*/

void draw_formatted( int x, int y, const char * format, ... );

#endif /* GRAPHICS_H_ */