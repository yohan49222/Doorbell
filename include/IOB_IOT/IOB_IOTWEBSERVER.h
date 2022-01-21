#ifndef IOB_IOTWEBSERVER_H
#define IOB_IOTWEBSERVER_H

#include <ESP8266WebServer.h>
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"
#include <ArduinoJson.h>
class IOB_IOTWEBSERVER
{
private:
     static void SwitchOn();
     static void SwitchOff();
     static void TraitRequestWeb(RelayState state);
     static void DebugServeur();

public: 
     ESP8266WebServer webServer;
     void init();
     void Loop();
     bool CreateJsonMessageForDebug(String &out);
     IOB_IOTMessageRecevedEventHandler webServer_Request_EventHandler;
     IOB_IOTMessageSendedventHandler  webServer_Response_EventHandler;
};

#endif