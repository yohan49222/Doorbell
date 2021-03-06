/*
     *********************************************
     *** Firmware pour module relay ESP01-S 1M ***
     *********************************************

     !!! IMPORTANT : Vous devez faire une premiere compilation de votre projet
                         pour generer le fichier "config.json" .

                         Vous devez ensuite editer ce meme fichier avec vos parametres
*/

#include "IOB_IOT.h"
//creation de la classe IOB_IOT (singleton)
IOB_IOT *iob = IOB_IOT::GetInstance();

void dingdong(bool sendOn, bool sendOff);

// Procedure de reception de message Http/Mqtt
void MessageRecep(IOB_IOTMessageRecevedEventArgs e)
{
     for (String s : e.MessageList())
     {
          Serial.println("[" + e.SendProtoleString() + "] " + s);
     }
     if (e.State() == RelayState::ON)
     {
          dingdong(false, true);
     }
     else if (e.State() == RelayState::OFF)
     {
          digitalWrite(iob->getRequired().relayPin, iob->getRequired().relayNcOrNo ? HIGH : LOW);
     }
     e.Handled(true);
}

// Procedure de reception de message Http/Mqtt
void MessageSend(IOB_IOTMessageSendedEventArgs e)
{
     for (String s : e.MessageList())
     {
          Serial.println("[" + e.SendProtoleString() + "] " + s);
     }
}

// Procedure de connection / deconnection du Wifi
void WifiStateChanged(IOB_IOTWifiStateChangedEventArgs e)
{
     for (String s : e.MessageList())
     {
          Serial.println("[WIFI] " + s);
     }
}

// Procedure de connection / deconnection du Mqtt
void MqttStateChanged(IOB_IOTMqttStateChangedEventArgs e)
{
     for (String s : e.MessageList())
     {
          Serial.println("[MQTT] " + s);
     }
}

// DINGDONG CUSTON
void dingdong(bool sendOn, bool sendOff)
{

     Serial.println("[MAIN] Process DINGDONG START ( custum process )");

     if (sendOn)
          iob->SendData(RelayState::ON); // Informe domoticz

     Serial.println("[MAIN] Ding ( relay ON )");
     digitalWrite(iob->getRequired().relayPin, iob->getRequired().relayNcOrNo ? LOW : HIGH); // Change la position du relai
     
     Serial.println("[MAIN] Ding ( pause 200ms )");

     delay(iob->getRequired().relayAutoOffAfter); // Ch'tite pause
     
     digitalWrite(iob->getRequired().relayPin, iob->getRequired().relayNcOrNo ? HIGH : LOW); // Rechange la position du relai


     Serial.println("[MAIN] Ding ( pause 1s Show state in Domoticz )");
     delay(1000);
     
     if (sendOff)
          iob->SendData(RelayState::OFF); // Informe domoticz

     Serial.println("[MAIN] Process DINGDONG END");
}

/**
 * @brief 
 * 
 * @param e 
 */
void ButtonPressed(IOB_IOTButtonPressedEventArgs e)
{
     for (String s : e.MessageList())
     {
          Serial.println("[BUTTON] " + s);
     }

     // Appel de la fonction dingdong avec parametre true/true .... une vache qui p...... Stoppppp
     // le premier 'true' indique de l'on souhaite prevenir domoticz que le bouton est sur ON
     // Le deuxieme 'true' indique de l'on souhaite prevenir domoticz que le relai en sur OFF
     dingdong(true, true);

     // Stop la propagation de l'??venement si true / 
     // si false , IOB_IOB activera le relai pendant 200ms puis le coupera
     e.Handled(true);
}

void setup()
{
     Serial.begin(115200L);
     delay(200);

     // Evenement declanche sur reception de commande mqtt
     iob->OnMqttRecep(MessageRecep);

     // Evenement declanch?? apres envois de commande mqtt a Domoticz
     iob->OnMqttSend(MessageSend);

     // Evenement declanch?? sur requette web : http://<ip>/switchOn ou switchOff
     iob->OnWebRecep(MessageRecep);

     // Evenement declanch?? apres envois de la reponse
     // au client web: http://<ip>/switchOn ou switchOff
     iob->OnWebSend(MessageSend);

     // Envenement declanch?? apres envois de commande en http
     // Le protocole prioritaire est le mqtt , 
     // note: le http est utilis?? si le mqtt est d??sactiv?? ou d??connect??
     iob->OnHttpSend(MessageSend);


     //Evenement d??clanch?? apres connection ou d??connection du Mqtt
     iob->OnMqttStateChanged(MqttStateChanged);

     //Evenement d??clanch?? apres connection ou d??connection du WIFI
     iob->OnWifiStateChanged(WifiStateChanged);

     //Evenement d??clanch?? apres sur l'appuis de contacteur de "sonette"
     iob->OnButtonPressed(ButtonPressed);

     // Lance le wifi , connecte, le mqtt, prepare le serveur web
     iob->Run();
}

void loop()
{
     // Loop IOb_IOT
     iob->Loop();
}
