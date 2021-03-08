/*
 * sleep.h
 *
 * Created: 8-3-2021 11:03:44
 *  Author: tycho
 */ 
#include <avr/sleep.h>

#ifndef SLEEP_H_
#define SLEEP_H_

#define SLEEP_ENABLE SLEEP.CTRL = sleep_enable;
#define SLEEP_DISABLE SLEEP.CTRL = sleep_disable;

void init_sleep(void);


#endif /* SLEEP_H_ */