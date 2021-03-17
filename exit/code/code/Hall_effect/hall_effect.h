/*
 * hall_effect.h
 *
 * Created: 17-3-2021 11:29:22
 *  Author: tycho
 */ 

#include <avr/io.h>

#ifndef HALL_EFFECT_H_
#define HALL_EFFECT_H_

#define DOOR_CLOSED	0
#define DOOR_OPEN	1

void init_hall_effect(void);


#endif /* HALL_EFFECT_H_ */