#ifndef IOB_IOTHTTP_H
#define IOB_IOTHTTP_H

#include <ESP8266HTTPClient.h>
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"
#include "IOB_IOT/SHARE/Helpers.h"
class IOB_IOT;
class IOB_IOTHTTP;

class IOB_IOTDomotic : Helper
{
private:
     IPAddress ip = INADDR_NONE;
     uint32_t port = 8080;

public:
     friend IOB_IOTHTTP;
     IOB_IOTDomotic()
     {
#ifdef DOMOTIC_SERVER
          ip = IPAddress::isValid((String)DOMOTIC_SERVER) ? ParsedIpFromString((String)DOMOTIC_SERVER) : INADDR_NONE;
#endif

#ifdef DOMOTIC_PORT
          port = DefinedInt(DOMOTIC_PORT) ? DOMOTIC_PORT : 8080;
#endif
     }
};

class IOB_IOTHTTP : IOB_IOTDomotic
{
private:
     HTTPClient httpClient;

public:
     IOB_IOTHTTP();
     IOB_IOTMessageSendedventHandler http_Send_EventHandler;
     bool CreateHttpMessageForDomoticz(RelayState state, String &out);
     bool Sendata(RelayState state);
     IPAddress GetHttpIp();
};
#endif
