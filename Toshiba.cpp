#include "Toshiba.h"


Toshiba::Toshiba(int IRpin) : Remote("Toshiba",IRpin,TOSHIBA_MSG_LEN, this->messageTemplate)
{
	//memcpy(this->message, this->messageTemplate, TOSHIBA_MSG_LEN);
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
void Toshiba::send()
{
	
	sender.enableIROut(38);
	sendPart();
	delayMicroseconds(TOSHIBA_HDR_DELAY);
	sendPart();
	
}

void Toshiba::setPowerfull(bool powerfull)
{
}

void Toshiba::setMode(Mode mode)
{
	switch (mode)
	{
	case OFF:	this->message[6] = B00000111;
		break;
	case HEAT:	this->message[6] = this->message[6] | B00000011;
		break;
	case COOL:	this->message[6] = this->message[6] | B00000001;
		break;
	default:
		this->message[6] = 0;
		break;
	}
}

void Toshiba::setFan(Fan fan)
{
	if (fan > FAN5) fan = FAN5;
	this->message[6] = this->message[6] | fan << 6;
}

void Toshiba::setTemp(byte temp)
{
	if (temp < 17 || temp>30) temp = 17;
	this->message[5] = (temp - 17) << 4;
}

void Toshiba::sendPart()
{
	sender.mark(TOSHIBA_HDR_MARK);
	sender.space(TOSHIBA_HDR_SPACE);
	for (int i = 0; i < TOSHIBA_MSG_LEN; i++)
	{
		//sendByte(((uint8_t*)message)[i]);
		 this->sendByte(message[i], TOSHIBA_BIT_MARK, TOSHIBA_ONE_SPACE, TOSHIBA_ZERO_SPACE);
	}
	sender.mark(TOSHIBA_END_MARK);
}

void Toshiba::calcCRC()
{
	message[8] = 0;
	for (int i = 0; i < 8; i++)
	{
		message[8] ^= message[i];
	}

}


//void Toshiba::sendByte(uint8_t byte)
//{
//	for (int i = 0; i < 8; i++)
//	{
//		if ((byte & 0x80) > 0)
//		{
//			sender.mark(TOSHIBA_BIT_MARK);
//			sender.space(TOSHIBA_ONE_SPACE);
//		}
//		else
//		{
//			sender.mark(TOSHIBA_BIT_MARK);
//			sender.space(TOSHIBA_ZERO_SPACE);
//		}
//		byte = byte << 1;
//	}
//}

