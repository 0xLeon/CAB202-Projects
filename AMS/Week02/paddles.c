#include <stdbool.h>
#include "cab202_graphics.h"

void draw_paddle(int screen_width, int screen_height, int offset_x, int offset_y, bool right, int size, char c) {
	int x = (right ? (screen_width - offset_x - 1) : offset_x);
	int start = ((screen_height - size) / 2) + offset_y;
	int end = start + size - 1;
	
	draw_line(x, start, x, end, c);
}

void draw_paddles() {
	int width;
	int height;
	
	get_screen_size(width, height);
	
	draw_paddle(width, height, 2, 2, false, 8, '!');
	draw_paddle(width, height, 4, 4, true, 8, '!');

	show_screen();
}

int main() {
	setup_screen();

	draw_paddles();

	draw_string(0, screen_height( ) - 1, "Press any key to finish...");
	wait_char();
	cleanup_screen();
	
	return 0;
}
