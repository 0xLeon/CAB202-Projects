#ifndef ANIMATION_H_
#define ANIMATION_H_

typedef struct vertex {
	int x;
	int y;
} vertex_t;

void walk_waypoints(vertex_t* vertices, int vertices_c, bool return_start, char c);

void walk_waypoints_interpolate(vertex_t a, vertex_t b, char c);

void walk_waypoints_draw_avatar(int x, int y, char c);

#endif
