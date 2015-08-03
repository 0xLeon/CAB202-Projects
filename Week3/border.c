#include "cab202_graphics.h"

void draw_rect(int x1, int y1, int x2, int y2, char c) {
	draw_line(x1, y1, x2, y1, c);		// top
	draw_line(x1, y2, x2, y2, c);		// bottom
	draw_line(x1, y1 + 1, x1, y2 - 1, c);	// left
	draw_line(x2, y1 + 1, x2, y2 - 1, c);	// right
}

void draw_border() {
	int left = 0;
	int right = 0;
	int top = 0;
	int bottom = 3;
	
	int width;
	int height;
	
	get_screen_size(width, height);
	draw_rect(left, top, width - right - 1, height - bottom - 1, '&');
	
	show_screen();
}

int main() {
	setup_screen();
	
	draw_border();
	
	wait_char();
	cleanup_screen();
	
	return 0;
}
