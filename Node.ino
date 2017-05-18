/*
 Name:		Node.ino
 Created:	4/19/2017 10:23:51 AM
 Author:	ivo
*/
 
#include "Thermometer.h"
#include "RemoteFactory.h"
#include "Engine.h"

#define IR_PIN 13    //GPIO 13
#define DS1820_PIN 5 //GPIO 5
#define TCP_PORT  3000
const char *ssid = "SSID1";
const char *password = "7402186329";

//Engine engine(80, new Thermometer(DS1820_PIN), RemoteFactory::Create(CONDITIONER, IR_PIN));

Engine engine(TCP_PORT, new Thermometer(DS1820_PIN), IR_PIN);
void setup() 
{
	pinMode(IR_PIN, OUTPUT);
	digitalWrite(IR_PIN, LOW);

	Serial.begin(115200);	
	connect();
	engine.start();
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	engine.loop();
}

void connect()
{
	Serial.println("Connecting ");
	WiFi.begin(ssid, password);
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
}