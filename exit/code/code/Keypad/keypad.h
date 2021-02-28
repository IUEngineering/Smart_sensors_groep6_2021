/*
 * keypad.h
 *
 * Created: 28-2-2021 15:21:47
 *  Author: tycho
 */ 

#include <stdint.h>

#ifndef KEYPAD_H_
#define KEYPAD_H_

#define password_length 5
#define ok 10
#define reset 11 
#define fault 13

#define green_on TCF0.CCABUF = 250
#define green_off TCF0.CCABUF = 0

#define red_on TCF0.CCBBUF = 250
#define red_off TCF0.CCBBUF = 0


void init_keypad(void);
void init_timer_F0(void);
void init_timer_D0(void);
void init_LED(void);
uint8_t what_key_PD(void);
uint8_t what_key_PE(void);
uint8_t password_check(uint8_t key);
void open_door(uint8_t val);
#endif /* KEYPAD_H_ */