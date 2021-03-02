#define F_CPU 32000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "clock.h"
#include "serialF0.h"

#define MAX_VALUE   2047                                   // only 11 bits are used
#define VCC         3.30
#define VREF        (((double) VCC) / 1.6)

void init_adc(void)
{
	PORTA.DIRCLR     = PIN2_bm;                      
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc | ADC_CH_MUXNEG_GND_MODE3_gc;       
	ADCA.CH0.CTRL    = ADC_CH_INPUTMODE_SINGLEENDED_gc;           
	ADCA.REFCTRL     = ADC_REFSEL_INTVCC_gc;
	ADCA.CTRLB       = ADC_RESOLUTION_12BIT_gc | ADC_CONMODE_bm;                  
	ADCA.PRESCALER   = ADC_PRESCALER_DIV32_gc;
	ADCA.CTRLA       = ADC_ENABLE_bm;
}

void init_pwm(void)
{
	PORTE.DIRSET = PIN0_bm;

	
	TCE0.CTRLB   = TC0_CCAEN_bm | TC_WGMODE_SINGLESLOPE_gc;
	TCE0.CTRLA   = TC_CLKSEL_DIV8_gc;
	TCE0.PER     = 9999;
	TCE0.CCA     = 0;
}

int16_t read_adc(void)                                     
{
	int16_t res;                                             

	ADCA.CH0.CTRL |= ADC_CH_START_bm;
	while ( !(ADCA.CH0.INTFLAGS & ADC_CH_CHIF_bm) ) ;
	res = ADCA.CH0.RES;
	ADCA.CH0.INTFLAGS |= ADC_CH_CHIF_bm;

	return res;
}

ISR(PORTD_INT0_vect)
{
	PORTA.OUTTGL = PIN2_bm;
}

int main(void)
{	                                        
	init_stream(F_CPU);
	sei();
	clear_screen();
	init_clock();
	init_adc();
	init_pwm();
	
	int16_t res; 
	double vinp;  

	sei();

	while (1) {
		
		res = read_adc();
		vinp = (double) res * VREF / (MAX_VALUE + 1);    
		if(bit_is_clear(PORTA.IN, 0)){
			if (res < 0){
				res = 0;
			}
			TCE0.CCABUF = (res*4.9);
		}
		
				
 
 
		printf("res: %d  spanning: %.2f V\n",  res, vinp);
		_delay_ms(200);
	}
}