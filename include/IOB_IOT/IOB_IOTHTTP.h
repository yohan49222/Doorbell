#ifndef IOB_IOTHTTP_H
#define IOB_IOTHTTP_H
#include "IOB_IOT/IotConfig.h"
#include "IOB_IOTEvent.h"

class IOB_IOTHTTP : protected IotConfig
{
private:
     using IotConfig::getRequired;
     HTTPClient httpClient;

protected:
     using IotConfig::getDomotic;

public:
     IOB_IOTMessageSendedventHandler http_Send_EventHandler;
     bool CreateHttpMessageForDomoticz(RelayState state, String &out)
     {
          String url = "http://";
          url += getDomotic().ip.toString();
          url += ":";
          url += getDomotic().port;
          url += "/json.htm?type=command&param=switchlight&idx=";
          url += getRequired().idxDevice;
          url += "&switchcmd=";
          url += RelayStateConverter::toString(state).c_str();
          out = url;
          return true;
     }

     bool Sendata(RelayState state, WiFiClient &espClient)
     {
          bool sendSuccess = false;

          String url;

          if (CreateHttpMessageForDomoticz(state, url))
          {
               IOB_IOTMessageSendedEventArgs e = IOB_IOTMessageSendedEventArgs(getRequired().idxDevice, state, SendProtole::HTTP, url);
               httpClient.begin(espClient, url.c_str());
               int httpCode = 0;
               httpCode = httpClient.GET();
               if (httpCode > 0)
               {
                    String payload = httpClient.getString();
                    if (httpCode == 200)
                    {
                         sendSuccess = true;
                         e.AddMessage("Message envoyé à Domoticz en HTTP");
                    }
                    else
                    {
                         e.AddMessage("Error : " + url);
                    }
                    http_Send_EventHandler.fire(e);
               }
               httpClient.end();
          }
          return sendSuccess;
     }
};
#endif
