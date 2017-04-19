
#include "Engine.h"
#include <ESP8266WebServer.h>

 
Engine::Engine(int port, Thermometer * thermometer, Remote* remote)
{
	this->server = new ESP8266WebServer(port);
	this->remote = remote;
	this->thermometer = thermometer;
	this->server->on("/Temperature", [this]() {onTemperature(); });
	this->server->on("/AirCond", HTTPMethod::HTTP_POST, [this]() {  onCondirionerCommand(); });
	this->server->onNotFound([this]() {badRequest();});

}

void Engine::start()
{
	this->server->begin();
	Serial.println("HTTP server started");
}

void Engine::onTemperature()
{
	String temperature = String(this->thermometer->temperature(), 1);
	Serial.print("Temperature :");
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
	Serial.println(message);

	Mode mode = static_cast<Mode> (this->server->arg("mode").toInt());
	Fan fan = static_cast<Fan>(this->server->arg("fan").toInt());
	int temp = this->server->arg("temp").toInt();

	this->remote->set(mode, fan, temp);
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
