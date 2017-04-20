#include "Toshiba.h"

Toshiba::Toshiba(int IRpin) :message(
{
	//0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x51, 0x03, 0x00, 0x53 //Heat22Auto
	0xF2, 0x0D, 0x07, 0xF8, 0x01, 0x74, 0x07, 0x00, 0x55, 0x3C, 0x01, 0x02, 0x18 //OFF
}),Remote(IRpin)
{
}

void Toshiba::set(Mode mode, Fan fan,byte temp)
{
	uint16_t data = 0;
	switch (mode)
	{
	case OFF:send(0);return;
		 
	case HEAT:data = 3;
		break;
	case COOL:data = 1;
		break;
	default:
		break;
	}
	if (fan > FAN3) fan = FAN3;
	data = ((fan << 6 || data)<<8)||((temp-17)*16+1);
	Serial.println("Toshiba set");
	Serial.print("data ");Serial.println(data);
	send(data);
}

//
// data
// FF0000MM TTTTTTTT
// MM  - 11  HEAT, 01 COOL
// FF  - 00  Auto,01 FAN1 ,10 FAN2 ,11 FAN3
// TT   -(temp-17)*16+1
// byte 5 -temperature
// byte 6 mode&fan
//
void Toshiba::prepareMessage(uint16_t data)
{
	if (data > 0)
	{
		this->message[2] = 0x03;
		this->message[3] = 0xF8;
		this->message[5] = data;
		this->message[6] = (data >> 8);
		this->lenght = 9;
		calcCRC();
	}
	else
	{
		this->message[2] = 0x07;
		this->message[3] = 0xFC;
		this->message[5] = 0x74;
		this->message[6] = 0x07;
		this->message[8] = 0x55;
		this->lenght = 13;
		// message[] = { 0xF2, 0x0D, 0x07, 0xF8, 0x01, 0x74, 0x07, 0x00, 0x55, 0x3C, 0x01, 0x02, 0x18 };
	}
	
}

void Toshiba::send(uint16_t data)
{
	prepareMessage(data);
	sender.enableIROut(38);
	sendPart();
	delayMicroseconds(TOSHIBA_HDR_DELAY);
	sendPart();
}
 
void Toshiba::sendPart( )
{
	sender.mark(TOSHIBA_HDR_MARK);
	sender.space(TOSHIBA_HDR_SPACE);
	for (int i = 0; i <this->lenght; i++)
	{
		sendByte(((uint8_t*)message)[i]);
	}
	sender.mark(TOSHIBA_BIT_MARK);
}

void Toshiba::sendByte(uint8_t byte)
{
	for (int i = 0; i < 8; i++)
	{
		if ((byte & 0x80)>0) 
		{
			sender.mark(TOSHIBA_BIT_MARK);
			sender.space(TOSHIBA_ONE_SPACE);
		}
		else 
		{
			sender.mark(TOSHIBA_BIT_MARK);
			sender.space(TOSHIBA_ZERO_SPACE);
		}
		byte = byte << 1;
	}
}

void Toshiba::calcCRC()
{
	message[8] = 0;
	for (int  i = 4; i < 8; i++)
	{
		message[8]  ^= message[i];
	}
	 
}