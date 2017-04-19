#include "IRSender.h"


IRSender::IRSender()
{

}


IRSender::~IRSender()
{
}

 

void IRSender::mark(int time) {
	// Sends an IR mark for the specified number of microseconds.
	// The mark output is modulated at the PWM frequency.
	TIMER_ENABLE_PWM; // Set SMCLK, Up mode
	delayMicroseconds(time);
	TIMER_DISABLE_PWM;
	
}

/* Leave pin off for time (given in microseconds) */
void IRSender::space(int time) {
	// Sends an IR space for the specified number of microseconds.
	// A space is no output, so the PWM output is disabled.
	TIMER_DISABLE_PWM; // Set SMCLK, Stop mode
	delayMicroseconds(time);
}


void IRSender::enableIROut(int khz) {

	// Disable the Timer_A Interrupt (which is used for receiving IR)
	//TIMER_DISABLE_INTR; //Timer_A Overflow Interrupt

	TIMER_PIN_SELECT(); // P2.3 output and P2.3 option select (when TIMER_PWM_PIN is P2_3)


	// COM2B = 00: disconnect OC2B; to send signal set to 10: OC2B non-inverted
	// WGM2 = 101: phase-correct PWM with OCRA as top
	// CS2 = 000: no prescaling
	// The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR2A.
	TIMER_CONFIG_KHZ(khz);
}

