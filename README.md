# *** Firmware pour module relay ESP01-S 1M ***

![](https://github.com/yohan49222/Doorbell/blob/main/images/relai%20esp01_.jpg) 
</br></br>
|   Integation|Optionnel|Depend|Commentaire|
| :------|:-----|:-----|:-----|
|   WIFI       |    yes    |    config.json    |    |
|   OTA        |    yes    |    WIFI      |     |
|   WEBSERVER  |    yes    |    WIFI      | |
|   MQTT       |    yes    |    WIFI      |    |
|   HTTP       |    yes    |    MQTT      | Surchargé par MQTT   |

**HTTP**, Le protocole http sera utilisé si le MQTT n'est pas disponible. 
</br></br>
# Configuration Info :

### *!!! IMPORTANT* :

* **1** - Vous devez faire une premiere ***compilation*** de votre projet pour générer le  fichier **"config.json"** .
* **2** - Vous devez ensuite éditer ce meme fichier avec vos parametres



<details><summary>Parametre JSON</summary>
<p>

```json
{
     "UPLOAD_WITH_OTA"             :"0",               /* Utilisation de l'OTA pour upload */
                                                       /* env.Replace(
                                                            UPLOAD_PROTOCOL="espota",
                                                            UPLOAD_PORT=<OTAIP>,
                                                       ) */

     "RELAY_PIN"                   :"0",               /* Numéro de Pin du relai ( normalement 0) */
     "BUTTON_PIN"                  :"2",               /* Numéro de Pin du button  */
     "NCORNO"                      :"1",               /* Connection relai sur NC ou NO */
     "RELAY_AUTO_OFF"              :"1",               /* Relay auto off */
     "RELAY_AUTO_OFF_AFTER"        :"200",             /* Relay auto off apres X ms*/
     "DEBOUNCE_TIME"               :"100",             /* Temps antirebon du bouton en ms */
     "BUTTON_PRESS_COUNT_MAX"      :"5",               /* 5 * 100ms Temps d'appuis minimum avant declanchement de l'evenement */
     "NOMMODULE"                   :"sonnette",        /* Nom du module Visible de domoticz et l'ota */
     "IDXDEVICE"                   :"13",              /* IDX du DEVICE dans Domoticz */
     "USE_MQTT"                    :"0",               /* Activation du "MQTT" 0 = désactivé , 1 = Activé */
     "USE_WEBSERVER"               :"0",               /* Activation du "WEBSERVER" 0 = désactivé , 1 = Activé */
     "USE_HTTP"                    :"0",               /* Activation du "HTTP" 0 = désactivé , 1 = Activé */
     "USE_IPFIXE"                  :"0",               /* Activation du "IPFIXE" 0 = désactivé , 1 = Activé */
     "USE_OTA"                     :"0",               /* Activation du "OTA" 0 = désactivé , 1 = Activé */
     "USE_WIFI"                    :"0",               /* Activation du "WIFI" 0 = désactivé , 1 = Activé */
     "IP":
     {
          "IPFIXE"                 :"192.168.1.51",    /* Adresse ip du module */
          "GATEWAY"                :"192.168.1.1",     /* Adresse ip de la box internet */
          "SUBNET"                 :"255.255.255.0",   /* Adresse ip du mask sous reseau  */
          "DNS"                    :"192.168.1.1"      /* Adresse ip du dns  */
     },
     "OTA":
     {
          "OTAIP"                  :"192.168.1.51",    /* Adresse ip OTA */
          "OTANAME"                :"sonnette",        /* Nom du module sur l'OTA ( peut etre different de le NOMMODULE ) */
          "OTAPASSWORD"            :""                 /* Mot de passe OTA ( peut etre vide ) */
     },
     "WIFI":
     {
          "MYSSID"                 :"",                /* Nom du reseau wifi */
          "MYSSID_PASSWORD"        :""                 /* Mot de passe du reseau wifi */
     },
     "WEBSERVER":
     {
          "WEBSERVER_PORT"         :"80"               /* Numero de port du serveur web */
     },
     "HTTP":
     {
          "DOMOTIC_SERVER"         :"192.168.1.2",     /* Adresse ip de la box domoticz */
          "DOMOTIC_PORT"           :"8080"             /* Port de la box Domoticz */
     },
     "MQTT":
     {
          "TOPICIN"                :"domoticz/out",    /* Topic sortant */
          "TOPICOUT"               :"domoticz/in",     /* Topic entrant */
          "MQTT_SERVER"            :"192.168.1.2",     /* Adresse ip du broker MQTT */
          "MQTT_PORT"              :"1883",            /* Port du broker MQTT */
          "INTERVALCONNECT"        :"1000",            /* Internal de tentative de connection mqtt */
          "MQTT_LOGIN"             :"",                /* Login MQTT ( optionel suivant confiruration du broker ) */
          "MQTT_PASSWORD"          :""                 /* Mot de passe MQTT ( optionel suivant confiruration du broker ) */
     }
}
```
</p>
</details>
</br></br>

# Code exemples :

<details><summary>Minimum main.cpp</summary>
<p>

```c
#include "IOB_IOT.h"
//creation de la classe IOB_IOT (singleton)
IOB_IOT *iob = IOB_IOT::GetInstance();

void setup()
{
     Serial.begin(115200L);
     delay(200);

     // Lance le wifi , connecte, le mqtt, prepare le serveur web
     iob->Run();
}

void loop()
{
     // Loop IOB_IOT
     iob->Loop();
}
```
</p>
</details>

<details><summary>Voir les messages entrant et sortant http / mqtt / serveur web</summary>
<p>

```c
#include "IOB_IOT.h"
//creation de la classe IOB_IOT (singleton)
IOB_IOT *iob = IOB_IOT::GetInstance();

// Procedure de reception de message Http/Mqtt
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

// Procedure de reception de message Http/Mqtt
void MessageSend(IOB_IOTMessageSendedEventArgs e)
{
     for (String s : e.MessageList())
     {
          Serial.println("[" + e.SendProtoleString() + "] " + s);
     }
}


void setup()
{
     Serial.begin(115200L);
     delay(200);

     // Evenement declanche sur reception de commande mqtt
     iob->OnMqttRecep(MessageRecep);

     // Evenement declanché apres envois de commande mqtt a Domoticz
     iob->OnMqttSend(MessageSend);

     // Evenement declanché sur requette web : http://<ip>/switchOn ou switchOff
     iob->OnWebRecep(MessageRecep);

     // Evenement declanché apres envois de la reponse
     // au client web: http://<ip>/switchOn ou switchOff
     iob->OnWebSend(MessageSend);

     // Envenement declanché apres envois de commande en http
     // Le protocole prioritaire est le mqtt , 
     // note: le http est utilisé si le mqtt est désactivé ou déconnecté
     iob->OnHttpSend(MessageSend);

     // Lance le wifi , connecte, le mqtt, prepare le serveur web
     iob->Run();
}

void loop()
{
     // Loop IOB_IOT
     iob->Loop();
}
```
</p>
</details>

<details>
<summary>Voir les connections WIFI / MQTT</summary>
<p>

```c
#include "IOB_IOT.h"
//creation de la classe IOB_IOT (singleton)
IOB_IOT *iob = IOB_IOT::GetInstance();

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

void setup()
{
     Serial.begin(115200L);
     delay(200);

     //Evenement déclanché apres connection ou déconnection du Mqtt
     iob->OnMqttStateChanged(MqttStateChanged);

     //Evenement déclanché apres connection ou déconnection du WIFI
     iob->OnWifiStateChanged(WifiStateChanged);

     // Lance le wifi , connecte, le mqtt, prepare le serveur web
     iob->Run();
}

void loop()
{
     // Loop IOb_IOT
     iob->Loop();
}
```
</p>
</details>


<details>
<summary>Action sur le bouton</summary>
<p>

```c
#include "IOB_IOT.h"
//creation de la classe IOB_IOT (singleton)
IOB_IOT *iob = IOB_IOT::GetInstance();

// DINGDONG CUSTON
void dingdong(bool sendOn, bool sendOff)
{

     Serial.println("[MAIN] Process DINGDONG START ( custum process )");

     if (sendOn)
          iob->SendData(RelayState::ON); // Informe domoticz

     Serial.println("[MAIN] Ding ( relay ON )");
     digitalWrite(RELAY_PIN, NCORNO ? LOW : HIGH); // Change la position du relai
     
     Serial.println("[MAIN] Ding ( pause 200ms )");

     delay(200); // Ch'tite pause
     
     digitalWrite(RELAY_PIN, NCORNO ? HIGH : LOW); // Rechange la position du relai


     Serial.println("[MAIN] Ding ( pause 1s Show state in Domoticz )");
     delay(1000);
     
     if (sendOff)
          iob->SendData(RelayState::OFF); // Informe domoticz

     Serial.println("[MAIN] Process DINGDONG END");
}

//bouton presse event 
void ButtonPressed(IOB_IOTButtonPressedEventArgs e)
{
     // Appel de la fonction dingdong avec parametre true/true .... 
     une vache qui p...... Stoppppp
     // le premier 'true' indique de l'on souhaite prevenir domoticz que le bouton est sur ON
     // Le deuxieme 'true' indique de l'on souhaite prevenir domoticz que le relai en sur OFF
     dingdong(true, true);

     // Stop la propagation de l'évenement si true / 
     // si false , IOB_IOB activera le relai pendant 200ms puis le coupera
     e.Handled(true);
}

void setup()
{
     Serial.begin(115200L);
     delay(200);

     //Evenement déclanché apres sur l'appuis de contacteur de "sonette"
     iob->OnButtonPressed(ButtonPressed);

     // Lance le wifi , connecte, le mqtt, prepare le serveur web
     iob->Run();
}

void loop()
{
     // Loop IOb_IOT
     iob->Loop();
}
```
</p>
</details>




</br>

# Liens  :

![](https://github.com/yohan49222/Doorbell/blob/main/images/logo98-98.png)

*  Voir mes vidéos [**Youtube**](https://www.youtube.com/channel/UCCFsJROyzppyjq3WvpNAwGA).
*  Contactez-moi via ma page facebook  [**IciOnBricole**](https://www.facebook.com/IciOnBricole).
