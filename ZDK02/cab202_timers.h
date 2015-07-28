/*
	cab202_timer_h: Defines portable functions for pausing execution
	and determining if a specified interval has elapsed.

	Copyright (C) Queensland University of Technology, 2015.
	*/

#ifndef __TIMER_H__
#define __TIMER_H__

/*	Constant number of milliseconds in a second. */
#define MILLISECONDS 1000

/*	Data structure to keep track of elapsed time. */
typedef struct {
	double reset_time;
	long milliseconds;
} cab202_timer_t;

/*	Data type to represent unique timer ID. */
typedef cab202_timer_t * timer_id;

/*
 *	create_timer:
 *
 *	Creates a new timer and sets it up with the required interval.
 *
 *	Input:
 *	-  milliseconds: the length of the desired interval between reset and expiry.
 *
 *	Output:
 *		Returns a non-negative ID number that can be used to check the status 
 *		of the timer if fewer than TIMER_MAX timers have been created. Otherwise, 
 *		returns TIMER_ERROR. 
 */

timer_id create_timer( long milliseconds );

/*
 *	reset_timer:
 *
 *	Reset a timer to start a new interval.
 *
 *	Input:
 *	-	timer: the address of a timer which is to be reset.
 *
 *	Output: void.
 */

void reset_timer( timer_id id );

/*
 *	timer_expired:
 *
 *	Checks a timer to see if the associated interval has passed. If the interval has
 *	elapsed, the timer is reset automatically, ready to start again.
 *
 *	Input:
 *	-	id: The ID of a timer to check and update.
 *
 *	Output:
 *		Returns TRUE if and only if the interval had elapsed.
 */

int timer_expired( timer_id timer );

/*	
 *	timer_pause:
 *
 *	Pauses execution in a system-friendly way to allow other processes to work and
 *	conserve clock cycles.
 *
 *	Input:
 *		milliseconds:	The duration of the desired pause.
 *
 *	Output: void.
 */

void timer_pause( long milliseconds );

/*
 *	get_current_time:
 *
 *	Gets an estimate of the elapsed system time.
 *
 *	Input: no input.
 *
 *	Output: Returns the current system time in measured in whole and fractional seconds.
 */

double get_current_time();

#endif
