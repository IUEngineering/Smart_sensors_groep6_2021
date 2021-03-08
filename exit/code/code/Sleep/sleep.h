/*
 * sleep.h
 *
 * Created: 8-3-2021 11:03:44
 *  Author: tycho
 */ 

#ifndef SLEEP_H_
#define SLEEP_H_

#define SLEEP_ENABLE SLEEP.CTRL = 1;
#define SLEEP_DISABLE SLEEP.CTRL = 0;

void init_sleep(void);


#endif /* SLEEP_H_ */