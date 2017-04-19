/*
 Name:		Node.ino
 Created:	4/19/2017 10:23:51 AM
 Author:	ivo
*/

#include "Thermometer.h"
#include "RemoteFactory.h"
 
#include <DallasTemperature.h>
#include <OneWire.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "Engine.h"

#define CONDITIONER DAIKIN
//#define CONDITIONER TOSHIBA
const char *ssid = "SSID1";
const char *password = "7402186329";


Remote *rc = RemoteFactory::Create(CONDITIONER, 2);
Thermometer *t = new Thermometer(5);
Engine engine(80, t,rc);

void setup() {
	Serial.begin(115200);
	WiFi.begin(ssid, password);
	Serial.println("Connecting");
	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	engine.start();
}

// the loop function runs over and over again until power down or reset
void loop() {
	engine.loop();

}

