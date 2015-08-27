#ifndef MEGAMANIAC_GO_DATA_H_
#define MEGAMANIAC_GO_DATA_H_

typedef struct go_additional_data_comparable_int {
	int previous_value;
	int current_value;
} go_additional_data_comparable_int_t;
typedef go_additional_data_comparable_int_t* go_additional_data_comparable_int_p;

typedef struct go_additional_data_enemy2_mover {
	double theta;
	double dtheta;
} go_additional_data_enemy2_mover_t;
typedef go_additional_data_enemy2_mover_t* go_additional_data_enemy2_mover_p;

#endif
