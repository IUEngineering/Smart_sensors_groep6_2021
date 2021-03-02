#define F_CPU 32000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "clock.h"
#include "serialF0.h"


void init_adc(void) //Initializing ADC
{
	PORTA.DIRCLR     = PIN3_bm;                                                 // PA3 = ADC input
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc | ADC_CH_MUXNEG_GND_MODE4_gc;      // Set PIN3 as positive | set GND als negative
	ADCA.CH0.CTRL    = ADC_CH_INPUTMODE_DIFFWGAIN_gc | ADC_CH_GAIN_DIV2_gc;     // GAIN * 1/2
	ADCA.REFCTRL     = ADC_REFSEL_INTVCC_gc;                                    // VCC/1.6 (VCC=3.3V)
	ADCA.CTRLB       = ADC_RESOLUTION_12BIT_gc | ADC_CONMODE_bm;                                
	ADCA.PRESCALER   = ADC_PRESCALER_DIV128_gc;									// 32M/128 is 250.000Hz ADC clock
	ADCA.CTRLA       = ADC_ENABLE_bm;
}

void init_pwm(void) //Initializing PWM 
{
	PORTE.DIRSET = PIN0_bm;                                     // PD8 = PWM output       
	TCE0.CTRLB   = TC0_CCAEN_bm | TC_WGMODE_SINGLESLOPE_gc;     //Singelsope PWM signal
	TCE0.CTRLA   = TC_CLKSEL_DIV8_gc;                           // 32M/8 = 400.000
	TCE0.PER     = 9999;                                        // 400K/(9999+1) = 400Hz PWM freq
	TCE0.CCA     = 0;                                           // Duty-cycle starts at 0 
}

int16_t read_adc(void) //Reading the ADC                                     
{
	int16_t res;                                             

	ADCA.CH0.CTRL |= ADC_CH_START_bm;
	while ( !(ADCA.CH0.INTFLAGS & ADC_CH_CHIF_bm) ) ;
	res = ADCA.CH0.RES;
	ADCA.CH0.INTFLAGS |= ADC_CH_CHIF_bm;

	return res;
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
	double pwm;

	while (1) {
		
		res = read_adc();     //Max_value = 1680
		pwm = res * 5.95;     //MAX_value * 5.95 = 9996 | Max_value = 99.9% duty-cycle PWM
		
		if (res < 0){
			res = 0;
		}
			if (pwm < 0){
				pwm = 0;
			} 
			
		if(bit_is_clear(PORTE.IN, 0)){
		TCE0.CCABUF = (pwm);     //Duty-cycle control | if pwm=0 than duty-cycle=0%, if pwm=9999 than duty-cycle=100%
		}
		 
		printf("res: %d  pwm: %.1f \n",  res, pwm);
		_delay_ms(200);
	}
}