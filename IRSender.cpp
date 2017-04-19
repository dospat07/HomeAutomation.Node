#include "IRSender.h"


IRtimer::IRtimer() {
	reset();
}

void ICACHE_FLASH_ATTR IRtimer::reset() {
	start = micros();
}

uint32_t ICACHE_FLASH_ATTR IRtimer::elapsed() {
	uint32_t now = micros();
	if (start <= now)  // Check if the system timer has wrapped.
		return (now - start);  // No wrap.
	else
		return (0xFFFFFFFF - start + now);  // Has wrapped.
}






void ICACHE_FLASH_ATTR IRSender::mark(unsigned int usec) {
	// Sends an IR mark for the specified number of microseconds.
	// The mark output is modulated at the PWM frequency.
	IRtimer usecTimer = IRtimer();
	while (usecTimer.elapsed() < usec) {
		digitalWrite(IRpin, HIGH);
		delayMicroseconds(halfPeriodicTime);
		digitalWrite(IRpin, LOW);
		// e.g. 38 kHz -> T = 26.31 microsec (periodic time), half of it is 13
		delayMicroseconds(halfPeriodicTime);
	}

}

/* Leave pin off for time (given in microseconds) */
void ICACHE_FLASH_ATTR IRSender::space(unsigned long time) {
	// Sends an IR space for the specified number of microseconds.
	// A space is no output, so the PWM output is disabled.
	//ledOff();

	digitalWrite(IRpin, LOW);
	if (time == 0) return;
	if (time <= 16383)  // delayMicroseconds is only accurate to 16383us.
		delayMicroseconds(time);
	else {
		// Invoke a delay(), where possible, to avoid triggering the WDT.
		delay(time / 1000UL);  // Delay for as many whole ms as we can.
		delayMicroseconds((int)time % 1000UL);  // Delay the remaining sub-msecond.
	}
}


void ICACHE_FLASH_ATTR IRSender::enableIROut(int khz) {

	// Enables IR output.
	// The khz value controls the modulation frequency in kilohertz.

	// T = 1/f but we need T/2 in microsecond and f is in kHz
	halfPeriodicTime = 500 / khz;
}

