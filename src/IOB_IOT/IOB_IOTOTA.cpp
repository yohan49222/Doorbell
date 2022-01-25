#include "IOB_IOT/IOB_IOTOTA.h"
#include "IOB_IOT.h"

#ifdef USE_OTA

IOB_IOTOTA::IOB_IOTOTA() : IOB_IOTOTAConfig()
{
     if(name == emptyString)
     {
          Required req = ((IOB_IOT*)this)->getRequired();
          ArduinoOTA.setHostname(req.nomModule.c_str());
     }
          
     else
          ArduinoOTA.setHostname(name.c_str());

     if (password != emptyString)
          ArduinoOTA.setPassword(password.c_str());

     ArduinoOTA.onStart([]()
                        { Serial.println("Start OTA UPDATE"); });

     ArduinoOTA.onEnd([]()
                      { Serial.println("\nEnd OTA UPDATE , Reboot... "); });

     ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                           { Serial.printf("Progression : %u%%\r", (progress / (total / 100))); });

     ArduinoOTA.onError([](ota_error_t error)
                        {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
     ArduinoOTA.begin();
}

/*
void IOB_IOTOTA::init(IOB_IOT *iob)
{
     if(name == emptyString)
          ArduinoOTA.setHostname(iob->getRequired().nomModule.c_str());
     else
          ArduinoOTA.setHostname(name.c_str());

     if (password != emptyString)
          ArduinoOTA.setPassword(password.c_str());

     ArduinoOTA.onStart([]()
                        { Serial.println("Start OTA UPDATE"); });

     ArduinoOTA.onEnd([]()
                      { Serial.println("\nEnd OTA UPDATE , Reboot... "); });

     ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                           { Serial.printf("Progression : %u%%\r", (progress / (total / 100))); });

     ArduinoOTA.onError([](ota_error_t error)
                        {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
     ArduinoOTA.begin();
}
*/
#endif
