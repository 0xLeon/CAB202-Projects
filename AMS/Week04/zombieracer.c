#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "cab202_timers.h"
#include "cab202_graphics.h"

// ----------------------------------------------------------------------
// Declare constants

#define OUTER_PERCENT	90
#define NOMINAL_WIDTH	6
#define DEFAULT_P	10

// -----------------------------------------------------------------------
// Declare helper functions.

double p_norm(double x, double y, double p);

void get_racetrack_params( 
	int* x0, int* xr_outer, int* xr_inner, 
	int* y0, int* yr_outer, int* yr_inner, 
	double* p);

void get_start_pos(int* start_x, int* start_y);

// -----------------------------------------------------------------------
// Implement game.

bool inside_ellipse(int x, int y, int x0, int y0, int xr, int yr, double p) {
	return (p_norm((((double) x) - x0) / xr, (((double) y) - y0) / yr, p) <= 1);
}

bool on_racetrack(int x, int y) {
	int x0, y0, xr_outer, yr_outer, xr_inner, yr_inner;
	double p;
	get_racetrack_params(&x0, &xr_outer, &xr_inner, &y0, &yr_outer, &yr_inner, &p);
	
	return (inside_ellipse(x, y, x0, y0, xr_outer, yr_outer, p) && !inside_ellipse(x, y, x0, y0, xr_inner, yr_inner, p));
}

void draw_racetrack() {
	for (int y = 0, y_max = screen_height(), x_max = screen_width(); y < y_max; y++) {
		for (int x = 0; x < x_max; x++) {
			if (!on_racetrack(x, y)) {
				draw_char(x, y, '&');
			}
		}
	}
}

void draw_zombie(int x, int y, bool zombie_alive) {
	draw_char(x, y, (zombie_alive ? 'Z' : 'X'));
}

void update_view(int zombie_x, int zombie_y, bool zombie_alive) {
	clear_screen();
	draw_racetrack();
	draw_zombie(zombie_x, zombie_y, zombie_alive);
	show_screen();
}

void zombie_racer(int start_x, int start_y) {
	int x = start_x;
	int y = start_y;

	int dir = 0; // Start facing North.

	bool alive = on_racetrack(x, y);
	update_view(x, y, alive);
	int key = wait_char();

	while ((key != 'q') && (key >= 0) && alive) {
		switch (key) {
			case '1':
				dir = (dir + 7) % 8;
				break;
			case '3':
				dir = (dir + 1) % 8;
				break;
		}
		
		switch (dir) {
			case 0:
				y--;
				break;
			case 1:
				y--;
				x++;
				break;
			case 2:
				x++;
				break;
			case 3:
				x++;
				y++;
				break;
			case 4:
				y++;
				break;
			case 5:
				y++;
				x--;
				break;
			case 6:
				x--;
				break;
			case 7:
				x--;
				y--;
				break;
		}

		alive = on_racetrack(x, y);
		update_view(x, y, alive);
		timer_pause(100);
		key = wait_char();
	}
	
	timer_pause(1000);
}

// -------------------------------------------------------------------------

int main() {
	srand(time(NULL));
	setup_screen();
	
	int start_x;
	int start_y;
	
	get_start_pos(&start_x, &start_y);
	zombie_racer(start_x, start_y);
	
	cleanup_screen();
	
	return 0;
}

//--------------------------------------------------------------------------
// Helper function implementations.

void get_start_pos(int* start_x, int* start_y) {
	// Get the geometry of the race track/
	int x0, y0, xr_outer, yr_outer, xr_inner, yr_inner;
	double p;
	get_racetrack_params(&x0, &xr_outer, &xr_inner, &y0, &yr_outer, &yr_inner, &p);
	
	// Start at the left, close to the middle of the track. 
	(*start_x) = x0 - xr_outer + (xr_outer - xr_inner) / 2;
	(*start_y) = y0;
}

double p_norm(double x, double y, double p) {
	// Error if bad input.
	if (isinf(x) || isinf(y) || p == 0) {
		fprintf(stderr, "Illegal arguments in p_norm: x = %f, y = %f, p = %f\n", x, y, p);
		abort();
	}
	
	// Compute p-norm(x,y,p)
	double t1 = fabs(x);
	double t2 = fabs(y);
	
	return pow(pow(t1, p) + pow(t2, p), 1 / p);
}

void get_racetrack_params(
	int* x0, int* xr_outer, int* xr_inner,
	int* y0, int* yr_outer, int* yr_inner,
	double* p) {
	(*x0) = screen_width() / 2;
	(*xr_outer) = OUTER_PERCENT * (screen_width() / 2) / 100;
	(*xr_inner) = (*xr_outer) - NOMINAL_WIDTH;

	(*y0) = screen_height() / 2;
	(*yr_outer) = OUTER_PERCENT * (screen_height() / 2) / 100;
	(*yr_inner) = (*yr_outer) - NOMINAL_WIDTH;

	(*p) = DEFAULT_P;
}
