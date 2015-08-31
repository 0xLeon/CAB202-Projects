#ifndef MEGAMANIAC_GO_DATA_H_
#define MEGAMANIAC_GO_DATA_H_

#include "cab202_timers.h"

typedef struct go_additional_data_comparable_int {
	int initial_value;
	int previous_value;
	int current_value;
} go_additional_data_comparable_int_t;
typedef go_additional_data_comparable_int_t* go_additional_data_comparable_int_p;

typedef struct go_additional_data_enemy2_mover {
	double theta;
	double dtheta;
} go_additional_data_enemy2_mover_t;
typedef go_additional_data_enemy2_mover_t* go_additional_data_enemy2_mover_p;

typedef struct go_additional_data_enemy4 {
	timer_id direction_change_timer;
	timer_id speed_change_timer;

	double next_x;
	double next_y;
	double angle;
	double speed;
} go_additional_data_enemy4_t;
typedef go_additional_data_enemy4_t* go_additional_data_enemy4_p;

typedef struct go_additional_data_trace_drawer {
	game_object_p game_object;
	int previous_screen_x;
	int previous_screen_y;
} go_additional_data_trace_drawer_t;
typedef go_additional_data_trace_drawer_t* go_additional_data_trace_drawer_p;

#endif
