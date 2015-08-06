#include <stdbool.h>
#include "cab202_graphics.h"
#include "cab202_timers.h"
#include "animation.h"

#define ABS(x)		(((x) >= 0) ? (x) : -(x))
#define SIGN(x)		((x > 0) - (x < 0))
#define WAIT_TIME	100L

int main() {
	vertex_t waypoints[] = {
		{35, 15},
		{45, 5},
		{5, 22},
		{17, 2},
	};
	
	setup_screen();
	walk_waypoints(waypoints, 4, true, '+');
	wait_char();
	cleanup_screen();
	
	return 0;
}

void walk_waypoints(vertex_t* vertices, int vertices_c, bool return_start, char c) {
	if (vertices_c < 1) {
		return;
	}
	else if (vertices_c == 1) {
		draw_char(vertices[0].x, vertices[0].y, c);
		show_screen();
		
		return;
	}
	else {
		for (int i = 0; i < (vertices_c - 1); i++) {
			walk_waypoints_interpolate(vertices[i], vertices[i + 1], c);
		}
		
		if (return_start && (vertices_c > 2)) {
			walk_waypoints_interpolate(vertices[vertices_c - 1], vertices[0], c);
		}
	}
}

void walk_waypoints_interpolate(vertex_t a, vertex_t b, char c) {
	if (a.x == b.x) {
		for (int i = a.y; ((b.y > a.y) ? (i <= b.y) : (i >= b.y)); (b.y > a.y) ? i++ : i--) {
			walk_waypoints_draw_avatar(a.x, i, c);
		}
	}
	else if (a.y == b.y) {
		for (int i = a.x; ((b.x > a.x) ? (i <= b.x) : (i >= b.x)); (b.x > a.x) ? i++ : i--) {
			walk_waypoints_draw_avatar(i, a.y, c);
		}
	}
	else {
		float dx = b.x - a.x;
		float dy = b.y - a.y;
		float err = 0.0;
		float derr = ABS(dy / dx);

		for (int x = a.x, y = a.y; (dx > 0) ? x <= b.x : x >= b.x; (dx > 0) ? x++ : x--) {
			walk_waypoints_draw_avatar(x, y, c);
			err += derr;
			
			while (err >= 0.5 && ((dy > 0) ? y <= b.y : y >= b.y)) {
				walk_waypoints_draw_avatar(x, y, c);
				
				y += SIGN(dy);

				err -= 1.0;
			}
		}
	}
}

void walk_waypoints_draw_avatar(int x, int y, char c) {
	//clear_screen();
	draw_char(x, y, c);
	show_screen();
	timer_pause(WAIT_TIME);
}
