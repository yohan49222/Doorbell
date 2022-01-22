#ifndef IOB_IOTWEBSERVER_H
#define IOB_IOTWEBSERVER_H

#include <ESP8266WebServer.h>
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"
#include <ArduinoJson.h>
class IOB_IOT;

class IOB_IOTWEBSERVER
{
private:
     static void SwitchOn();
     static void SwitchOff();
     static void TraitRequestWeb(IOB_IOT *iob, RelayState state);
     static void DebugServeur();

public: 
     ESP8266WebServer webServer;
     void init(IOB_IOT *iob);
     void Loop(IOB_IOT *iob);
     bool CreateJsonMessageForDebug(IOB_IOT *iob, String &out);
     IOB_IOTMessageRecevedEventHandler webServer_Request_EventHandler;
     IOB_IOTMessageSendedventHandler  webServer_Response_EventHandler;
};

#endif