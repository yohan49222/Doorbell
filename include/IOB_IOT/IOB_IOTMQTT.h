#ifndef IOB_IOTMQTT_H
#define IOB_IOTMQTT_H

#include <PubSubClient.h>
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"

class IOB_IOT;


class IOB_IOTMQTT
{
private:
     PubSubClient MQTT_Client;
     uint32_t countEchec = 0;
     unsigned long intervalConnectSave = 1000;
     bool CanSendMqtt(IOB_IOT *iob);
     bool CanUseMqtt(IOB_IOT *iob);
     bool CanUseMqttSecure(IOB_IOT *iob);
     void ReconnectMQTT(IOB_IOT *iob);

public:
     void init(IOB_IOT *iob);
     void LoopMqtt(IOB_IOT *iob);     
     bool Sendata(IOB_IOT *iob, RelayState state);

     static void ParseMqttMessage(IOB_IOT *iob, char *topic, byte *message, unsigned int length);
     static void CallbackMQTT(char *topic, byte *message, unsigned int length);

     IOB_IOTMqttStateEventHandler mqtt_State_Changed_EventHandler;
     IOB_IOTMessageRecevedEventHandler mqtt_Recep_EventHandler;
     IOB_IOTMessageSendedventHandler mqtt_Send_EventHandler;

     friend IOB_IOT;
};
#endif