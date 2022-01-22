#ifndef IOB_IOTHTTP_H
#define IOB_IOTHTTP_H

#include <ESP8266HTTPClient.h>
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"

class IOB_IOT;

class IOB_IOTHTTP
{
private:
     HTTPClient httpClient;

public:
     IOB_IOTMessageSendedventHandler http_Send_EventHandler;
     bool CreateHttpMessageForDomoticz(IOB_IOT *iob, RelayState state, String &out);
     bool Sendata(IOB_IOT *iob, RelayState state, WiFiClient &espClient);
};
#endif
