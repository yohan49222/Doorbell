#ifndef IOTCONFIG_H
#define IOTCONFIG_H

#ifndef IOB_IOT_CONF_H
#include "IOB_IOT_Conf.h"
#endif /* IOB_IOT_CONF_H */

#include "IOB_IOT/SHARE/Helpers.h"

#ifdef USE_WIFI

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

#endif

struct Required
{
     String nomModule = emptyString;
     uint32_t idxDevice = 0;
     uint32_t relayPin = 0;
     uint32_t buttonPin = 2;
     bool relayNcOrNo = true;
     uint32_t debounceTime = 0;
     uint32_t buttonPresseCountMax = 0;
     uint32_t relayAutoOff = 1;
     unsigned long relayAutoOffAfter = 200;
};

class IotConfig : private Helper
{
private:
     using Helper::DefinedInt;
     using Helper::DefinedString;
     using Helper::EqualString;
     using Helper::GenerateRamdomModuleNane;
     using Helper::ParsedIpFromString;
     Required required;

#ifdef USE_WIFI
     WifiConfig configWifi;
#ifdef USE_IPFIXE
     IpConfig configIp;
#endif
#ifdef USE_OTA
     OtaConfig configOta;
#endif
#ifdef USE_MQTT
     Mqtt mqtt;
#endif
#ifdef USE_HTTP
     Domotic domo;
#endif
#ifdef USE_WEBSERVER
     uint32_t webServerPort = 80;
#endif
#endif

public:
     IotConfig();
     Required getRequired();

#ifdef USE_WIFI
     WifiConfig getConfigWifi();
#ifdef USE_IPFIXE
     IpConfig getConfigIp();
#endif
#ifdef USE_OTA
     OtaConfig getConfigOta();
#endif
#ifdef USE_MQTT
     void setPreviousMillis(unsigned long i);
     void setintervalConnect(unsigned long i);
     Mqtt getMqtt();
#endif
#ifdef USE_HTTP
     Domotic getDomotic();
#endif
#ifdef USE_WEBSERVER
     uint32_t getWebServerPort();
#endif
#endif
};
#endif