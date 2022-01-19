/*
	*********************************************
	*** Firmware pour module relay ESP01-S 1M ***
	*********************************************

	!!! IMPORTANT : Vous devez faire une premiere compilation de votre projet 
					pour generer le fichier "config.json" .

					Vous devez ensuite editer ce meme fichier avec vos parametres

	Bibliothéques nécessaires :
		- pubsubclient : https://github.com/knolleary/pubsubclient
		- ArduinoJson v5.13.3 : https://github.com/bblanchon/ArduinoJson


platform = espressif8266
framework = arduino
board = esp07s
board_build.f_cpu = 160000000L
board_build.flash_mode = qio
board_build.ldscript = eagle.flash.1m64.ld
extra_scripts = 
	pre:preparefile.py
	pre:buildconfig.py
lib_deps = 
	knolleary/PubSubClient@^2.8
	bblanchon/ArduinoJson@5.13.3
upload_speed = 921600
monitor_speed = 115200


*/

#include "IOB_IOT.h"
IOB_IOT *iob = IOB_IOT::GetInstance();


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

void MessageRecep(IOB_IOTMessageRecevedEventArgs e)
{	
	for( String s : e.MessageList())
	{
		Serial.println("["+e.SendProtoleString()+"] " + s);
	}
	if (e.State() == 1)
	{
		dingdong(false,true);
	}
	else if (e.State() == 0)
	{
		digitalWrite(RELAY_PIN, NCORNO ? HIGH : LOW);
	}
	e.Handled(true);
}

void MessageSend(IOB_IOTMessageSendedEventArgs e)
{
	for( String s : e.MessageList())
	{
		Serial.println("["+e.SendProtoleString()+"] " + s);
	}
}

void WifiStateChanged(IOB_IOTWifiStateChangedEventArgs e)
{
	if(e.State() == 1)
	{
		Serial.println("[WIFI] Connecté");
		for( String s : e.MessageList())
		{
			Serial.println("[WIFI] " + s);
		}
	}
	
	else
	Serial.println("[WIFI] Déconnecter");
}

void MqttStateChanged(IOB_IOTMqttStateChangedEventArgs e)
{
	if(e.State() == 1)
	{
		Serial.println("[MQTT] Connecté");
		for( String s : e.MessageList())
		{
			Serial.println("[MQTT] " + s);
		}
	}
	
	else
	Serial.println("[MQTT] Déconnecter");
}

void ButtonPressed(IOB_IOTButtonPressedEventArgs e)
{
	dingdong(true, true);
	e.Handled(true);
}

void setup()
{
	Serial.begin(115200L);
	delay(200);

	iob->OnMessageRecep(MessageRecep);
	iob->OnMessageSend(MessageSend);
	iob->OnMqttStateChanged(MqttStateChanged);
	iob->OnWifiStateChanged(WifiStateChanged);
	iob->OnButtonPressed(ButtonPressed);

	iob->Run();
}

void loop()
{
	iob->Loop();
} 
