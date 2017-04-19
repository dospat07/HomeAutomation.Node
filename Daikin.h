#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "stdint.h"
#include "IRSender.h"

#define DAIKIN_HDR_MARK	    3400
#define DAIKIN_HDR_SPACE	1600
#define DAIKIN_BIT_MARK	    400
#define DAIKIN_ONE_SPACE	1250
#define DAIKIN_ZERO_SPACE	400
//#define DAIKIN_HDR_DELAY    34776
//#define DAIKIN_START_DELAY  25341
#define DAIKIN_HDR_DELAY_MS   34 
#define DAIKIN_START_DELAY_MS 25 

 
class Daikin  : public IRSender
{
public:
	Daikin();
	~Daikin();
	 
	
	void send(uint16_t data);
	

private :
	uint8_t message[35];
	void calcCRC(uint8_t start, uint8_t len);
	uint8_t reverse(uint8_t byte);
	void sendPart(void* data, uint8_t position, uint8_t lenght);
	void sendByte(uint8_t byte);
	void prepareMessage(uint16_t data);
};


