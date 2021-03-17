/*
 * hall_effect.c
 *
 * Created: 17-3-2021 11:29:00
 *  Author: tycho
 */ 

#include <avr/interrupt.h>
#include "../Keypad/keypad.h"

static typedef union{
	struct{
		uint8_t pin7 : 1;
		uint8_t pin6 : 1;
		uint8_t pin5 : 1;
		uint8_t pin4 : 1;
		uint8_t pin3 : 1;
		uint8_t pin2 : 1;
		uint8_t pin1 : 1;
		uint8_t pin0 : 1; 
	};	
	uint8_t PORT_INPUT_STORE;
} PORT_PINS_TYPE;

void init_hall_effect(void){
	
	//set pin PA0 as input
	PORTA.DIRCLR = PIN0_bm;
	
	//Port A0
	//configure input sense on falling edge and rising edge
	PORTA.PIN1CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	
	//configure PA0 interrupt 1
	PORTA.INT1MASK = PIN0_bm;
	PORTA.INTCTRL = PORT_INT1LVL_MED_gc;	
	
}

ISR(PORTA_INT1_vect){
	PORT_PINS_TYPE INPUTA = PORTA.IN;
	if(INPUTA.pin0 == 1){
		TCC0.PER = 200;
		blue_on;
	} else {
		TCC0.PER = 65535;
		blue_off;
	}
	
}