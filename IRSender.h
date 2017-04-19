#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "stdint.h"


 
class IRtimer {
public:
	IRtimer();
	void reset();
	uint32_t elapsed();
private:
	uint32_t start;
};

class IRSender
{
public:
	 
	IRSender(int pin) : IRpin(pin) {};
	void enableIROut(int khz);
	void mark(unsigned int usec);
	void space(unsigned long time);
private :
	int halfPeriodicTime;
	int IRpin ;
 


};


















