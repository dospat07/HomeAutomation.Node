#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "stdint.h"

#include "Remote.h"

#define DAIKIN_HDR_MARK	    3400
#define DAIKIN_HDR_SPACE	1600
#define DAIKIN_BIT_MARK	    400
#define DAIKIN_ONE_SPACE	1250
#define DAIKIN_ZERO_SPACE	400

#define DAIKIN_HDR_DELAY_MS   34 
#define DAIKIN_START_DELAY_MS 25 

#define DAIKIN_MSG_LEN 35
class Daikin :public Remote
{
public:

	Daikin(int IRpin);


private:
	uint8_t messageTemplate[DAIKIN_MSG_LEN] = {
		0x88, 0x5B, 0xE4, 0x00, 0xA3, 0x00, 0x00, 0xEB,
		0x88, 0x5B, 0xE4, 0x00, 0x42, 0x51, 0x20, 0x47,
		0x88, 0x5B, 0xE4, 0x00, 0x00, 0x12, 0x4C, 0x00, 0x05, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x03, 0x00, 0x00, 0x4A 
	};
	void setMode(Mode mode);
	void setFan(Fan fan);
	void setTemp(byte temp);
	void setPowerfull(bool);
	void send();


	void sendPart(void* data, uint8_t position, uint8_t lenght);
	//void sendByte(uint8_t byte);
	void calcCRC();
	uint8_t reverse(uint8_t byte);


};


