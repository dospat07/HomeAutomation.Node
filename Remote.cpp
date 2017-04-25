#include "Remote.h"

void Remote::set(Mode mode, Fan fan, byte temp)
{

	setMode(mode);
	setFan(fan);
	setTemp(temp);

	this->calcCRC();
	this->printMessage();
	this->send();
}

void Remote::printMessage()
{
#ifdef DEBUG_MESSAGE
	Serial.println();
	Serial.print(this->acName);Serial.println(" send message : ");
	for (int i = 0; i < this->messageLenght; i++)
	{
		Serial.print(this->message[i], HEX); Serial.print(" ");
	};
#endif // DEBUG_MESSAGE	
}

void Remote::sendByte(uint8_t byte, unsigned int bitMark, unsigned int oneSpace, unsigned int zeroSpace)
{

	for (int i = 0; i < 8; i++)
	{
		if ((byte & 0x80) > 0)
		{
			sender.mark(bitMark);
			sender.space(oneSpace);
		}
		else
		{
			sender.mark(bitMark);
			sender.space(zeroSpace);
		}
		byte = byte << 1;
	}
}
