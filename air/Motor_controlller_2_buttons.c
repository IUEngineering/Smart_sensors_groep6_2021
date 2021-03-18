#define  F_CPU 32000000UL

#include <avr/io.h>
#include <util/delay.h>


#define LOCK_PERIOD_MS       2000

int button_open(void)
{
	if ( bit_is_clear(PORTD.IN,PIN3_bp) ) {
		if ( bit_is_clear(PORTD.IN,PIN3_bp) ) 
		return 1;
	}

	return 0;
}

int button_close(void)
{
	if ( bit_is_clear(PORTD.IN,PIN2_bp) ) {
		if ( bit_is_clear(PORTD.IN,PIN2_bp) ) 
		return 1;
	}

	return 0;
}


int main(void)
{
	PORTD.DIRCLR   = PIN3_bm;            // input pin switch D3
	PORTD.DIRCLR   = PIN2_bm;            // input pin switch D2
	PORTD.PIN3CTRL = PORT_OPC_PULLUP_gc; // enable pull up pin D3
	PORTD.PIN2CTRL = PORT_OPC_PULLUP_gc; // enable pull up pin D2
	PORTD.DIRSET   = PIN4_bm;            // output pin MOTOR-F
	PORTD.DIRSET   = PIN5_bm;            // output pin MOTOR-B

	while (1) {
		PORTD.OUTSET = PIN4_bm;          // toggles output MOTOR-F
		PORTD.OUTSET = PIN5_bm;          // toggles output MOTOR-B
		if ( button_open() ) {
			PORTD.OUTCLR = PIN4_bm;
			_delay_ms(LOCK_PERIOD_MS);       // lock input
		}
		if ( button_close() ) {
			PORTD.OUTCLR = PIN5_bm;
			_delay_ms(LOCK_PERIOD_MS);       // lock input
		}
	}
}