/*
* Graphics.c
*
* Base-level curses-based graphics library for CAB202, 2015 semester 2.
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cab202_graphics.h"
#include "curses.h"

#define ABS(x)	(((x) >= 0) ? (x) : -(x))
#define SIGN(x)	(((x) > 0) - ((x) < 0))

bool auto_save_screen = false;

typedef struct Screen {
	int width;
	int height;
	char * buffer;
} Screen;

Screen * override_screen = NULL;

/**
*	Set up the terminal display for curses-based graphics.
*/
void setup_screen( void ) {
	// Enter curses mode.
	initscr();

	// Do not echo keypresses.
	noecho();

	// Turn off the cursor.
	curs_set( 0 );

	// Cause getch to return ERR if no key pressed within 0 milliseconds.
	timeout( 0 );

	// Enable the keypad.
	keypad( stdscr, TRUE );

	// Erase any previous content that may be lingering in this screen.
	clear();
}

/**
*	Restore the terminal to its normal operational state.
*/
void cleanup_screen( void ) {
	// cleanup curses.
	endwin();

	// cleanup after ourselves.
	if ( override_screen != NULL ) {
		free( override_screen->buffer );
		free( override_screen );
		override_screen = NULL;
	}
}

/**
*	Clear the terminal window.
*/
void clear_screen( void ) {
	// Clear the curses screen.
	clear();

	// Erase the contents of the current window.
	if ( override_screen != NULL ) {
		int w = override_screen->width;
		int h = override_screen->height;
		char * scr = override_screen->buffer;
		memset( scr, ' ', w * h );
	}
}

/**
*	Make the current contents of the window visible.
*/
void show_screen( void ) {
	// Save a screen shot, if automatic saves are enabled. 
	if ( auto_save_screen ) {
		save_screen();
	}

	// Force an update of the curses display.
	refresh();
}

/**
*	Draws the specified character at the prescibed location (x,y) on the window.
*/
void draw_char( int x, int y, char value ) {
	// Always attempt to display the character, regardless of the size of the overridden screen.
	mvaddch( y, x, value );

	// Update the overridden screen as well.
	if ( override_screen != NULL ) {
		int w = override_screen->width;
		int h = override_screen->height;

		if ( x >= 0 && x < w && y >= 0 && y < h ) {
			char * scr = override_screen->buffer;
			scr[x + y * w] = value;
		}
	}
}

void draw_line( int x1, int y1, int x2, int y2, char value ) {
#if 0
	if ( x1 == x2 ) {
		//Draw vertical line
		unsigned char i;
		for ( i = y1; i <= y2; i++ ) {
			draw_char( x1, i, value );
		}
	}
	else if ( y1 == y2 ) {
		//Draw horizontal line
		unsigned char i;
		for ( i = x1; i <= x2; i++ ) {
			draw_char( i, y1, value );
		}
	}
	else {
		// Figure out octant
		unsigned int oct;
		float g = ( (float)y2 - y1 ) / ( (float)x2 - x1 );

		if ( x2 > x1 ) {
			if ( g > 1 ) {
				oct = 1;
			}
			else if ( g <= 1 && g > 0 ) {
				oct = 0;
			}
			else if ( g <= 0 && g > -1 ) {
				oct = 7;
			}
			else {
				oct = 6;
			}
		}
		else {
			if ( g > 1 ) {
				oct = 5;
			}
			else if ( g <= 1 && g > 0 ) {
				oct = 4;
			}
			else if ( g <= 0 && g > -1 ) {
				oct = 3;
			}
			else {
				oct = 2;
			}
		}

		// Translate octants to settings
		unsigned int useX;
		switch ( oct ) {
		case 0:
		case 3:
		case 4:
		case 7:
			useX = 1;
			break;
		case 1:
		case 2:
		case 5:
		case 6:
			useX = 0;
			break;

		}

		unsigned int i1, i, i2, c;
		float dx = (float)x2 - x1, dy = (float)y2 - y1, m;

		if ( useX ) {
			m = dy / dx;
			c = y1; i1 = x1; i2 = x2;
		}
		else {
			m = dx / dy;
			c = x1; i1 = y1; i2 = y2;
		}

		int flipM;

		switch ( oct ) {
		case 0:
		case 1:
		case 2:
		case 7:
			flipM = 1;
			break;
		case 3:
		case 4:
		case 5:
		case 6:
			flipM = -1;
			break;
		}

		// Perform the actual loop
		float err = 0.0;

		for ( i = i1; ( i2 > i1 ) ? i <= i2 : i >= i2; ( i2 > i1 ) ? i++ : i-- ) {
			( useX ) ? draw_char( i, c, value ) : draw_char( c, i, value );
			err += ABS( m );

			if ( err > 0.5f ) {
				( SIGN( m*flipM ) > 0 ) ? c++ : c--;
				err -= 1.0f;
			}
		}
	}
#else
	if ( x1 == x2 ) {
		// Draw vertical line
		for ( int i = y1; ( y2 > y1 ) ? i <= y2 : i >= y2; ( y2 > y1 ) ? i++ : i-- ) {
			draw_char( x1, i, value );
		}
	}
	else if ( y1 == y2 ) {
		// Draw horizontal line
		for ( int i = x1; ( x2 > x1 ) ? i <= x2 : i >= x2; ( x2 > x1 ) ? i++ : i-- ) {
			draw_char( i, y1, value );
		}
	}
	else {
		// Get Bresenhaming...
		float dx = x2-x1;
		float dy = y2-y1;
		float err = 0.0;
		float derr = ABS( dy/dx );

		for ( int x = x1, y = y1; ( dx > 0 ) ? x <= x2 : x >= x2; ( dx > 0 ) ? x++ : x-- ) {
			draw_char( x, y, value );
			err += derr;
			while ( err >= 0.5 && ( ( dy > 0 ) ? y <= y2 : y >= y2 ) ) {
				draw_char( x, y, value );
				y += ( dy > 0 ) - ( dy < 0 );

				err -= 1.0;
			}
		}
	}
#endif
}

void draw_string( int x, int y, char * text ) {
	for ( int i = 0; text[i]; i++ ) {
		draw_char( x + i, y, text[i] );
	}
}

void draw_int( int x, int y, int value ) {
	char buffer[100];
	sprintf( buffer, "%d", value );
	draw_string( x, y, buffer );
}

void draw_double( int x, int y, double value ) {
	char buffer[100];
	sprintf( buffer, "%g", value );
	draw_string( x, y, buffer );
}

int get_char() {
	return getch();
}

int wait_char() {
	timeout( -1 );
	int result = getch();
	timeout( 0 );
	return result;
}

void get_screen_size_( int * width, int * height ) {
	*width = screen_width();
	*height = screen_height();
}

int screen_width( void ) {
	return override_screen == NULL ? getmaxx( stdscr ) : override_screen->width;
}

int screen_height( void ) {
	return override_screen == NULL ? getmaxy( stdscr ) : override_screen->height;
}

/**
 *	Gets the character at the designated location on the screen.
 *	This uses the override screen if it is non-NULL, or otherwise
 *	uses the current curses terminal.
 */

char get_screen_char( int x, int y ) {
	if ( override_screen == NULL ) {
		return mvinch( y, x ) & 0xff;
	}

	int w = override_screen->width;
	int h = override_screen->height;

	if ( x >= 0 && x < w && y >= 0 && y < h ) {
		char * scr = override_screen->buffer;
		return scr[x + y * w];
	}
	else {
		return 0;
	}
}

/**
*	Saves a screen shot to an automatically named local file.
*/

void save_screen( void ) {
	char * fileName = CAB202_SCREEN_NAME;

	FILE * f = fopen( fileName, "a" );

	if ( f == NULL ) return;

	int width = screen_width();
	int height = screen_height();

	fprintf( f, "Frame\n" );
	fprintf( f, "width=%d\n", width );
	fprintf( f, "height=%d\n", height );

	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			char c = get_screen_char( x, y );

			if ( c != ' ' ) {
				fprintf( f, "%d,%d,%d\n", x, y, (int)c );
			}
		}
	}

	fprintf( f, "EndFrame\n" );

	fclose( f );
}

/**
*	This function is provided to support programatic emulation
*	of a resized terminal window.
*	Subsequent calls to screen_width() and screen_height() will
*	return the supplied values of width and height.
*/

void override_screen_size( int width, int height ) {
	if ( override_screen != NULL ) {
		free( override_screen->buffer );
		free( override_screen );
	}

	override_screen = calloc( 1, sizeof( Screen ) );
	override_screen->width = width;
	override_screen->height = height;
	override_screen->buffer = calloc( width * height, sizeof(char) );
	memset( override_screen->buffer, ' ', width * height );
}

/**
*	This function is provided to support programmatic emulation
*	of a resized terminal window. It undoes the effects of
*	override_screen_size.
*	Subsequent calls to screen_width() and screen_height() will
*	return the width and height of the current terminal window,
*	respectively.
*/

void use_default_screen_size( void ) {
	if ( override_screen != NULL ) {
		free( override_screen->buffer );
		free( override_screen );
		override_screen = NULL;
	}
}

/**
*	Draws formatted text at the specified location.
*	Total length of text must be less than 1000 characters.
*/

void draw_formatted( int x, int y, const char * format, ... ) {
	va_list args;
	va_start( args, format );
	char buffer[1000];
	vsprintf( buffer, format, args );
	draw_string( x, y, buffer );
}