/*
     *********************************************
     *** Firmware pour module relay ESP01-S 1M ***
     *********************************************

     !!! IMPORTANT : Vous devez faire une premiere compilation de votre projet
                         pour generer le fichier "config.json" .

                         Vous devez ensuite editer ce meme fichier avec vos parametres
*/

#include "IOB_IOT.h"
IOB_IOT *iob = IOB_IOT::GetInstance();

void dingdong(bool sendOn, bool sendOff)
{

     Serial.println("[MAIN] Process DINGDONG START ( custum process )");

     if (sendOn)
          iob->SendData("On");

     Serial.println("[MAIN] Ding ( relay ON )");
     digitalWrite(RELAY_PIN, NCORNO ? LOW : HIGH);
     delay(200);
     Serial.println("[MAIN] Ding ( pause 200ms )");
     digitalWrite(RELAY_PIN, NCORNO ? HIGH : LOW);
     Serial.println("[MAIN] Ding ( pause 200ms )");
     delay(1000);

     Serial.println("[MAIN] Ding ( pause 1s Show sate in Domoticz )");
     if (sendOff)
          iob->SendData("Off");

     Serial.println("[MAIN] Process DINGDONG END");
}

void MessageRecep(IOB_IOTMessageRecevedEventArgs e)
{
     for (String s : e.MessageList())
     {
          Serial.println("[" + e.SendProtoleString() + "] " + s);
     }
     if (e.State() == 1)
     {
          dingdong(false, true);
     }
     else if (e.State() == 0)
     {
          digitalWrite(RELAY_PIN, NCORNO ? HIGH : LOW);
     }
     e.Handled(true);
}

void MessageSend(IOB_IOTMessageSendedEventArgs e)
{
     for (String s : e.MessageList())
     {
          Serial.println("[" + e.SendProtoleString() + "] " + s);
     }
}

void WifiStateChanged(IOB_IOTWifiStateChangedEventArgs e)
{
     for (String s : e.MessageList())
     {
          Serial.println("[WIFI] " + s);
     }
}

void MqttStateChanged(IOB_IOTMqttStateChangedEventArgs e)
{
     for (String s : e.MessageList())
     {
          Serial.println("[MQTT] " + s);
     }
}

void ButtonPressed(IOB_IOTButtonPressedEventArgs e)
{
     dingdong(true, true);
     e.Handled(true);
}

void setup()
{
     Serial.begin(115200L);
     delay(200);

     iob->OnMqttRecep(MessageRecep);
     iob->OnMqttSend(MessageSend);

     iob->OnWebRecep(MessageRecep);
     iob->OnWebSend(MessageSend);

     iob->OnHttpSend(MessageSend);

     iob->OnMqttStateChanged(MqttStateChanged);
     iob->OnWifiStateChanged(WifiStateChanged);
     iob->OnButtonPressed(ButtonPressed);

     iob->Run();
}

void loop()
{
     iob->Loop();
}
