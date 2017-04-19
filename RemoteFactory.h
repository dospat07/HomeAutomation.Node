// RemoteFactory.h
#include "Remote.h"
#include "Toshiba.h"
#include "Daikin.h"
#ifndef _REMOTEFACTORY_h
#define _REMOTEFACTORY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

enum Conditioner { DAIKIN = 0, TOSHIBA };
class RemoteFactory
{
public:
	static Remote* Create(Conditioner type,ushort irPin)
	{

		Remote* remote = NULL;
		switch (type)
		{
		case DAIKIN: remote = new Daikin(irPin);break;
		case TOSHIBA: remote = new Toshiba(irPin);break;
		default:
			break;
		}
		return remote;
	}
};

#endif

