// 
// 
// 

#include "Engine.h"
#include <ESP8266WebServer.h>
 

Engine::Engine(int port,int ds1820pin)
{
	this->server = new ESP8266WebServer(port);
	this->oneWire = new OneWire(ds1820pin);
	this->DS18B20 = new DallasTemperature(this->oneWire);
	this->DS18B20->begin();
	this->server->on("/Temperature", [this]() {onTemperature(); });
	this->server->on("/AirCond", HTTPMethod::HTTP_POST, [this]() {  onCondirionerCommand(); });

	this->server->onNotFound([this]() {this->server->send(404, "text/plain", "not found"); });
	 
}

void Engine::start()
{
	this->server->begin();
	Serial.println("HTTP server started");
}

void Engine::onTemperature()
{
	this->DS18B20->requestTemperatures();
	String temperature = String(this->DS18B20->getTempCByIndex(0), 1);
	Serial.print("Temperature ");
	Serial.println(temperature);
 	this->server->send(200, "text/plain",temperature);
}

void Engine::onCondirionerCommand()
{	 	 
	String message = "\nArguments: ";
	message += this->server->args();
	message += "\n";
	message += "mode " + this->server->arg("mode") + "\n";
	message += "fan " + this->server->arg("fan") + "\n";
	message += "temp " + this->server->arg("temp") + "\n";
	
	this->server->send(200, "text/plain", message);
}

void Engine::loop()
{
	this->server->handleClient();
}
