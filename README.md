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

## Configuration Info 

Parametre JSON:
{
    "UPLOAD_WITH_OTA":"0",          /* Utilisation de l'OTA pour upload */
    "RELAY_PIN":"0",                /* Numéro de Pin du relai ( normalement 0) */
    "BUTTON_PIN":"2",               /* Numéro de Pin du button  */
    "NCORNO":"1",                   /* Connection relai sur NC ou NO */
    "DEBOUNCE_TIME":"100",          /* Temps antirebon du bouton en ms */
    "BUTTON_PRESS_COUNT_MAX":"5",   /* 5 * 100ms Temps d'appuis minimum avant declanchement de l'evenement */
    "NOMMODULE":"sonnette",         /* Nom du module Visible de domoticz et l'ota */
    "IDXDEVICE":"13",               /* IDX du DEVICE dans Domoticz */
    "USE_MQTT":"0",                 /* Activation du "MQTT" 0 = désactivé , 1 = Activé */
    "USE_WEBSERVER":"0",            /* Activation du "WEBSERVER" 0 = désactivé , 1 = Activé */
    "USE_HTTP":"0",                 /* Activation du "HTTP" 0 = désactivé , 1 = Activé */
    "USE_IPFIXE":"0",               /* Activation du "IPFIXE" 0 = désactivé , 1 = Activé */
    "USE_OTA":"0",                  /* Activation du "OTA" 0 = désactivé , 1 = Activé */
    "USE_WIFI":"0",                 /* Activation du "WIFI" 0 = désactivé , 1 = Activé */
    "IP":
    {
        "IPFIXE":"192.168.1.51",    /* Adresse ip du module */
        "GATEWAY":"192.168.1.1",    /* Adresse ip de la box internet */
        "SUBNET":"255.255.255.0",   /* Adresse ip du mask sous reseau  */
        "DNS":"192.168.1.1"         /* Adresse ip du dns  */
    },
    "OTA":
    {
        "OTAIP":"192.168.1.51",     /* Adresse ip OTA */
        "OTANAME":"sonnette",       /* Nom du module sur l'OTA ( peut etre different de le NOMMODULE ) */
        "OTAPASSWORD":""            /* Mot de passe OTA ( peut etre vide ) */
    },
    "WIFI":
    {
        "MYSSID":"",                /* Nom du reseau wifi */
        "MYSSID_PASSWORD":""        /* Mot de passe du reseau wifi */
    },
    "WEBSERVER":
    {
        "WEBSERVER_PORT":"80"       /* Numero de port du serveur web */
    },
    "HTTP":
    {
        "DOMOTIC_SERVER":"192.168.1.2", /* Adresse ip de la box domoticz */
        "DOMOTIC_PORT":"8080"           /* Port de la box Domoticz */
    },
    "MQTT":
    {
        "TOPICIN":"domoticz/out",       /* Topic sortant */
        "TOPICOUT":"domoticz/in",       /* Topic entrant */
        "MQTT_SERVER":"192.168.1.2",    /* Adresse ip du broker MQTT */
        "MQTT_PORT":"1883",             /* Port du broker MQTT */
        "MQTT_LOGIN":"",                /* Login MQTT ( optionel suivant confiruration du broker ) */
        "MQTT_PASSWORD":""              /* Mot de passe MQTT ( optionel suivant confiruration du broker )
    }
}
