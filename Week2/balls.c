#include "cab202_graphics.h"

void draw_balls() {
	draw_char(63, 22, 'O');
	draw_char(43, 19, 'O');
	draw_char(73, 1, 'O');
	draw_char(22, 20, 'O');
	
	show_screen();
}

int main() {
	setup_screen();
	
	draw_balls();
	
	draw_string(0, screen_height() - 1, "Press any key to finish...");
	wait_char();
	cleanup_screen();
	
	return 0;
}