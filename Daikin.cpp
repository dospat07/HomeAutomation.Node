	#include "Daikin.h"
 
	Daikin::Daikin(int IRpin) : message(
	{

		  0x88, 0x5B, 0xE4, 0x00, 0xA3, 0x00, 0x00, 0xEB, 
		  0x88, 0x5B, 0xE4, 0x00, 0x42, 0x51, 0x20, 0x47, 
		  0x88, 0x5B, 0xE4, 0x00, 0x00, 0x12, 0x4C, 0x00, 0x05, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x03, 0x00, 0x00, 0x4A 
		/*  
		0x88, 0x5B, 0xE4, 0x00, 0xA3, 0x00, 0x00, 0xEB,
		0x88, 0x5B, 0xE4, 0x00, 0x42, 0x23, 0x80, 0x98,
		0x88, 0x5B, 0xE4, 0x00, 0x00, 0x92, 0x44, 0x00, 0x05, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x03, 0x00, 0x00, 0xC2 */
	}), Remote(IRpin)
	{
		 
	};
	
	
	/* 
	https://jamesstewy.com/blog/post/8/
	O M M M F F F F  P T T T T T T T 0

	O   on/off byte21/b7
	0 - OFF
	1 - ON

	MMM  mode byte21/b4b3b2
	000 - Auto
	010 - Dry
	110 - Cool
	001 - Heat
	011 - Fan

	FFFF  Fan Speed byte24/b4b3b2b1
	0101 - Auto
	1101 - Night
	1100 - 1
	0010 - 2
	1010 - 3
	0110 - 4
	1110 - 5

	P - powerfull byte29/bit7
	0 - off
	1 - on

	TTTTTT  - Temperature byte22/b6b5b4b3b2b1
	must be reversed

	22c = 010110 reversed = 011010 26
	23c = 010111 reversed = 111010 58
	24c = 011000 reversed = 000110  6
	*/

	void Daikin::set(Mode mode, Fan fan, byte temp)
	{
		byte _mode = B10000000;
		switch (mode)
		{
		case OFF:_mode =  B00000000;
			break;
		case HEAT:_mode = B10000010;
			break;
		case COOL:_mode = B10001100;
			break;
		default:
			break;
		}
		/*  ON/OF		*/
		this->message[21] = this->message[21] & B01111111; // clear 
		this->message[21] = this->message[21] | (B10000000 & _mode);

		/*  MODE        */
		this->message[21] = this->message[21] & B11110001; // clear 
		this->message[21] = this->message[21] | (B00001110 & _mode);

		byte _fan = B01010000;
		switch (fan)
		{
		case AUTO:_fan = B01010000;
			break;
		case FAN1:_fan = B11010000;
			break;
		case FAN2:_fan = B11000000;
			break;
		case FAN3:_fan = B00100000;
			break;
		case FAN4:_fan = B10100000;
			break;
		case FAN5:_fan = B01100000;
			break;
		case FAN6:_fan = B11100000;
			break;
		default:
			break;
		}	
		/*  FAN			*/
		this->message[24] = this->message[24] & B11110000; // clear 
		this->message[24] = this->message[24] | (B00001111 & _fan);

		/*  Tempertature*/
		this->message[22] = reverse(temp)>>2;

		/*  Powerfull   */
		//this->message[29] = this->message[29] & B01111111; // clear 
		//this->message[29] = this->message[29] | (B10000000 & data);
		this->calcCRC(16, 18);
		this->send();
	}
	void Daikin::prepareMessage(uint16_t data)
	{
		uint8_t byte =data>>8;
		/*  ON/OF		*/
		this->message[21] = this->message[21] & B01111111; // clear 
		this->message[21] = this->message[21] | (B10000000 & byte);

		/*  MODE        */	
		this->message[21] = this->message[21] & B11110001; // clear 
		this->message[21] = this->message[21] | (B00001110 & (byte >> 3));	

		/*  Tempertature*/
		this->message[22] = data & 0x7F;

		/*  FAN			*/
		this->message[24] = this->message[24] & B11110000; // clear 
		this->message[24] = this->message[24] | (B00001111 & byte);

		/*  Powerfull   */
		this->message[29] = this->message[29] & B01111111; // clear 
		this->message[29] = this->message[29] | (B10000000 & data);

		this->calcCRC(16, 18);
		
	}

	uint8_t Daikin::reverse(uint8_t byte)
	{/*
		uint8_t reversed = 0;
		for (int i = 0; i < 8; i++)
		{
			 
			reversed += ((uint8_t)pow(2, 7 - i))*(((uint8_t)pow(2, i) & byte) > 0);
		}
		return reversed;*/
		uint8_t reversed = 0;
		uint8_t mask = B11111111;
	    uint8_t left_shift = B10000000;
		uint8_t right_shift = B00000001;

		while (right_shift>0)
		{
			if ((left_shift&byte) > 0)
			{
				 
				reversed = reversed | (mask&right_shift);
			}
			 
			left_shift >>= 1;
			right_shift <<= 1;
		}
		return reversed;


	}

	void Daikin::send()
	{
		 
		sender.enableIROut(38);

		sender.mark(DAIKIN_BIT_MARK);

		delay(DAIKIN_START_DELAY_MS);

		sendPart(this->message, 0, 8);
		delay(DAIKIN_HDR_DELAY_MS);
		sendPart(this->message, 8, 8);
		delay(DAIKIN_HDR_DELAY_MS);
		sendPart(this->message, 16, 19);
	}

	void Daikin::send(uint16_t data)
	{
		prepareMessage(data);
		send();
	}

	void Daikin::sendPart(void* data, uint8_t position, uint8_t lenght)
	{
		sender.mark(DAIKIN_HDR_MARK);
		sender.space(DAIKIN_HDR_SPACE);
		for (int i = position; i <position + lenght; i++)
		{
			sendByte(((uint8_t*)data)[i]);
		}
		sender.mark(DAIKIN_BIT_MARK);
	}


	void Daikin::sendByte(uint8_t byte)
	{
		for (int i = 0; i < 8; i++)
		{


			if ((byte & 0x80)>0) {


				sender.mark(DAIKIN_BIT_MARK);
				sender.space(DAIKIN_ONE_SPACE);
			}
			else {

				sender.mark(DAIKIN_BIT_MARK);
				sender.space(DAIKIN_ZERO_SPACE);
			}
			byte = byte << 1;
		}
	}
	void Daikin::calcCRC(uint8_t start, uint8_t len)
	{
		uint16_t crc = 0;
		for (uint8_t i = start; i < start + len; i++)
		{

			crc += this->reverse(this->message[i]);


		}

		/*	Serial.print("crc  ");
		Serial.println(crc);*/
		this->message[start + len] = this->reverse(crc);

	}
	 
