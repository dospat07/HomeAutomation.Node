
#include "Engine.h"
#include <ESP8266WebServer.h>
#include "RemoteFactory.h"
 
Engine::Engine(int port, Thermometer * thermometer, Remote* remote)
{
	this->remote = remote;
	this->server = new ESP8266WebServer(port);
	this->thermometer = thermometer;
	this->server->on("/Temperature", [this]() {onTemperature(); });
	this->server->on("/AirCond", HTTPMethod::HTTP_POST, [this]() {  onCondirionerCommand(); });
	this->server->onNotFound([this]() {badRequest();});

}
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
	String message = "\nParameters: ";
	message += this->server->args();
	message += "\n";
	message += "mode " + this->server->arg("mode") + "\n";
	message += "fan " + this->server->arg("fan") + "\n";
	message += "temp " + this->server->arg("temp") + "\n";
	message += "model " + this->server->arg("model") + "\n";
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
