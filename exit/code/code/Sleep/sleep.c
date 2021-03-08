/*
 * sleep.c
 *
 * Created: 8-3-2021 11:02:28
 *  Author: tycho
 */ 

#include "sleep.h"
#include <avr/sleep.h>

void init_sleep(void){
	SLEEP.CTRL = SLEEP_SMODE_PDOWN_gc;
}