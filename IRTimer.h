// IRTimer.h

#ifndef _IRTIMER_h
#define _IRTIMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
class IRtimer {
public:
	IRtimer();
	void reset();
	uint32_t elapsed();
private:
	uint32_t start;
};

#endif

