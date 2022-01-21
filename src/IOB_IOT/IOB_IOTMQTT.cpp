#include "IOB_IOT/IOB_IOTMQTT.h"
#include "IOB_IOT.h"

#ifdef USE_MQTT

void IOB_IOTMQTT::init()
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     MQTT_Client.setBufferSize(512);
     MQTT_Client.setServer(iob->getMqtt().ip, iob->getMqtt().port);
     MQTT_Client.setClient(iob->espClient);
     MQTT_Client.setCallback(IOB_IOTMQTT::CallbackMQTT);
}

void IOB_IOTMQTT::CallbackMQTT(char *topic, byte *message, unsigned int length)
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     iob->ParseMqttMessage(topic, message, length);
}

bool IOB_IOTMQTT::CanSendMqtt()
{
     return CanUseMqtt() && MQTT_Client.connected();
}

bool IOB_IOTMQTT::CanUseMqtt()
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     return iob->getMqtt().ip.isSet() && iob->getRequired().idxDevice > 0;
}

bool IOB_IOTMQTT::CanUseMqttSecure()
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     return iob->getMqtt().login != emptyString && iob->getMqtt().password != emptyString && CanUseMqtt();
}

void IOB_IOTMQTT::ReconnectMQTT()
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     IOB_IOTMqttStateChangedEventArgs e = IOB_IOTMqttStateChangedEventArgs();
     if (CanUseMqttSecure() && MQTT_Client.connect(iob->getRequired().nomModule.c_str(), iob->getMqtt().login.c_str(), iob->getMqtt().password.c_str()))
     {
          e.State(ConState::CONNECTED);
          e.AddMessage("connecté avec login/password");
          MQTT_Client.subscribe(iob->getMqtt().topicIn.c_str());
          e.AddMessage("Ready , wait in/out message");
     }
     else if (CanUseMqtt() && MQTT_Client.connect(iob->getRequired().nomModule.c_str()))
     {
          e.State(ConState::CONNECTED);
          e.AddMessage("Connecté sans login/password");
          MQTT_Client.subscribe(iob->getMqtt().topicIn.c_str());
          e.AddMessage("Ready , wait in/out message");
     }
     else
     {
          e.AddMessage("Erreur, rc=" + String(MQTT_Client.state()));
          e.AddMessage("Prochaine tentative dans (ms) : " + String(iob->getMqtt().intervalConnect));
     }
     mqtt_State_Changed_EventHandler.fire(e);
}

void IOB_IOTMQTT::ParseMqttMessage(char *topic, byte *message, unsigned int length)
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     // Message reçu du Broker.
     String string;
     // On vérifie qu'il vient bien de Domoticz.
     int valeur = strcmp(topic, iob->getMqtt().topicIn.c_str());
     if (valeur == 0)
     {
          for (unsigned int i = 0; i < length; i++)
          {
               string += ((char)message[i]);
          }

          StaticJsonBuffer<512> jsonBuffer;
          JsonObject &root = jsonBuffer.parseObject(string);
          if (root.success())
          {
               int idx = root["idx"];
               int nvalue = root["nvalue"];
               if (idx == (int)iob->getRequired().idxDevice)
               {
                    IOB_IOTMessageRecevedEventArgs e = IOB_IOTMessageRecevedEventArgs(iob->getRequired().idxDevice, RelayStateConverter::fromInt(nvalue), SendProtole::MQTT);
                    e.AddMessage("Message arrivé [topic] : " + String(topic));
                    e.AddMessage("Recep " + String((nvalue == 1) ? "On" : "Off") + " ");
                    iob->mqtt_Recep_EventHandler.fire(e);
                    if (!e.Handled())
                    {
                         if (nvalue == 1)
                         {
                              digitalWrite(iob->getRequired().relayPin, iob->getRequired().relayNcOrNo ? LOW : HIGH);
                         }
                         else
                         {
                              digitalWrite(iob->getRequired().relayPin, iob->getRequired().relayNcOrNo ? HIGH : LOW);
                         }
                    }
               }
          }
     }
}

bool IOB_IOTMQTT::Sendata(RelayState state)
{
     IOB_IOT *iob = IOB_IOT::GetInstance();

     bool sendSuccess = false;
     String messJson;
     if (iob->CreateJsonMessageForDomoticz(state, messJson))
     {
          IOB_IOTMessageSendedEventArgs e = IOB_IOTMessageSendedEventArgs(iob->getRequired().idxDevice, state, SendProtole::MQTT, messJson);
          // sendSuccess = SendData(messJson, SendDataMethod::SENDBY_MQTT_ONLY, e);
          sendSuccess = MQTT_Client.publish(iob->getMqtt().topicOut.c_str(), messJson.c_str());
          e.AddMessage("Message envoyé à Domoticz en MQTT");
          mqtt_Send_EventHandler.fire(e);
     }
     return sendSuccess;
}

void IOB_IOTMQTT::LoopMqtt()
{
     IOB_IOT *iob = IOB_IOT::GetInstance();

     unsigned long currentMillis = millis();
     if (CanUseMqtt())
     {
          if (!MQTT_Client.connected() && ((currentMillis - iob->getMqtt().previousMillis) >= iob->getMqtt().intervalConnect))
          {
               ReconnectMQTT();
               iob->setPreviousMillis(currentMillis);
          }
          else if (MQTT_Client.connected())
          {
               MQTT_Client.loop();
               if ((currentMillis - iob->getMqtt().previousMillis) >= iob->getMqtt().intervalConnect)
               {
                    iob->setPreviousMillis(currentMillis);
               }
          }
     }
}

#endif