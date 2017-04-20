
#include "Engine.h"
#include <ESP8266WebServer.h>
#include "RemoteFactory.h"
 /*
Engine::Engine(int port, Thermometer * thermometer, Remote* remote)
{
	this->remote = remote;
	this->server = new ESP8266WebServer(port);
	this->thermometer = thermometer;
	this->server->on("/Temperature", [this]() {onTemperature(); });
	this->server->on("/AirCond", HTTPMethod::HTTP_POST, [this]() {  onCondirionerCommand(); });
	this->server->onNotFound([this]() {badRequest();});

}*/
Engine::Engine(int port, Thermometer * thermometer, int IRpin)
{
	this->server = new ESP8266WebServer(port);
	this->thermometer = thermometer;
	this->server->on("/Temperature", [this]() {onTemperature(); });
	this->server->on("/AirCond", HTTPMethod::HTTP_POST, [this]() {  onCondirionerCommand(); });
	this->server->onNotFound([this]() {badRequest();});
	this->irPin = IRpin;
}
void Engine::start()
{
	this->server->begin();
	Serial.println("HTTP server started");
}

void Engine::onTemperature()
{
	String temperature = String(this->thermometer->temperature(), 1);
	String response = "{""temperature"":" + temperature + "}";
	Serial.print("Temperature :");
	Serial.println(temperature);
	this->server->send(200, "text/plain", response);
}

void Engine::onCondirionerCommand()
{
	String message = "\n\rParameters: ";
	message += this->server->args();
	message += "\n\r";
	message += "mode " + this->server->arg("mode") + "\n\r";
	message += "fan " + this->server->arg("fan") + "\n\r";
	message += "temp " + this->server->arg("temp") + "\n\r";
	message += "model " + this->server->arg("model") + "\n\r";
	Serial.println(message);

	Mode mode = static_cast<Mode> (this->server->arg("mode").toInt());
	Fan fan = static_cast<Fan>(this->server->arg("fan").toInt());
	int temp = this->server->arg("temp").toInt();
	Conditioner conditioner = static_cast<Conditioner> (this->server->arg("model").toInt());

	this->remote = RemoteFactory::Create(conditioner, irPin);
	this->remote->set(mode, fan, temp);
	delete this->remote;
	this->server->send(200, "text/plain", message);
	


}

void Engine::badRequest()
{
	this->server->send(400, "text/plain", "Bad Request");
}
void Engine::loop()
{
	this->server->handleClient();
}
