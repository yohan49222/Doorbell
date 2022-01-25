#ifndef IOB_IOTMQTT_H
#define IOB_IOTMQTT_H

#include <PubSubClient.h>
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"
#include "IOB_IOT/SHARE/Helpers.h"

class IOB_IOT;
class IOB_IOTMQTT;
class IOB_IOTMqttConfig : Helper
{
private:
     
     String topicOut = emptyString;
     IPAddress ip = INADDR_NONE;
     String topicIn = emptyString;
     uint32_t port = 1883;
     String login = emptyString;
     String password = emptyString;
     unsigned long previousMillis = 0;
     unsigned long intervalConnect = 1000;

public:
     friend IOB_IOTMQTT;
     IOB_IOTMqttConfig()
     {
#ifdef MQTT_SERVER
          ip = IPAddress::isValid((String)MQTT_SERVER) ? ParsedIpFromString((String)MQTT_SERVER) : INADDR_NONE;
#endif

#ifdef MQTT_PORT
          port = DefinedInt(MQTT_PORT) ? MQTT_PORT : 1883;
#endif

#ifdef TOPICIN
          topicIn = DefinedString((String)TOPICIN) ? (String)TOPICIN : "domoticz/out";
#endif

#ifdef TOPICOUT
          topicOut = DefinedString((String)TOPICOUT) ? ((String)TOPICOUT) : "domoticz/in";
#endif

#ifdef MQTT_LOGIN
          login = DefinedString((String)MQTT_LOGIN) && !EqualString((String)MQTT_LOGIN, "login") ? (String)MQTT_LOGIN : emptyString;

#endif

#ifdef MQTT_PASSWORD
          password = DefinedString((String)MQTT_PASSWORD) && !EqualString((String)MQTT_PASSWORD, "password") ? (String)MQTT_PASSWORD : emptyString;
#endif

#ifdef INTERVALCONNECT
          intervalConnect = DefinedInt(INTERVALCONNECT) ? INTERVALCONNECT : 1000;
#endif
     }
     
     void setintervalConnect(unsigned long i)
     {
          intervalConnect = i;
     }
     void setPreviousMillis(unsigned long i)
     {
          previousMillis = i;
     }
};

class IOB_IOTMQTT : public IOB_IOTMqttConfig
{
private:
     PubSubClient MQTT_Client;
     uint32_t countEchec = 0;
     unsigned long intervalConnectSave = 1000;
     bool CanSendMqtt();
     bool CanUseMqtt();
     bool CanUseMqttSecure();
     void ReconnectMQTT();

public:
     IOB_IOTMQTT();
     void Loop();
     bool Sendata(RelayState state);
     IPAddress GetMqttIp();
     String GetTopicIn();
     static void ParseMqttMessage(IOB_IOT *iob, char *topic, byte *message, unsigned int length);
     static void CallbackMQTT(char *topic, byte *message, unsigned int length);

     IOB_IOTMqttStateEventHandler mqtt_State_Changed_EventHandler;
     IOB_IOTMessageRecevedEventHandler mqtt_Recep_EventHandler;
     IOB_IOTMessageSendedventHandler mqtt_Send_EventHandler;

     friend IOB_IOT;
};
#endif