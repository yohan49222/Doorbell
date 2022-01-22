#include "IOB_IOT/IOB_IOTHTTP.h"
#include "IOB_IOT.h"

#ifdef USE_HTTP
bool IOB_IOTHTTP::CreateHttpMessageForDomoticz(IOB_IOT *iob, RelayState state, String &out)
{
     String url = "http://";
     url += iob->getDomotic().ip.toString();
     url += ":";
     url += iob->getDomotic().port;
     url += "/json.htm?type=command&param=switchlight&idx=";
     url += iob->getRequired().idxDevice;
     url += "&switchcmd=";
     url += RelayStateConverter::toString(state).c_str();
     out = url;
     return true;
}

bool IOB_IOTHTTP::Sendata(IOB_IOT *iob, RelayState state, WiFiClient &espClient)
{
     bool sendSuccess = false;
     String url;

     if (CreateHttpMessageForDomoticz(iob, state, url))
     {
          IOB_IOTMessageSendedEventArgs e = IOB_IOTMessageSendedEventArgs(iob->getRequired().idxDevice, state, SendProtole::HTTP, url);
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
#endif