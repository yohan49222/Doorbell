#ifndef IOTCONFIG_H
#define IOTCONFIG_H

#ifndef IOB_IOT_CONF_H
#include "IOB_IOT_Conf.h"
#endif /* IOB_IOT_CONF_H */

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