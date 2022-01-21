#ifndef IOB_IOTMQTT_H
#define IOB_IOTMQTT_H

#include <PubSubClient.h>

#include "IOB_IOTEvent.h"

class IOB_IOTMQTT
{
private:
     PubSubClient MQTT_Client;

public:
     void init();
     bool CanSendMqtt();
     bool CanUseMqtt();
     bool CanUseMqttSecure();
     void ReconnectMQTT();
     static void ParseMqttMessage(char *topic, byte *message, unsigned int length);
     
     static void CallbackMQTT(char *topic, byte *message, unsigned int length);
     IOB_IOTMqttStateEventHandler mqtt_State_Changed_EventHandler;
     IOB_IOTMessageRecevedEventHandler mqtt_Recep_EventHandler;
     IOB_IOTMessageSendedventHandler mqtt_Send_EventHandler;
     bool Sendata(RelayState state);
     void LoopMqtt();
};
#endif