/*
 Bibliothéques nécessaires :
  - pubsubclient : https://github.com/knolleary/pubsubclient
  - ArduinoJson v5.13.3 : https://github.com/bblanchon/ArduinoJson
*/

#define USE_WIFI
#define USE_OTA
#define USE_MQTT
#define USE_HTTP
#define USE_WEBSERVER

#include <ESP8266WiFi.h>

#define PRINTLN(VAR) \
Serial.println(VAR);

#define RELAY_PIN 0	 // Numero de pin du relay
#define BUTTON_PIN 2 // Numero de pin du boutton
#define NCORNO true	 // Configure l'etat initial du relai
/*
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

#ifdef USE_WIFI

// Inclure les librairies.
#include <ArduinoJson.h>
#include "wifi_config.h"
#include "app_config.h"
WiFiClient espClient;
bool haveWifi = false;


void onConnected(const WiFiEventStationModeConnected &event);
void onDisconnected(const WiFiEventStationModeDisconnected &event);
void onGotIP(const WiFiEventStationModeGotIP &event);
void SendData(String state);

#ifdef USE_HTTP
#include <ESP8266HTTPClient.h>
bool canusehttp = false;
HTTPClient httpClient;
#endif

#ifdef USE_MQTT
#include <PubSubClient.h>
void callbackMQTT(char *topic, byte *message, unsigned int length);
PubSubClient MQTT_Client(MQTT_SERVER, MQTT_PORT, callbackMQTT, espClient);
bool canusemqtt = false;
bool canusemqttsecure = false;
#endif // USE_MQTT

#ifdef USE_OTA
#include <ArduinoOTA.h>
#endif //USE_OTA

#ifdef USE_WEBSERVER
#include <ESP8266WebServer.h>
ESP8266WebServer webServer(80);
#endif //USE_WEBSERVER

unsigned long previousMillisMQTT = 0;
unsigned long intervalConnectMQTT = 1000;

bool definedString(const char *stest)
{
	return String(stest).compareTo("") > 0;
}

bool definedInt(const int itest)
{
	return itest > 0;
}

void CheckConfig()
{
	haveWifi = (definedString(SSID_PASSWORD) && definedString(SSID_PASSWORD));
	Serial.printf("haveWifi : %s\n", String(haveWifi).c_str());

#ifdef USE_MQTT
	bool haveTopic = (definedString(TOPICIN) && definedString(TOPICOUT));
	bool haveMQTT = (definedString(MQTT_SERVER) && definedInt(MQTT_PORT));
	bool haveMQTTsecure = (definedString(MQTT_LOGIN) && definedString(MQTT_PASSWORD));
	bool haveDevice = (definedString(NOMMODULE) && definedInt(IDXDEVICE));
	canusemqtt = (haveWifi && haveTopic && haveMQTT && haveDevice);
	canusemqttsecure = (canusemqtt && haveMQTTsecure);
	Serial.printf("haveMQTT : %s\n", String(haveMQTT).c_str());
	Serial.printf("haveMQTTsecure : %s\n", String(haveMQTTsecure).c_str());
	Serial.printf("haveTopic : %s\n", String(haveTopic).c_str());
	Serial.printf("haveDevice : %s\n", String(haveDevice).c_str());	
	Serial.printf("canusemqtt : %s\n", String(canusemqtt).c_str());
	Serial.printf("canusemqttsecure : %s\n", String(canusemqttsecure).c_str());
#endif // USE_MQTT

#ifdef USE_HTTP
	bool haveDomotic = (definedString(DOMOTIC_SERVER) && definedInt(DOMOTIC_PORT));
	bool canusehttp = (haveWifi && haveDomotic && definedInt(IDXDEVICE));
	Serial.printf("canusehttp : %s\n", String(canusehttp).c_str());
	Serial.printf("haveDomotic : %s\n", String(haveDomotic).c_str());
#endif // USE_HTTP
}

/*
creation d'une string au format json
return true si ok sinon false
*/
bool creatJsonMEssage(String state, String &out)
{
	StaticJsonBuffer<256> jsonBuffer;
	JsonObject &root = jsonBuffer.createObject();

	// assigantion des variables.
	root["command"] = "switchlight";
	root["idx"] = IDXDEVICE;
	root["switchcmd"] = state;

	String messageOut;

	// formatage de la chaine json
	int printed = root.printTo(messageOut);
	if (printed > 0)
	{
		/*
		 Convertion du message en Char
		 */
		char messageChar[messageOut.length() + 1];
		messageOut.toCharArray(messageChar, messageOut.length() + 1);

		// assignation de out
		out = String(messageChar);
		return true;
	}
	return false;
}

#endif // USE_WIFI

/**
	DINGDONG relay et Envois d'information a la domotic
	@param sendOn Envoyer un "On" a la box domotics
	@param sendOff Envoyer un "Off" a la box domotics
*/
void dingdong(bool sendOn, bool sendOff)
{

	PRINTLN("Process DINGDONG START");

#ifdef USE_WIFI
	if (sendOn)
		SendData("On");
#endif // USE_WIFI

	digitalWrite(RELAY_PIN, NCORNO ? LOW : HIGH);
	delay(200);
	digitalWrite(RELAY_PIN, NCORNO ? HIGH : LOW);
	delay(1000);

#ifdef USE_WIFI
	if (sendOff)
		SendData("Off");
#endif // USE_WIFI
	PRINTLN("Process DINGDONG END");
}

/**
	DINGDONG relay et Envois d'information a la domotic
	@param sendOn Envoyer un "On" a la box domotics
*/
void dingdong(bool sendOn)
{
	dingdong(sendOn, true);
}

/**
	DINGDONG relay et Envois d'information a la domotic
*/
void dingdong()
{
	dingdong(false);
}

#ifdef USE_WEBSERVER
void switchOn()
{
	PRINTLN("Reception HTTP sur l'url /SWITCHON");
	dingdong();
	String messJson;
	PRINTLN("Envois de la reponse HTTP");
	if (creatJsonMEssage("On", messJson))
		webServer.send(200, "text/json", messJson);
}
void switchOff()
{
}
#endif //USER_WEBSERVER

#ifdef USE_MQTT
// CONNEXION MQTT
// **************
void reconnectMQTT()
{
#ifdef USE_WIFI
#ifdef USE_MQTT
	if (WiFi.isConnected() && canusemqtt)
	{
		// Boucle jusqu'à la connexion MQTT
		while (!MQTT_Client.connected())
		{
			PRINTLN("Tentative de connexion MQTT...");
			// Tentative de connexion
			if (canusemqttsecure && MQTT_Client.connect(NOMMODULE, MQTT_LOGIN, MQTT_PASSWORD))
			{
				PRINTLN("MQTT connecté avec login/password");
				MQTT_Client.subscribe(TOPICOUT);
				PRINTLN("MQTT Ready , wait in/out message");
			}
			else if (!canusemqttsecure && MQTT_Client.connect(NOMMODULE))
			{
				PRINTLN("MQTT connecté sans login/password");
				MQTT_Client.subscribe(TOPICOUT);
				PRINTLN("MQTT Ready , wait in/out message");
			}
			else
			{
				Serial.print("Erreur, rc=");
				Serial.print(MQTT_Client.state());
				Serial.println(" prochaine tentative dans 2s");
				delay(2000);
			}
			if (!WiFi.isConnected())
				break;
		}
	}
#endif // USE_MQTT

#endif
}

void callbackMQTT(char *topic, byte *message, unsigned int length)
{
}
#endif //USE_MQTT

// ENVOI DES DATAS.
// ***************
void SendData(String state)
{
#ifdef USE_WIFI
	if (WiFi.isConnected())
	{
#ifdef USE_MQTT
		if (canusemqtt && MQTT_Client.connected())
		{
			String messJson;
			if (creatJsonMEssage(state, messJson))
			{
				MQTT_Client.publish(TOPICOUT, messJson.c_str());
				PRINTLN("Message envoyé à Domoticz en MQTT");
			}
		}
#else
		if (0 == 1)
		{
		}
#endif // USE_MQTT

#ifdef USE_HTTP
		else if (canusehttp)
		{
			String url = "http://";
			url += DOMOTIC_SERVER;
			url += ":";
			url += DOMOTIC_PORT;
			url += "/json.htm?type=command&param=switchlight&idx=";
			url += IDXDEVICE;
			url += "&switchcmd=";
			url += state.c_str();
			PRINTLN("URL: ");
			PRINTLN(url.c_str());
			httpClient.begin(espClient, url.c_str());
			int httpCode = httpClient.GET();
			if (httpCode > 0)
			{
				String payload = httpClient.getString();
				Serial.println(payload);
			}
			httpClient.end();
			PRINTLN("Message envoyé à Domoticz en HTTP");
		}
#endif // USE_HTTP
		else
		{
			PRINTLN("MQTT non connecté et Paramatres HTTP invalide, impossible d'envoyer le message");
		}
	}
	else
	{
		PRINTLN("Wifi non connecté , impossible d'envoyer le message");
	}
#else  // USE_WIFi
	PRINTLN("Compiler sans prise en charge du wifi , impossible d'envoyer le message");
#endif // USE_WIFI
}


void setup()
{

	Serial.begin(115200L);
	delay(200);

	pinMode(RELAY_PIN, OUTPUT);
	pinMode(BUTTON_PIN, INPUT_PULLUP);

	digitalWrite(RELAY_PIN, HIGH);

	attachInterrupt(BUTTON_PIN, buttonpressed, FALLING);

#ifdef USE_WIFI
	CheckConfig();

	WiFi.mode(WIFI_STA);
	WiFi.config(ip, gateway, subnet, dns);
	WiFi.setHostname(NOMMODULE);
	WiFi.begin(SSID, SSID_PASSWORD);

	static WiFiEventHandler onConnectedHandler = WiFi.onStationModeConnected(onConnected);
	static WiFiEventHandler onGotIPHandler = WiFi.onStationModeGotIP(onGotIP);
	static WiFiEventHandler onDisConnectedHandler = WiFi.onStationModeDisconnected(onDisconnected);

#ifdef USE_OTA
	ArduinoOTA.setHostname(NOMMODULE);
	// ArduinoOTA.setPassword((const char *)"123");
	ArduinoOTA.onStart([](){ Serial.println("Start"); });
	ArduinoOTA.onEnd([](){ Serial.println("\nEnd"); });
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){ Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
	ArduinoOTA.onError([](ota_error_t error){
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
	ArduinoOTA.begin();
#endif //USE_OTA

#ifdef USE_WEBSERVER
	webServer.on("/switchOn", switchOn);
	webServer.on("/switchOff", switchOff);
	webServer.begin();
#endif //USEWEBSERVER

#endif //USE_WIFI
}

void loop()
{

#ifdef USE_WIFI
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillisMQTT >= intervalConnectMQTT)
	{
		// Si l'objet est connecté au réseau, on effectue les tâches qui doivent l'être dans ce cas
		if (WiFi.isConnected())
		{	
#ifdef USE_MQTT
			if (canusemqtt)
			{
				if (!MQTT_Client.connected())
				{
					PRINTLN("MQTT déconnecté, on reconnecte !");
					reconnectMQTT();
				}
				else
				{
					Serial.print(".");
					MQTT_Client.loop();
				}
			}
#endif // USE_MQTT
		}
		else if (haveWifi)
		{
			PRINTLN("wifi error");
			if (WiFi.waitForConnectResult() != WL_CONNECTED)
				return;
#ifdef USE_OTA
			ArduinoOTA.begin();
#endif //USE_OTA
		}
		previousMillisMQTT = currentMillis;
	}
#endif
	if (button_count >= 5)
	{
		button_count = 0;
		dingdong(true);
		PRINTLN("button pressed");
	}

#ifdef USE_WIFI
#ifdef USE_WEBSERVER
	webServer.handleClient();
#endif //USE_WEBSERVER
#ifdef USE_OTA
	ArduinoOTA.handle();
#endif //USE_OTA
#endif

} //end loop

#ifdef USE_WIFI

void onConnected(const WiFiEventStationModeConnected &event)
{
	PRINTLN("WiFi connecté");
	PRINTLN("Adresse IP : " + WiFi.localIP().toString());
}

void onDisconnected(const WiFiEventStationModeDisconnected &event)
{
	PRINTLN("WiFi Déconnecté");
	PRINTLN("Raison : ");
	PRINTLN(event.reason);
}

void onGotIP(const WiFiEventStationModeGotIP &event)
{
	PRINTLN("Adresse IP : " + WiFi.localIP().toString());
	PRINTLN("Passerelle IP : " + WiFi.gatewayIP().toString());
	PRINTLN("DNS IP : " + WiFi.dnsIP().toString());
	PRINTLN("Puissance de réception : ");
	PRINTLN(WiFi.RSSI());
}

#endif //USE_WIFI
