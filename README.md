# *** Firmware pour module relay ESP01-S 1M ***

![](https://github.com/yohan49222/Doorbell/blob/main/images/relai%20esp01_.jpg) 


## !!! IMPORTANT : 
Vous devez faire une premiere compilation de votre projet pour generer le fichier "config.json" .
Vous devez ensuite editer ce meme fichier avec vos parametres

|   Integation|Optionnel|Depend|
| :------:|:-----:|:-----:|
|   WIFI       |    yes    |    Config    | Comment \#define USE_WIFI   |
|   OTA        |    yes    |    WIFI      | Comment \#define USE_OTA    |
|   WEBSERVER  |    yes    |    WIFI      | Comment \#define USE_WEBSERVER|
|   MQTT       |    yes    |    WIFI      | Comment \#define USE_MQTT   |
|   HTTP       |    yes    |    WIFI      | Comment \#define USE_HTTP   |

**HTTP**, Le protocole http sera utilisé si le MQTT n'est pas disponible. 

