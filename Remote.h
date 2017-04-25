// Remote.h
#include "IRSender.h"
#ifndef _REMOTE_h
#define _REMOTE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define DEBUG_MESSAGE;
 
enum Mode { OFF=0, HEAT, COOL };
enum Fan { AUTO=0, FAN1, FAN2, FAN3, FAN4, FAN5,FAN6};
class Remote
{
public:
	Remote(int IRpin,uint messageSize , uint8_t *pMessage) :sender(IRpin)
	{
		this->message = &pMessage[0];
		this->messageLenght = messageSize;
	};
protected:
	
	IRSender sender;
	uint messageLenght;
	virtual void setMode(Mode mode) = 0;
	virtual void setFan(Fan fan) = 0;
	virtual void setTemp(byte temp) = 0;
	virtual void calcCRC() = 0;
	virtual void send() = 0;
	virtual void setPowerfull(bool)=0;
public:	
	 
	uint8_t*  message;
	void set(Mode mode, Fan fan, byte temp);
	void printMessage();
	void sendByte(uint8_t byte, unsigned int bitMark, unsigned int oneSpace, unsigned int zeroSpace);
	
};

#endif


