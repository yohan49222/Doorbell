#ifndef IOB_IOTOTA_H
#define IOB_IOTOTA_H

#include "IOB_IOT/IotConfig.h"

class IOB_IOTOTA : private IotConfig
{
private:
     using IotConfig::getConfigOta;

public:
     IOB_IOTOTA()
     {
          ArduinoOTA.setHostname(getConfigOta().name.c_str());
          if (getConfigOta().password != emptyString)
               ArduinoOTA.setPassword(getConfigOta().password.c_str());

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
};
#endif