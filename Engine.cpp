
#include "Engine.h"
#include <ESP8266WebServer.h>

//Engine::Engine(int port, int ds1820pin, Remote* remote)
Engine::Engine(int port, Thermometer * thermometer, Remote* remote)
{
	this->server = new ESP8266WebServer(port);
	/*this->oneWire = new OneWire(ds1820pin);
	this->DS18B20 = new DallasTemperature(this->oneWire);
	this->DS18B20->begin();*/
	this->remote = remote;
	this->thermometer = thermometer;
	this->server->on("/Temperature", [this]() {onTemperature(); });
	this->server->on("/AirCond", HTTPMethod::HTTP_POST, [this]() {  onCondirionerCommand(); });
	this->server->onNotFound([this]() {badRequest("Bad Request");});

}

void Engine::start()
{
	this->server->begin();
	Serial.println("HTTP server started");
}

void Engine::onTemperature()
{
	/*this->DS18B20->requestTemperatures();
	String temperature = String(this->DS18B20->getTempCByIndex(0), 1);*/
	String temperature = String(this->thermometer->temperature(), 1);
	Serial.print("Temperature ");
	Serial.println(temperature);
	this->server->send(200, "text/plain", temperature);
}

void Engine::onCondirionerCommand()
{
	String message = "\nArguments: ";
	message += this->server->args();
	message += "\n";
	message += "mode " + this->server->arg("mode") + "\n";
	message += "fan " + this->server->arg("fan") + "\n";
	message += "temp " + this->server->arg("temp") + "\n";
	Mode mode = static_cast<Mode> (this->server->arg("mode").toInt());
	Fan fan = static_cast<Fan>(this->server->arg("fan").toInt());
	int temp = this->server->arg("temp").toInt();
	this->remote->set(mode, fan,temp );
	//this->remote->set(Mode::COOL, Fan::FAN3, this->server->arg("temp").toInt());
	this->server->send(200, "text/plain", message);


}

void Engine::badRequest(String msg)
{
	this->server->send(400, "text/plain", msg);
}
void Engine::loop()
{
	this->server->handleClient();
}
