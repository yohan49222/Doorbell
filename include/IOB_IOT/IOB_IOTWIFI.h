#ifndef IOB_IOTWIFI_H
#define IOB_IOTWIFI_H

#include <ESP8266WiFi.h>
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"
#include "IOB_IOT/SHARE/Helpers.h"
class IOB_IOT;
class IOB_IOTWIFI;

class IOB_IOTIpConfig : Helper
{
private:
     IPAddress ip = INADDR_NONE;
     IPAddress gateWay = INADDR_NONE;
     IPAddress subnet = INADDR_NONE;
     IPAddress dns = INADDR_NONE;

public:
     IOB_IOTIpConfig()
     {
#ifdef IPFIXE
     ip = IPAddress::isValid((String)IPFIXE) ? ParsedIpFromString((String)IPFIXE) : INADDR_NONE;
#endif

#ifdef GATEWAY
     gateWay = IPAddress::isValid((String)GATEWAY) ? ParsedIpFromString((String)GATEWAY) : INADDR_NONE;
#endif

#ifdef SUBNET
     subnet = IPAddress::isValid((String)SUBNET) ? ParsedIpFromString((String)SUBNET) : INADDR_NONE;
#endif

#ifdef DNS
     dns = IPAddress::isValid((String)DNS) ? ParsedIpFromString((String)DNS) : INADDR_NONE;
#endif
     }
     friend IOB_IOTWIFI;
};

class IOB_IOTWIFIConfig : Helper
{
private:
     String ssid = emptyString;
     String password = emptyString;
public:
     IOB_IOTWIFIConfig()
     {
#ifdef MYSSID
     ssid = DefinedString((String)MYSSID) && !EqualString((String)MYSSID, "my_ssid") ? (String)MYSSID : emptyString;
#endif

#ifdef MYSSID_PASSWORD
     password = DefinedString((String)MYSSID_PASSWORD) && !EqualString((String)MYSSID_PASSWORD, "my_ssid_password") ? (String)MYSSID_PASSWORD : emptyString;
#endif
     }
     friend IOB_IOTWIFI;
};

class IOB_IOTWIFI : IOB_IOTWIFIConfig, IOB_IOTIpConfig
{
private:
     static void OnConnected(const WiFiEventStationModeConnected &event);
     static void OnDisconnected(const WiFiEventStationModeDisconnected &event);
     static void OnGotIP(const WiFiEventStationModeGotIP &event);

     WiFiEventHandler onConnectedHandler;
     WiFiEventHandler onGotIPHandler;
     WiFiEventHandler onDisConnectedHandler;


public:
     IOB_IOTWIFI();
     bool Loop();
     WiFiClient espClient;
     IOB_IOTWifiStateEventHandler wifiStateChangedEventHandler;

     friend IOB_IOT;
};

#endif