#include "IOB_IOT/IOB_IOTHTTP.h"
#include "IOB_IOT.h"

#ifdef USE_HTTP

IOB_IOTHTTP::IOB_IOTHTTP() : IOB_IOTDomotic()
{
}

IPAddress IOB_IOTHTTP::GetHttpIp()
{
     return ip;
}

bool IOB_IOTHTTP::CreateHttpMessageForDomoticz(RelayState state, String &out)
{
     Required req = ((IOB_IOT*)this)->getRequired();
     String url = "http://";
     url += ip.toString();
     url += ":";
     url += port;
     url += "/json.htm?type=command&param=switchlight&idx=";
     url += req.idxDevice;
     url += "&switchcmd=";
     url += RelayStateConverter::toString(state).c_str();
     out = url;
     return true;
}

bool IOB_IOTHTTP::Sendata(RelayState state)
{
     bool sendSuccess = false;
     Required req = ((IOB_IOT*)this)->getRequired();
     if(!ip.isSet() || req.idxDevice == 0)
          return false;

     String url;
     if (CreateHttpMessageForDomoticz(state, url))
     {
          IOB_IOTMessageSendedEventArgs e = IOB_IOTMessageSendedEventArgs(req.idxDevice, state, SendProtole::HTTP, url);
          WiFiClient espClient = ((IOB_IOT*)this)->espClient;
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