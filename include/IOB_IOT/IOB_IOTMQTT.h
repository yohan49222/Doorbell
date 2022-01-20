#ifndef IOB_IOTMQTT_H
#define IOB_IOTMQTT_H

//#ifndef PubSubClient_h
//#define PubSubClient_h
#include <PubSubClient.h>
//#endif
#include <ArduinoJson.h>
#include "IOB_IOT/IotConfig.h"
#include "IOB_IOTEvent.h"

class IOB_IOTMQTT : protected IotConfig
{
protected:
     using IotConfig::get;
     
     
     

private:
     PubSubClient MQTT_Client;
     using IotConfig::setPreviousMillis;
     using IotConfig::getRequired;
     using IotConfig::getMqtt;

public:
     void init();
     bool CanSendMqtt();
     bool CanUseMqtt();
     bool CanUseMqttSecure();
     void ReconnectMQTT();
     static void ParseMqttMessage(char *topic, byte *message, unsigned int length, IotConfig conf);
     bool CreateJsonMessageForDomoticz(RelayState state, String &out);
     static void CallbackMQTT(char *topic, byte *message, unsigned int length);
     IOB_IOTMqttStateEventHandler mqtt_State_Changed_EventHandler;
     IOB_IOTMessageRecevedEventHandler mqtt_Recep_EventHandler;
     IOB_IOTMessageSendedventHandler mqtt_Send_EventHandler;
     bool Sendata(RelayState state);
     void LoopMqtt();
};
#endif