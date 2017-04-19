// Thermometer.h
#include <DallasTemperature.h>
#include <OneWire.h>
#ifndef _THERMOMETER_h
#define _THERMOMETER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
class Thermometer
{
public :
	Thermometer(int ds1820pin) 
	{
		this->oneWire = new OneWire(ds1820pin);
		this->DS18B20 = new DallasTemperature(this->oneWire);
		this->DS18B20->begin();
	}
	float temperature();
private:

	// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
	OneWire *oneWire;

	// Pass our oneWire reference to Dallas Temperature. 
	DallasTemperature *DS18B20;
};

#endif

