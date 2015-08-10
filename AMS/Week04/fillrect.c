#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cab202_timers.h"
#include "cab202_graphics.h"

void fill_rect(int x, int y, int width, int height, char c) {
	if ((width < 1) || (height < 1)) {
		return;
	}
	
	int x2 = x + width - 1;
	int y2 = y + height;
	
	for (; y < y2; y++) {
		draw_line(x, y, x2, y, c);
	}
}

void demo() {
	int x = rand() % screen_width() / 2;
	int y = rand() % screen_height() / 2;
	int w = rand() % (screen_width() - x);
	int h = rand() % (screen_height() - y);
	char* chars = "~!@#$%^&*-+";
	char ch = chars[rand() % strlen(chars)];

	fill_rect(x, y, w, h, ch);
	fill_rect(1, 1, 1, 1, '#');

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
