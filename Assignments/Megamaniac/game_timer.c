#include <stdlib.h>
#include <assert.h>
#include "game_timer.h"
#include "cab202_timers.h"

timer_id game_create_timer(long milliseconds) {
	assert(milliseconds > 0);

	timer_id timer = malloc(sizeof(cab202_timer_t));

	if (NULL == timer) {
		return NULL;
	}

	timer->milliseconds = milliseconds;
	reset_timer(timer);

	return timer;
}
