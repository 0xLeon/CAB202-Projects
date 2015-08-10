#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cab202_timers.h"
#include "cab202_graphics.h"

void draw_rect(int x, int y, int width, int height, char c) {
	if ((width < 1) || (height < 1)) {
		return;
	}
	
	int x2 = x + width - 1;
	int y2 = y + height - 1;
	
	draw_line(x, y, x2, y, c);		// top
	draw_line(x, y2, x2, y2, c);		// bottom
	draw_line(x, y, x, y2, c);		// left
	draw_line(x2, y, x2, y2, c);		// right
}

void demo() {
	int x = rand() % screen_width() / 2;
	int y = rand() % screen_height() / 2;
	int w = rand() % (screen_width() - x);
	int h = rand() % (screen_height() - y);
	char* chars = "~!@#$%^&*-+";
	char ch = chars[rand() % strlen(chars)];

	draw_rect(x, y, w, h, ch);

	show_screen();
}

int main() {
	setup_screen();
	srand(time(NULL));
	demo();
	timer_pause(5000);
	cleanup_screen();
	
	return 0;
}
