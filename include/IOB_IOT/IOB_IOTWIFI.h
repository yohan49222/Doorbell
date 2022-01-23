#ifndef IOB_IOTWIFI_H
#define IOB_IOTWIFI_H

#include <ESP8266WiFi.h>
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"

class IOB_IOT;

class IOB_IOTWIFI
{
private:
     

     static void OnConnected(const WiFiEventStationModeConnected &event);
     static void OnDisconnected(const WiFiEventStationModeDisconnected &event);
     static void OnGotIP(const WiFiEventStationModeGotIP &event);

     WiFiEventHandler onConnectedHandler;
     WiFiEventHandler onGotIPHandler;
     WiFiEventHandler onDisConnectedHandler;


public:
     void Begin(IOB_IOT *iob);
     bool Loop(IOB_IOT *iob);
     WiFiClient espClient;
     IOB_IOTWifiStateEventHandler wifiStateChangedEventHandler;

};

#endif