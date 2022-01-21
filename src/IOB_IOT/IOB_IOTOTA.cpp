#include "IOB_IOT/IOB_IOTOTA.h"
#include "IOB_IOT.h"

#ifdef USE_OTA
void IOB_IOTOTA::init()
{
     IOB_IOT *inst = IOB_IOT::GetInstance();

     ArduinoOTA.setHostname(inst->getConfigOta().name.c_str());
     if (inst->getConfigOta().password != emptyString)
          ArduinoOTA.setPassword(inst->getConfigOta().password.c_str());

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
#endif
