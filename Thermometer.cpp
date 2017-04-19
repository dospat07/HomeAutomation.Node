// 
// 
// 

#include "Thermometer.h"
/* temperature in celsius */
float Thermometer::temperature()
{
	this->DS18B20->requestTemperatures();
	return this->DS18B20->getTempCByIndex(0);
}
