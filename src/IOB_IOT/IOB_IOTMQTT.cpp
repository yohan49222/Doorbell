#include "IOB_IOT/IOB_IOTMQTT.h"
#include "IOB_IOT.h"

void IOB_IOTMQTT::init()
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     MQTT_Client.setBufferSize(512);
     MQTT_Client.setServer(IOB_IOTMQTT::getMqtt().ip, IOB_IOTMQTT::getMqtt().port);
     MQTT_Client.setClient(iob->espClient);
     MQTT_Client.setCallback(IOB_IOTMQTT::CallbackMQTT);
}

void IOB_IOTMQTT::CallbackMQTT(char *topic, byte *message, unsigned int length)
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     iob->ParseMqttMessage(topic, message, length, iob->getConf());
}

bool IOB_IOTMQTT::CanSendMqtt()
{
     return CanUseMqtt() && MQTT_Client.connected();
}

bool IOB_IOTMQTT::CanUseMqtt()
{
     return IotConfig::getMqtt().ip.isSet() && IotConfig::getRequired().idxDevice > 0;
}

bool IOB_IOTMQTT::CanUseMqttSecure()
{
     return IotConfig::getMqtt().login != emptyString && IotConfig::getMqtt().password != emptyString && CanUseMqtt();
}

void IOB_IOTMQTT::ReconnectMQTT()
{
     IOB_IOTMqttStateChangedEventArgs e = IOB_IOTMqttStateChangedEventArgs();
     if (CanUseMqttSecure() && MQTT_Client.connect(IotConfig::getRequired().nomModule.c_str(), IotConfig::getMqtt().login.c_str(), IotConfig::getMqtt().password.c_str()))
     {
          e.State(ConState::CONNECTED);
          e.AddMessage("connecté avec login/password");
          MQTT_Client.subscribe(IotConfig::getMqtt().topicIn.c_str());
          e.AddMessage("Ready , wait in/out message");
     }
     else if (CanUseMqtt() && MQTT_Client.connect(IotConfig::getRequired().nomModule.c_str()))
     {
          e.State(ConState::CONNECTED);
          e.AddMessage("Connecté sans login/password");
          MQTT_Client.subscribe(IotConfig::getMqtt().topicIn.c_str());
          e.AddMessage("Ready , wait in/out message");
     }
     else
     {
          e.AddMessage("Erreur, rc=" + String(MQTT_Client.state()));
          e.AddMessage("Prochaine tentative dans (ms) : " + String(IotConfig::getMqtt().intervalConnect));
     }
     mqtt_State_Changed_EventHandler.fire(e);
}

void IOB_IOTMQTT::ParseMqttMessage(char *topic, byte *message, unsigned int length, IotConfig conf)
{
     IOB_IOT *iot = IOB_IOT::GetInstance();

     // Message reçu du Broker.
     String string;
     // On vérifie qu'il vient bien de Domoticz.
     int valeur = strcmp(topic, conf.getMqtt().topicIn.c_str());
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
               if (idx == (int)conf.getRequired().idxDevice)
               {
                    IOB_IOTMessageRecevedEventArgs e = IOB_IOTMessageRecevedEventArgs(conf.getRequired().idxDevice, RelayStateConverter::fromInt(nvalue), SendProtole::MQTT);
                    e.AddMessage("Message arrivé [topic] : " + String(topic));
                    e.AddMessage("Recep " + String((nvalue == 1) ? "On" : "Off") + " ");
                    iot->mqtt_Recep_EventHandler.fire(e);
                    if (!e.Handled())
                    {
                         if (nvalue == 1)
                         {
                              digitalWrite(conf.getRequired().relayPin, conf.getRequired().relayNcOrNo ? LOW : HIGH);
                         }
                         else
                         {
                              digitalWrite(conf.getRequired().relayPin, conf.getRequired().relayNcOrNo ? HIGH : LOW);
                         }
                    }
               }
          }
     }
}

bool IOB_IOTMQTT::CreateJsonMessageForDomoticz(RelayState state, String &out)
{
     StaticJsonBuffer<256> jsonBuffer;
     JsonObject &root = jsonBuffer.createObject();

     // assigantion des variables.
     root["command"] = "switchlight";
     root["idx"] = IotConfig::getRequired().idxDevice;
     root["switchcmd"] = RelayStateConverter::toString(state);

     String messageOut;

     // formatage de la chaine json
     int printed = root.printTo(messageOut);
     if (printed > 0)
     {
          // Convertion du message en Char
          char messageChar[messageOut.length() + 1];
          messageOut.toCharArray(messageChar, messageOut.length() + 1);

          // assignation de out
          out = String(messageChar);
          return true;
     }
     return false;
}

bool IOB_IOTMQTT::Sendata(RelayState state)
{
     bool sendSuccess = false;
     String messJson;
     if (CreateJsonMessageForDomoticz(state, messJson))
     {
          IOB_IOTMessageSendedEventArgs e = IOB_IOTMessageSendedEventArgs(IOB_IOTMQTT::getRequired().idxDevice, state, SendProtole::MQTT, messJson);
          // sendSuccess = SendData(messJson, SendDataMethod::SENDBY_MQTT_ONLY, e);
          sendSuccess = MQTT_Client.publish(IOB_IOTMQTT::getMqtt().topicOut.c_str(), messJson.c_str());
          e.AddMessage("Message envoyé à Domoticz en MQTT");
          mqtt_Send_EventHandler.fire(e);
     }
     return sendSuccess;
}

void IOB_IOTMQTT::LoopMqtt()
{
     unsigned long currentMillis = millis();
     if (CanUseMqtt())
     {
          if (!MQTT_Client.connected() && ((currentMillis - getMqtt().previousMillis) >= getMqtt().intervalConnect))
          {
               ReconnectMQTT();
               setPreviousMillis(currentMillis);
          }
          else if (MQTT_Client.connected())
          {
               MQTT_Client.loop();
               if ((currentMillis - getMqtt().previousMillis) >= getMqtt().intervalConnect)
               {
                    setPreviousMillis(currentMillis);
               }
          }
     }
}
