/*
 * Code for the Exit node
 * Created: 16-2-2021 16:19:22
 * Author : Tycho Jöbsis and Nino ...
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 32000000

#include <util/delay.h>
#include "NRF/nrf24spiXM2.h"
#include "NRF/nrf24L01.h"
#include "Keypad/keypad.h"
#include "CLK/clock.h"


void init_nrf(void);


uint8_t  pipe[5] = {0x48, 0x76, 0x41, 0x30, 0x31};       //!< pipe address "HvA01"

// state 0 means that no one is home
// state 1 means that there is someone home
int state = 1;

int main(void)
{
	// init nrf
	// init keypad
	// init hall effect sensor
	// init sleep mode
	init_clock();
	init_keypad();
	init_LED();
    init_nrf();
	
	// enable global interrupts
	// enter idle mode
	
	red_on;
	_delay_ms(500);
	red_off;
	
    while (1) 
    {
		switch (state)
		{
			case 0:
				//send over NRF
				//enter idle mode
				break;
				
			case 1:
				//send over NRF
				//enter idle mode
				break;
			default:
				state = 1;
				break;
		}
	}
}

void init_nrf(void)
{
	nrfspiInit();                                        // Initialize SPI
	nrfBegin();                                          // Initialize radio module

	nrfSetRetries(NRF_SETUP_ARD_1000US_gc,               // Auto Retransmission Delay: 1000 us
	NRF_SETUP_ARC_8RETRANSMIT_gc);						 // Auto Retransmission Count: 8 retries
	nrfSetPALevel(NRF_RF_SETUP_PWR_6DBM_gc);             // Power Control: -6 dBm
	nrfSetDataRate(NRF_RF_SETUP_RF_DR_250K_gc);          // Data Rate: 250 Kbps
	nrfSetCRCLength(NRF_CONFIG_CRC_16_gc);               // CRC Check
	nrfSetChannel(54);                                   // Channel: 54
	nrfSetAutoAck(1);                                    // Auto Acknowledge on
	nrfEnableDynamicPayloads();                          // Enable Dynamic Payloads

	nrfClearInterruptBits();                             // Clear interrupt bits
	nrfFlushRx();                                        // Flush fifo's
	nrfFlushTx();

	nrfOpenWritingPipe(pipe);                            // Pipe for sending
	nrfOpenReadingPipe(0, pipe);                         // Necessary for acknowledge
}


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
*/

// ISR for the NRF
ISR(PORTF_INT0_vect){
	// nrf what happened
	// react to message
	// go back to idle mode
}

// ISR for the keypad on port D
ISR(PORTD_INT0_vect){
	cli();
	uint8_t key;
	int8_t door;
	// wake up
	// check what key is pressed
	key	= what_key_PD();
	// add key press to password check
	door = password_check(key);
	// if password check is filled compare with password
	// correct open door and show green led
	// incorrect show red led. (maybe in a later revision buz a buzzer)
	open_door(door);
	// go back to idle mode
	sei();
}

// ISR for the keypad on port E
ISR(PORTE_INT0_vect){
	cli();
	uint8_t key;
	int8_t door;
	// wake up
	// check what key is pressed
	key = what_key_PE();
	// add key press to password check
	door = password_check(key);
	// if password check is filled compare with password
	// correct open door and show green led
	// incorrect show red led. (maybe in a later revision buz a buzzer)
	// go back to idle
	open_door(door);
	// go back to idle mode
	sei();
}

// ISR for last person out button
ISR(PORTA_INT0_vect){
	// make state 0
	state = 0;
}