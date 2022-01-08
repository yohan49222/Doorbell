/*
 Bibliothéques nécessaires :
  - pubsubclient : https://github.com/knolleary/pubsubclient
  - ArduinoJson v5.13.3 : https://github.com/bblanchon/ArduinoJson
*/

#include "IOB_IOT.h"
IOB_IOT *iob = IOB_IOT::getInstance();

#define RELAY_PIN 0	 // Numero de pin du relay
#define BUTTON_PIN 2 // Numero de pin du boutton
#define NCORNO true	 // Configure l'etat initial du relai
/**
	Filtre anti-rebond (debouncer)
*/
#define DEBOUNCE_TIME 100
volatile uint32_t DebounceTimer = 0;
uint32_t button_count = 0;
void IRAM_ATTR buttonpressed()
{
	if (millis() - DEBOUNCE_TIME >= DebounceTimer)
	{
		DebounceTimer = millis();
		button_count += 1;
		Serial.printf("Button has been pressed %u times\n", button_count);
	}
}

/**
	DINGDONG relay et Envois d'information a la domotic
	@param sendOn Envoyer un "On" a la box domotics
	@param sendOff Envoyer un "Off" a la box domotics
*/
void dingdong(bool sendOn, bool sendOff)
{

	Serial.println("Process DINGDONG START");

	if (sendOn)
		iob->SendData("On");

	digitalWrite(RELAY_PIN, NCORNO ? LOW : HIGH);
	delay(200);
	digitalWrite(RELAY_PIN, NCORNO ? HIGH : LOW);
	delay(1000);

	if (sendOff)
		iob->SendData("Off");

	Serial.println("Process DINGDONG END");
}

void ChangeState(IOB_IOTEventArgs e)
{
	if (e.State() == 1)
	{
		dingdong(false,true);
	}
	else if (e.State() == 0)
	{
		digitalWrite(RELAY_PIN, NCORNO ? HIGH : LOW);
	}
}

void setup()
{
	Serial.begin(115200L);
	delay(200);

	pinMode(RELAY_PIN, OUTPUT);
	pinMode(BUTTON_PIN, INPUT_PULLUP);

	digitalWrite(RELAY_PIN, NCORNO ? LOW : HIGH);

	attachInterrupt(BUTTON_PIN, buttonpressed, FALLING);

	iob->Run();
	iob->OnRecevChangeState(ChangeState);
}

void loop()
{
	iob->Loop();

	if (button_count >= 5)
	{
		button_count = 0;
		dingdong(true, true);
		Serial.println("button pressed");
	}
} 
