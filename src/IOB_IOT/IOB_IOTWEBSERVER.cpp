#include "IOB_IOT/IOB_IOTWEBSERVER.h"
#include "IOB_IOT.h"

#ifdef USE_WEBSERVER

void IOB_IOTWEBSERVER::init()
{
     webServer.on("/switchOn", IOB_IOTWEBSERVER::SwitchOn);
     webServer.on("/switchOff", IOB_IOTWEBSERVER::SwitchOff);
     webServer.on("/DebugServeur.json", IOB_IOTWEBSERVER::DebugServeur);
     webServer.begin(80);
}
void IOB_IOTWEBSERVER::Loop()
{
     webServer.handleClient();
}

bool IOB_IOTWEBSERVER::CreateJsonMessageForDebug(String &out)
{
     StaticJsonBuffer<1024> jsonBuffer;
     StaticJsonBuffer<1024> jsonBuffer2;
     JsonObject &root = jsonBuffer.createObject();

     // assigantion des variables

#ifdef USE_IPFIXE
     // JsonObject &root2 = jsonBuffer2.createObject();
     // root2["ipFixe"] = sConfigIp.ip.toString();
     // root2["ipGateWay"] = sConfigIp.gateWay.toString();
     // root2["ipSubnet"] = sConfigIp.subnet.toString();
     // root2["ipDns"] = sConfigIp.dns.toString();
     // root["ip"] = root2;
#endif

#ifdef USE_OTA
     // JsonObject &root3 = jsonBuffer2.createObject();
     // root3["OtaName"] = sConfigOta.name;
     // root3["OtaPassword"] = sConfigOta.password;
     // root["ota"] = root3;
#endif

     // root["Ssid"] = sConfigWifi.ssid;
     // root["SsidPassword"] = sConfigWifi.password;

#ifdef USE_HTTP
     // root["DomoticzServer"] = sConfigDomo.ip.toString();
     // root["DomoticzPort"] = String(sConfigDomo.port);
#endif

#ifdef USE_MQTT
     // root["TopicIn"] = sConfigMqtt.topicIn;
     // root["topicOut"] = sConfigMqtt.topicOut;
     // root["MqttServer"] = sConfigMqtt.ip.toString();
     // root["MqttPort"] = String(sConfigMqtt.port);
     // root["MqttLogin"] = sConfigMqtt.login;
     // root["MqttPassword"] = sConfigMqtt.password;
     // root["NomModule"] = sConfigIot.nomModule;
     // root["IdxDevice"] = String(sConfigIot.idxDevice);
#endif

     String messageOut;

     // formatage de la chaine json
     int printed = root.printTo(messageOut);
     if (printed > 0)
     {
          /*
           Convertion du message en Char
           */
          char messageChar[messageOut.length() + 1];
          messageOut.toCharArray(messageChar, messageOut.length() + 1);

          // assignation de out
          out = String(messageChar);
          return true;
     }
     return false;
}

void IOB_IOTWEBSERVER::TraitRequestWeb(RelayState state)
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     IPAddress clientIp = iob->webServer.client().remoteIP();
     IOB_IOTMessageRecevedEventArgs e = IOB_IOTMessageRecevedEventArgs(iob->getRequired().idxDevice, state, SendProtole::WEBSERVER);
     e.AddMessage("Reception commande " + e.StateString() + " de " + clientIp.toString());

     if (
#ifdef USE_HTTP
         clientIp != iob->getDomotic().ip
#else
         true
#endif
         &&
#ifdef USE_MQTT
         clientIp != iob->getMqtt().ip
#else
         true
#endif

     )
     {
          iob->SendData(state);
          e.AddMessage("Envois commande " + e.StateString() + " a la domotique");
     }
     else
     {
          e.AddMessage("Pas d'envoi de commande a la domotique");
     }
     iob->webServer_Request_EventHandler.fire(e);
     String messJson;
     if (iob->CreateJsonMessageForDomoticz(state, messJson))
          iob->webServer.send(200, "text/json", messJson);

     IOB_IOTMessageSendedEventArgs er = IOB_IOTMessageSendedEventArgs(iob->getRequired().idxDevice, state, SendProtole::WEBSERVER);
     e.AddMessage("Envois de la reponse HTTP " + messJson);
     iob->webServer_Response_EventHandler.fire(er);
}

void IOB_IOTWEBSERVER::SwitchOn()
{
     TraitRequestWeb(RelayState::ON);
}

void IOB_IOTWEBSERVER::SwitchOff()
{
     TraitRequestWeb(RelayState::OFF);
}

void IOB_IOTWEBSERVER::DebugServeur()
{
     IOB_IOT *iob = IOB_IOT::GetInstance();
     String messJson;
     if (iob->CreateJsonMessageForDebug(messJson))
          iob->webServer.send(200, "application/json", messJson);
}

#endif