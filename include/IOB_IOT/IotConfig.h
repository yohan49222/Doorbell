#ifndef IOTCONFIG_H
#define IOTCONFIG_H

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

#include "IOB_IOT/Helpers.h"

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

struct Mqtt
{
     String topicIn = emptyString;
     String topicOut = emptyString;
     IPAddress ip = INADDR_NONE;
     uint32_t port = 1883;
     String login = emptyString;
     String password = emptyString;
     unsigned long previousMillis = 0;
     unsigned long intervalConnect = 1000;
};

struct Domotic
{
     IPAddress ip = INADDR_NONE;
     uint32_t port = 8080;
};

struct Required
{
     String nomModule = emptyString;
     uint32_t idxDevice = 0;
     uint32_t relayPin = 0;
     uint32_t buttonPin = 2;
     bool relayNcOrNo = true;
     uint32_t debounceTime = 0;
     uint32_t buttonPresseCountMax = 0;
};

class IotConfig : private Helper
{
private:
     using Helper::DefinedInt;
     using Helper::DefinedString;
     using Helper::EqualString;
     using Helper::GenerateRamdomModuleNane;
     using Helper::ParsedIpFromString;
     IpConfig configIp;
     WifiConfig configWifi;
     OtaConfig configOta;
     Required required;
     Mqtt mqtt;
     Domotic domo;
     uint32_t webServerPort = 80;

public:
     IotConfig();
     IotConfig get();
     void setPreviousMillis(unsigned long i);
     IpConfig getConfigIp();
     WifiConfig getConfigWifi();
     Domotic getDomotic();
     OtaConfig getConfigOta();
     uint32_t getWebServerPort();
     Mqtt getMqtt();
     Required getRequired();
};
#endif