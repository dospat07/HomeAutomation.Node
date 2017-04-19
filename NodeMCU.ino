



#include <DallasTemperature.h>
#include <OneWire.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "Engine.h"

const char *ssid = "SSID1";
const char *password = "7402186329";
Engine engine(80, 5);
void setup() {
	Serial.begin(115200);
	WiFi.begin(ssid, password);
	Serial.println("");

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

