#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "IRSender.h"
#include "Remote.h"

#define TOSHIBA_HDR_MARK	4500
#define TOSHIBA_HDR_SPACE	4510
#define TOSHIBA_BIT_MARK	520
#define TOSHIBA_ONE_SPACE	1680
#define TOSHIBA_ZERO_SPACE	572
#define TOSHIBA_HDR_DELAY   5560

class Toshiba:public Remote	
{
public:

	Toshiba(int IRpin);	
	void set(Mode mode, Fan fan, byte temp);
	void send(byte mode,byte temp);

private:
	 
	uint8_t message[13];
	void prepareMessage(byte mode, byte temp);
	void sendPart();
	void sendByte(uint8_t byte);
	void calcCRC();
    uint8_t lenght;
};

