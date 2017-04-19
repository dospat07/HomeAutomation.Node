#pragma once


#include "stdint.h"


//#include "Daikin.h"
// Clock freq in kilohertz
// Defined because using F_CPU in TIMER_CONFIG_KHZ does not work.
// TIMER_CONFIG_KHZ does a calculation and using too large of a
// number causes overflow.
#define FOSC 16000

//#define TIMER_PWM_PIN        P2_4
#define PWM_BIT			     BIT4


#define TIMER_PIN_SELECT() ({    \
  P2DIR |= PWM_BIT;              \
  P2SEL &= ~PWM_BIT;             \
  P2OUT &= ~PWM_BIT;             \
})


#define TIMER_ENABLE_PWM     ({ TA1CTL = TASSEL_2 + MC_1 ;  P2SEL |= PWM_BIT; }) // SMCLK, Up mode
#define TIMER_DISABLE_PWM    ({ TA1CTL = TASSEL_2 + MC_0 ;  P2SEL &= ~PWM_BIT;  P2OUT &= ~PWM_BIT; }) // SMCLK, Stop mode

#define TIMER_CONFIG_KHZ(val) ({ \
	  TA1CCTL2 |= OUTMOD_4;	     \
	  TA1CCR0 = FOSC/val/2;     \
	})
 
class IRSender
{
public:
	IRSender();

	~IRSender();
	//void sendDaikin(void * data);

protected :
	void enableIROut(int khz);
	virtual void mark(int usec);
	virtual void space(int usec);
//
//private :
//
//	void sendDaikinPart(void * data,uint8_t position, uint8_t lenght);
//	
//	void sendDaikinByte(uint8_t byte);


};


















