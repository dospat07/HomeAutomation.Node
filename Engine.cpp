
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
	this->server->on("/Remote", HTTPMethod::HTTP_POST, [this]() {  onCondirionerCommand(); });
	this->server->onNotFound([this]() {badRequest();});
	this->irPin = IRpin;
}
void Engine::start()
{
	this->server->begin();
	Serial.println("HTTP server started");
}

void Engine::showRequest()
{

	String message = "\n\rRequest ";
	message += this->server->uri()+"\n\r";
	for (int i = 0; i < this->server->args(); i++)
	{
		message += this->server->argName(i)+" " + this->server->arg(i)+"\n\r";
	}
	;
	/*message += "\n\r";
	message += "mode  " + this->server->arg("mode") 
	message += "fan   " + this->server->arg("fan") + "\n\r";
	message += "temp  " + this->server->arg("temp") + "\n\r";
	message += "model " + this->server->arg("model") + "\n\r";*/
	Serial.println(message);
	 
}
void Engine::onTemperature()
{
	showRequest();
	String temperature = String(this->thermometer->temperature(), 1);
	String response = "{\"temperature\":" + temperature + "}";
	 
	Serial.println(response);
	this->server->send(200, "text/plain", response);
}

void Engine::onCondirionerCommand()
{
	showRequest();
	Mode mode = static_cast<Mode> (this->server->arg("mode").toInt());
	Fan fan = static_cast<Fan>(this->server->arg("fan").toInt());
	int temp = this->server->arg("temp").toInt();
	Conditioner conditioner = static_cast<Conditioner> (this->server->arg("model").toInt());
	
	String response = "mode " + String(mode);	 
	response += " fan " + String(fan);
	response += " temp " + String(temp);
	response += " model " + String(conditioner);
    this->remote = RemoteFactory::Create(conditioner, irPin);
	if (remote != NULL ) {
		this->remote->set(mode, fan, temp);
		delete this->remote;
		this->server->send(200, "text/plain", response);
	}
	else
	{
		badRequest();
	}
	
}

void Engine::badRequest()
{
	this->server->send(400, "text/plain", "Bad Request");
}
void Engine::loop()
{
	this->server->handleClient();
}
