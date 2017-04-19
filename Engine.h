// Engine.h

#include <ESP8266WebServer.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "Remote.h"
#include "Thermometer.h"
#ifndef _ENGINE_h
#define _ENGINE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Engine {
public:
	//Engine(int port, int ds1820pin,Remote* remote
	Engine(int port, Thermometer * thermometer, Remote* remote);
	void start();
	void loop();
	
private :


	//// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
	//OneWire *oneWire;

	//// Pass our oneWire reference to Dallas Temperature. 
	//DallasTemperature *DS18B20;

	ESP8266WebServer *server;
	Thermometer * thermometer;
	Remote* remote;
	void onTemperature();
	void onCondirionerCommand();
	void badRequest(String msg);
	
	
};

#endif

