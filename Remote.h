// Remote.h
#include "IRSender.h"
#ifndef _REMOTE_h
#define _REMOTE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
enum Mode { OFF=0, HEAT, COOL };
enum Fan { AUTO=0, FAN1, FAN2, FAN3, FAN4, FAN5,FAN6};
class Remote
{
public:
	Remote(int IRpin) :sender(IRpin)
	{};
protected:
	IRSender sender;
public:	
	virtual void set(Mode mode, Fan fan, byte temp);
	
};

#endif


