/*
 * keypad.c
 *
 * Created: 22-2-2021 12:56:03
 *  Author: tycho
 */ 
#define F_CPU 32000000

#include "keypad.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*! \brief  setup of the keypad + last person out button.
*
*	\details
*	The Xmega256A3U will be in a sleep mode until one of its buttons is pressed or the door is opened.
*	<TABLE>
*	<TR><TH COLSPAN=2> Keypad smart Sensors 2021   </TH><TH> Xmegaboard-V2 </TH></TR>
*	<TR><TD> 1   </TD><TD> KP_1		</TD><TD> PD0           </TD></TR>
*	<TR><TD> 2   </TD><TD> KP_2		</TD><TD> PD1           </TD></TR>
*	<TR><TD> 3   </TD><TD> KP_3		</TD><TD> PD2           </TD></TR>
*	<TR><TD> 4   </TD><TD> KP_4		</TD><TD> PD3           </TD></TR>
*	<TR><TD> 5   </TD><TD> KP_5		</TD><TD> PD4           </TD></TR>
*	<TR><TD> 6   </TD><TD> KP_6		</TD><TD> PD5           </TD></TR>
*	<TR><TD> 7   </TD><TD> KP_7		</TD><TD> PD6           </TD></TR>
*	<TR><TD> 8   </TD><TD> KP_8		</TD><TD> PD7           </TD></TR>
*	<TR><TD> 9   </TD><TD> KP_9		</TD><TD> PE0           </TD></TR>
*	<TR><TD> 0   </TD><TD> KP_0		</TD><TD> PE1           </TD></TR>
*	<TR><TD> OK  </TD><TD> KP_OK	</TD><TD> PE2           </TD></TR>
*	<TR><TD> RES </TD><TD> KP_RESET	</TD><TD> PE3           </TD></TR>
*	<TR><TD> LPO </TD><TD> LP_OUT	</TD><TD> PA1           </TD></TR>
*	</TABLE>
*
*	<TABLE>
*	<TR><TH COLSPAN=2> HvA module LED connections</TH></TR>
*	<TR><TH> LED NAME </TH><TH> on PIN </TH></TR>
*	<TR><TD> RED </TD><TD> PC0 </TD></TR>
*	<TR><TD> GREEN </TD><TD> PF0 </TD></TR>
*	<TR><TD> BLUE </TD><TD> PF1 </TD></TR>
*	</TABLE>
*/

// this is the password for the keypad!
uint8_t password[password_length] = {1, 1, 1, 1, 1};

void init_keypad(void){
	
	//set pin PA1 as input 
	PORTA.DIRCLR = PIN1_bm;
	
	//set pin PB0 as output
	PORTB.DIRSET = PIN0_bm;
	
	//set port D as input
	PORTD.DIRCLR = 0xFF;
	
	//set E0, E1, E2, E3 as inputs
	PORTE.DIRCLR = 0x0F;
	
	//for port A
	//configure input sense on falling edge
	PORTA.PIN1CTRL = PORT_ISC_BOTHEDGES_gc;
	
	//for port D 
	//use internal pull ups
	//configure input sense on falling edge
	PORTD.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTD.PIN1CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTD.PIN2CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTD.PIN3CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTD.PIN4CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTD.PIN5CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTD.PIN7CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	
	//for port E
	//use internal pull ups
	//configure input sense on falling edge
	PORTE.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTE.PIN1CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTE.PIN2CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTE.PIN3CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTE.PIN4CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTE.PIN5CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTE.PIN6CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTE.PIN7CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	
	//configure PA1 interrupt 0
	PORTA.INT0MASK = PIN1_bm;
	PORTA.INTCTRL = PORT_INT0LVL_LO_gc;
	
	//configure port D interrupt 0
	PORTD.INT0MASK = 0xFF;
	PORTD.INTCTRL = PORT_INT0LVL_MED_gc;
	
	//configure port E interrupt 0
	PORTE.INT0MASK = 0x0F;
	PORTE.INTCTRL = PORT_INT0LVL_MED_gc;
}

void init_timer_F0(void){
	PORTF.DIRSET = PIN0_bm|PIN1_bm;
	TCF0.PER = 9999;
	TCF0.CTRLA = TC_CLKSEL_DIV8_gc;
	TCF0.CTRLB = TC0_CCAEN_bm | TC0_CCBEN_bm | TC_WGMODE_SS_gc;
	TCF0.CCA = 0;
	TCF0.CCB = 0;
	TCF0.CCC = 10;
}

void init_timer_D0(void){
	TCD0.CTRLA = TC_CLKSEL_OFF_gc;
	TCD0.INTCTRLA = TC_OVFINTLVL_LO_gc;
	TCD0.CTRLB = TC_WGMODE_SS_gc;
	TCD0.PER = 65535;  // 2^16 - 1
}

void init_LED(void){
	init_timer_D0();
	init_timer_F0();
}

uint8_t what_key_PD(void){
	_delay_us(500);
	uint8_t in_PD = PORTD.IN;
	
	switch(in_PD){
		case 0xFE:
			return 1;
			break;
			
		case 0xFD:
			return 2;
			break;
		
		case 0xFB:
			return 3;
			break;
		
		case 0xF7:
			return 4;
			break;
		
		case 0xEF:
			return 5;
			break;
		
		case 0xDF:
			return 6;
			break;
		
		case 0xBF:
			return 7;
			break;
		
		case 0x7F:
			return 8;
			break;
			
		default:
			return fault;
			break;
	}
}

uint8_t what_key_PE(void){
	_delay_us(500);
	uint8_t in_PE = PORTE.IN;
	
	switch(in_PE){
		case 0xFE: //PE0
		return 9;
		break;
		
		case 0xFD: //PE1
		return 0;
		break;
		
		case 0xFB: //PE2
		return ok;
		break;
		
		case 0xF7: //PE3
		return reset;
		break;
		
		default:
		return fault;
		break;
	}
}

//uint8_t password_check(uint8_t key){
	//if (key == ok)
	//{
		//green_on;
		//_delay_ms(500);
		//green_off;
	//} else {
		//red_on;
		//_delay_ms(500);
		//red_off;
	//}
	//return 200;
//}

uint8_t password_check(uint8_t key){
	static uint8_t password_compare[password_length];// = {1,1,1,1,1};
	static uint8_t n;
	
	// check the password if the ok key is pressed
	if (key == ok){
		for (uint8_t i = 0; i < 5; i++)
		{
			green_on;
			_delay_ms(20);
			green_off;
			_delay_ms(20);
		}
		_delay_ms(500);
		//if (!(n == (password_length))) return 0;
		if (n >= password_length)
		{
			n = 0;
			for (uint8_t i = 0; i < 5; i++)
			{
				red_on;
				_delay_ms(200);
				red_off;
				_delay_ms(200);
			}
				
			return 0;
		}
		
		for (uint8_t i = 0; i < password_length; i++)
		{
			if (!(password[i] == password_compare[i])) 
			{
				return 0;
			}
		}
		for (uint8_t i = 0; i < 5; i++)
		{
			green_on;
			_delay_ms(200);
			green_off;
			_delay_ms(200);
		}
		n = 0;
		return 1;
	}
	
	// reset the password if the reset key is pressed
	if (key == reset)
	{
		n = 0;
		return 0;
	}
	
	if (n == password_length)
	{
		n = 0;
		//for (uint8_t i = 0; i < 5; i++)
		//{
			//red_on;
			//green_on;
			//_delay_ms(200);
			//red_off;
			//green_off;
			//_delay_ms(200);
		//}
		//return 0;
	}
	
	// add number in password checker
	password_compare[n] = key;
	n++;
	for (uint8_t i = 0; i < 2; i++)
	{
		green_on;
		_delay_ms(200);
		green_off;
		_delay_ms(200);
	}
	return 2;
}

void open_door(uint8_t val){
	if (val == 1) {
		// unlock door
		PORTB.OUTSET = PIN0_bm;
		// green led on
		green_on;
		// start timer
		TCD0.CTRLA = TC_CLKSEL_DIV1024_gc;
		
	} else if (val == 0) {
		asm("nop");
	} else {
		// red led on
		red_on;
		// start timer
		TCD0.CTRLA = TC_CLKSEL_DIV1024_gc;
	}
}