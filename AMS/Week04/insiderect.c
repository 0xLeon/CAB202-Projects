#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "cab202_timers.h"
#include "cab202_graphics.h"


bool inside_rect(int x, int y, int left, int top, int width, int height) {
	if ((width < 1) || (height < 1)) {
		return false;
	}
	
	return ((x >= left) && (x < (left + width))) && ((y >= top) && (y < (top + height)));
}

void draw_formatted(int x, int y, const char * format, ...);

void demo() {
	const int X_MAX = screen_width();
	const int Y_MAX = screen_height();
	const int LEFT = 11;
	const int MARGIN = 5;

	// Generate coordinates, draw box.
	int left = LEFT + MARGIN + rand() % (X_MAX - LEFT - 2 * MARGIN);
	int top = MARGIN + rand() % (Y_MAX - 2 * MARGIN);
	int width = rand() % (X_MAX - left - MARGIN);
	int height = rand() % (Y_MAX - top - MARGIN);

	draw_formatted(0, 0, "left  : %d\n", left);
	draw_formatted(0, 1, "top   : %d\n", top);
	draw_formatted(0, 2, "width : %d\n", width);
	draw_formatted(0, 3, "height: %d\n", height);

	int x = left - MARGIN;

	while (x < (left + width + MARGIN)) {
		int y = top - MARGIN;

		while (y < (top + height + MARGIN)) {
			bool inside = inside_rect(x, y, left, top, width, height);

			if (inside) {
				draw_char(x, y, '+');
			}
			else {
				draw_char(x, y, '#');
			}

			y++;
		}

		x++;
	}

	show_screen();
}

int main() {
	srand(time(NULL));
	setup_screen();
	demo();
	timer_pause(2500);
	cleanup_screen();
	
	return 0;
}

/**
 *	Draws formatted text at the specified location.
 *	Total length of text must be less than 1000 characters.
 */
void draw_formatted(int x, int y, const char* format, ...) {
	va_list args;
	va_start(args, format);
	
	char buffer[1000];
	
	vsprintf(buffer, format, args);
	draw_string( x, y, buffer );
}
