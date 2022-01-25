#include "IOB_IOT/IOB_IOTMQTT.h"
#include "IOB_IOT.h"

#ifdef USE_MQTT

IOB_IOTMQTT::IOB_IOTMQTT() : IOB_IOTMqttConfig()
{
     IOB_IOT *iob = ((IOB_IOT*)this);
     if (!CanUseMqtt())
          return;

     MQTT_Client.setBufferSize(512);
     MQTT_Client.setServer(ip, port);
     MQTT_Client.setClient(iob->espClient);
     MQTT_Client.setCallback(IOB_IOTMQTT::CallbackMQTT);
}
IPAddress IOB_IOTMQTT::GetMqttIp()
{
     return ip;
}

String IOB_IOTMQTT::GetTopicIn()
{
     return topicIn;
}

bool IOB_IOTMQTT::CanSendMqtt()
{
     return CanUseMqtt() && MQTT_Client.connected();
}

bool IOB_IOTMQTT::CanUseMqtt()
{
     Required req = ((IOB_IOT*)this)->getRequired();
     return ip.isSet() && req.idxDevice > 0;
}

bool IOB_IOTMQTT::CanUseMqttSecure()
{
     return login != emptyString && password != emptyString && CanUseMqtt();
}

void IOB_IOTMQTT::ReconnectMQTT()
{
     Required req = ((IOB_IOT*)this)->getRequired();
     IOB_IOTMqttStateChangedEventArgs e = IOB_IOTMqttStateChangedEventArgs();
     if (CanUseMqttSecure() && MQTT_Client.connect(req.nomModule.c_str(), login.c_str(), password.c_str()))
     {
          e.State(ConState::CONNECTED);
          countEchec = 0;
          setintervalConnect(intervalConnectSave);
          e.AddMessage("connecté avec login/password");
          MQTT_Client.subscribe(topicIn.c_str());
          e.AddMessage("Ready , wait in/out message");
     }
     else if (CanUseMqtt() && MQTT_Client.connect(req.nomModule.c_str()))
     {
          e.State(ConState::CONNECTED);
          countEchec = 0;
          setintervalConnect(intervalConnectSave);
          e.AddMessage("Connecté sans login/password");
          MQTT_Client.subscribe(topicIn.c_str());
          e.AddMessage("Ready , wait in/out message");
     }
     else
     {
          countEchec++;

          if(countEchec == 5)
          {
               intervalConnectSave = intervalConnect;
               setintervalConnect(10000);
          }
                

          e.AddMessage("Erreur, rc= " + String(MQTT_Client.state()));
          e.AddMessage("Prochaine tentative dans (ms) : " + String(intervalConnect));
     }
     mqtt_State_Changed_EventHandler.fire(e);
}

bool IOB_IOTMQTT::Sendata(RelayState state)
{

     Required req = ((IOB_IOT*)this)->getRequired();
     bool sendSuccess = false;

     if(!CanSendMqtt())
          return false;

     String messJson;
     if (((IOB_IOT*)this)->CreateJsonMessageForDomoticz(state, messJson))
     {
          IOB_IOTMessageSendedEventArgs e = IOB_IOTMessageSendedEventArgs(req.idxDevice, state, SendProtole::MQTT, messJson);
          sendSuccess = MQTT_Client.publish(topicOut.c_str(), messJson.c_str());
          e.AddMessage("Message envoyé à Domoticz en MQTT");
          mqtt_Send_EventHandler.fire(e);
     }
     return sendSuccess;
}

void IOB_IOTMQTT::Loop()
{
     unsigned long currentMillis = millis();
     if (!CanUseMqtt())
          return;

     if (!MQTT_Client.connected() && ((currentMillis - previousMillis) >= intervalConnect))
     {
          ReconnectMQTT();
          setPreviousMillis(currentMillis);
     }
     else if (MQTT_Client.connected())
     {
          MQTT_Client.loop();
          if ((currentMillis - previousMillis) >= intervalConnect)
          {
               setPreviousMillis(currentMillis);
          }
     }
}

/*
 * static methodes for CallBack PubSubClient
*/
void IOB_IOTMQTT::ParseMqttMessage(IOB_IOT *iob, char *topic, byte *message, unsigned int length)
{
     int valeur = strcmp(topic, iob->GetTopicIn().c_str());
     if (valeur != 0)
          return;

     String string;
     for (unsigned int i = 0; i < length; i++)
     {
          string += ((char)message[i]);
     }

     StaticJsonBuffer<512> jsonBuffer;
     JsonObject &root = jsonBuffer.parseObject(string);
     if (!root.success())
          return;

     uint32_t idx = iob->getRequired().idxDevice;

     int id = root["idx"];
     int nvalue = root["nvalue"];
     if (idx != (uint32_t)id)
          return;

     RelayState state = RelayStateConverter::fromInt(nvalue);
     IOB_IOTMessageRecevedEventArgs e = IOB_IOTMessageRecevedEventArgs(id, state, SendProtole::MQTT);
     e.AddMessage("Message arrivé [topic] : " + String(topic));
     e.AddMessage("Recep : " + (String)RelayStateConverter::toString(state).c_str() );
     iob->mqtt_Recep_EventHandler.fire(e);
     if (!e.Handled())
     {
          uint32_t rpin = iob->getRequired().relayPin;
          if (nvalue == 1)
          {
               digitalWrite(rpin, iob->getRequired().relayNcOrNo ? LOW : HIGH);
          }
          else
          {
               digitalWrite(rpin, iob->getRequired().relayNcOrNo ? HIGH : LOW);
          }
     }
}

void IOB_IOTMQTT::CallbackMQTT(char *topic, byte *message, unsigned int length)
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     iob->ParseMqttMessage(iob, topic, message, length);
}

#endif