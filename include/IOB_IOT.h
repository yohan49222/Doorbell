#ifndef IOB_IOT_H
#define IOB_IOT_H

#ifndef IOB_IOT_CONF_H
#include "IOB_IOT_Conf.h"
#endif /* IOB_IOT_CONF_H */

#ifdef USE_WIFI

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#ifdef USE_WEBSERVER
#include <ESP8266WebServer.h>
#endif /* USE_WEBSERVER */
#ifdef USE_HTTP
#include <ESP8266HTTPClient.h>
#endif
#ifdef USE_MQTT
#include <PubSubClient.h>
#endif /* USE_MQTT */

#else
#ifndef Arduino_h
#include "Arduino.h"
#include "IPAddress.h"
#endif
#endif /* USE_WIFI */

#ifndef IOB_IOTEVENT_H
#include "IOB_IOTEvent.h"
#endif /* IOB_IOTEVENT_H */

#ifndef IOB_IOTBUTTONPRESSED_H
#include "IOB_IOTButtonPressed.h"
#endif /* IOB_IOTBUTTONPRESSED_H */

struct IpConfig
{
     IPAddress ip = INADDR_NONE;
     IPAddress gateWay = INADDR_NONE;
     IPAddress subnet = INADDR_NONE;
     IPAddress dns = INADDR_NONE;
};

struct OtaConfig
{
     String name = emptyString;
     String password = emptyString;
};
struct WifiConfig
{
     String ssid = emptyString;
     String password = emptyString;
};
struct MqttConfig
{
     unsigned long previousMillis = 0;
     unsigned long intervalConnect = 1000;
     String topicIn = emptyString;
     String topicOut = emptyString;
     IPAddress ip = INADDR_NONE;
     uint32_t port = 1883;
     String login = emptyString;
     String password = emptyString;
};

struct IotConfig
{
     String nomModule = emptyString;
     uint32_t idxDevice = 0;
     uint32_t relayPin = 0;
     uint32_t buttonPin = 2;
     bool relayNcOrNo = true;
     uint32_t debounceTime = 0;
     uint32_t buttonPresseCountMax = 0;
};

struct DomoConfig
{
     IPAddress ip;
     uint32_t port;
};

class IOB_IOT
{
private:
     static IOB_IOT *inst_; // The one, single instance
     IOB_IOT();
     IOB_IOT(const IOB_IOT &);
     IOB_IOT &operator=(const IOB_IOT &);

#ifdef USE_WIFI
     WifiConfig sConfigWifi;
     IotConfig sConfigIot;
     WiFiClient espClient;

#ifdef USE_IPFIXE
     IpConfig sConfigIp;
#endif /* USE_IPFIXE */

#ifdef USE_OTA
     OtaConfig sConfigOta;
#endif /* USE_OTA */

#ifdef USE_HTTP
     DomoConfig sConfigDomo;
     HTTPClient httpClient;
     bool CreateHttpMessageForDomoticz(RelayState state, String &out);
#endif /* USE HTTP */

#ifdef USE_MQTT
     MqttConfig sConfigMqtt;
     void ReconnectMQTT();
     void ParseMqttMessage(char *topic, byte *message, unsigned int length);
     static void CallbackMQTT(char *topic, byte *message, unsigned int length);
     PubSubClient MQTT_Client;
#endif /* USE_MQTT */

#ifdef USE_WEBSERVER
     ESP8266WebServer webServer;
     unsigned int webServerPort;
     bool CreateJsonMessageForDebug(String &out);
     static void SwitchOn();
     static void SwitchOff();
     static void TraitRequestWeb(RelayState state);
     static void DebugServeur();
#endif /* USE_WEBSERVER */

     IOB_IOTMessageRecevedEventHandler messageRecepEventHandler;
     IOB_IOTMessageSendedventHandler messageSendedEventHandler;
     IOB_IOTMqttStateEventHandler mqttStateChangedEventHandler;
     IOB_IOTWifiStateEventHandler wifiStateChangedEventHandler;

     static void OnConnected(const WiFiEventStationModeConnected &event);
     static void OnDisconnected(const WiFiEventStationModeDisconnected &event);
     static void OnGotIP(const WiFiEventStationModeGotIP &event);

     WiFiEventHandler onConnectedHandler;
     WiFiEventHandler onGotIPHandler;
     WiFiEventHandler onDisConnectedHandler;

     bool SendData(String send, int sendVia, IOB_IOTMessageSendedEventArgs &e);
     bool CreateJsonMessageForDomoticz(RelayState state, String &out);

#endif /* USE_WIFI */

     IOB_IOTButtonPressedEventHandler buttonPressedEventHandler;


     bool EqualString(String stest, String stestto);
     bool DefinedString(String stest);
     bool DefinedInt(const int itest);
     IPAddress ParsedIpFromString(String sip);
     String GenerateRamdomModuleNane();

     volatile uint32_t debounceTimer = 0;

     uint32_t buttonPresseCount = 0;
     static void IRAM_ATTR ButtonPressed();
     void SendData(RelayState state);

public:
     static IOB_IOT *GetInstance();
     ~IOB_IOT();
     void Loop();
     void Run();
     void SendData(String state);

     void OnMessageSend(std::function<void(IOB_IOTMessageSendedEventArgs &)> handler);
     void OnMessageRecep(std::function<void(IOB_IOTMessageRecevedEventArgs &)> handler);
     void OnMqttStateChanged(std::function<void(IOB_IOTMqttStateChangedEventArgs &)> handler);
     void OnWifiStateChanged(std::function<void(IOB_IOTWifiStateChangedEventArgs &)> handler);

     void OnButtonPressed(std::function<void(IOB_IOTButtonPressedEventArgs &)> handler);
};

#define CANUSEWIFI sConfigWifi.ssid != emptyString && sConfigWifi.password != emptyString
#define CANUSEMQTT sConfigMqtt.ip.isSet() && sConfigIot.idxDevice > 0
#define CANUSEMQTTSECURE sConfigMqtt.login != emptyString && sConfigMqtt.password != emptyString && CANUSEMQTT
#define CANSENDVIAMQTT CANUSEMQTT &&WiFi.isConnected() && MQTT_Client.connected()
#define CANUSEHTTP sConfigDomo.ip.isSet() && sConfigIot.idxDevice > 0
#define CANSENDVIAHTTP CANUSEHTTP && WiFi.isConnected()
#define CANUSEOTASECURE sConfigOta.password != emptyString
#define CANUSEIPFIXE sConfigIp.ip.isSet() && sConfigIp.gateWay.isSet() && sConfigIp.subnet.isSet() && sConfigIp.dns.isSet()

#endif /* IOB_IOT_H */