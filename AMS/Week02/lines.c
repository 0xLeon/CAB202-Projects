#include "cab202_graphics.h"

typedef struct vertex {
	int x;
	int y;
} vertex_t;

void draw_poly(vertex_t* vertices, int vertices_c) {
	if (vertices_c < 1) {
		return;
	}
	else if (vertices_c > 2) {
		draw_line(vertices[vertices_c - 1].x, vertices[vertices_c - 1].y, vertices[0].x, vertices[0].y, '*');
	}
	
	for (int i = 0; i < (vertices_c - 1); i++) {
		draw_line(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y, '*');
	}
}

void draw_lines() {
	vertex_t vertices[] = {
		{9, 12},
		{11, 4},
		{16, 3},
		{0, 16}
	};
	
	draw_poly(vertices, 4);
	
	show_screen();
}

int main() {
	setup_screen();
	
	draw_lines();
	
	draw_string( 0, screen_height() - 1, "Press any key to finish..." );
	wait_char();
	cleanup_screen();
	
	return 0;
}
