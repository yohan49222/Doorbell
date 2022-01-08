# relai esp01
![](https://github.com/yohan49222/Doorbell/blob/main/images/relai%20esp01_.jpg) 
## Firmware pour module relai esp01

|   Integation|Optionnel|Depend|
| :------:|:-----:|:-----:|
|   WIFI       |    yes    |    Config    | Comment \#define USE_WIFI   |
|   OTA        |    yes    |    WIFI      | Comment \#define USE_OTA    |
|   WEBSERVER  |    yes    |    WIFI      | Comment \#define USE_WEBSERVER|
|   MQTT       |    yes    |    WIFI      | Comment \#define USE_MQTT   |
|   HTTP       |    yes    |    WIFI      | Comment \#define USE_HTTP   |

**HTTP**, Le protocole http sera utilisé sur le MQTT n'est pas disponible. 

voir fichier "IOB_IOT_public.h" pour la configuration