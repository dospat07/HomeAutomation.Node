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
#define TOSHIBA_BIT_MARK	526
#define TOSHIBA_ONE_SPACE	1680
#define TOSHIBA_ZERO_SPACE	557
#define TOSHIBA_HDR_DELAY   5560
#define TOSHIBA_END_MARK    520


//#define TOSHIBA_HDR_MARK	4400 
//#define TOSHIBA_HDR_SPACE	4300 
//#define TOSHIBA_BIT_MARK	560
//#define TOSHIBA_ONE_SPACE	1590
//#define TOSHIBA_ZERO_SPACE	472
//#define TOSHIBA_HDR_DELAY   7048
//#define TOSHIBA_END_MARK    440

//#define HVAC_TOSHIBA_HDR_MARK  4400  
//#define HVAC_TOSHIBA_HDR_SPACE  4300  
//#define HVAC_TOSHIBA_BIT_MARK  560  
//#define HVAC_TOSHIBA_ONE_SPACE  1590  
//#define HVAC_MISTUBISHI_ZERO_SPACE 472  
//#define HVAC_TOSHIBA_RPT_MARK  440  
//#define HVAC_TOSHIBA_RPT_SPACE  7048 // Above original iremote limit  
#define TOSHIBA_MSG_LEN 9
class Toshiba:public Remote	
{
public:

	Toshiba(int IRpin);	
		
private:
	uint8_t messageTemplate[TOSHIBA_MSG_LEN] ={ 0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00 }; 
	void setMode(Mode mode);
	void setFan(Fan fan);
	void setTemp(byte temp);
	void send();
	void setPowerfull(bool powerfull);
	//uint8_t message[TOSHIBA_MGS_LEN];
	void sendPart();
	//void sendByte(uint8_t byte);
	void calcCRC();
    
};

