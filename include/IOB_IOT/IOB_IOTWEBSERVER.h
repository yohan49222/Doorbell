#ifndef IOB_IOTWEBSERVER_H
#define IOB_IOTWEBSERVER_H

#include <ESP8266WebServer.h>
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"
#include <ArduinoJson.h>
#include "IOB_IOT/SHARE/Helpers.h"
class IOB_IOT;

class IOB_IOTWEBSERVER : Helper
{
private:
     uint32_t webServerPort = 80;
     static void SwitchOn();
     static void SwitchOff();
     static void TraitRequestWeb(IOB_IOT *iob, RelayState state);
     static void DebugServeur();
     ESP8266WebServer webServer;

public:
     IOB_IOTWEBSERVER();

     void Loop();
     bool CreateJsonMessageForDebug(String &out);
     IOB_IOTMessageRecevedEventHandler webServer_Request_EventHandler;
     IOB_IOTMessageSendedventHandler  webServer_Response_EventHandler;

     friend IOB_IOT;
};

#endif