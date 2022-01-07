#ifndef IOB_IOT_private_h
#define IOB_IOT_private_h


#define USE_WIFI
#ifdef USE_WIFI
#include "wifi_config.h"


// Nom usuel de ce module. Sera visible uniquement dans les Log Domoticz.
#define NOMMODULE "sonnette"
// Index du Device à actionner.
#define IDXDEVICE 13



//#define USE_IPFIXE
#ifdef USE_IPFIXE
#define IPFIXE "192.168.1.51"
#define GATEWAY "192.168.1.1"
#define SUBNET "255.255.255.0"
#define DNS "192.168.1.1"
#endif


//#define USE_HTTP
#ifdef USE_HTTP
// Adresse IP ou DNS de la box domotic.
#define DOMOTIC_SERVER "192.168.1.2"
 // Port de la box domotic
#define DOMOTIC_PORT 8080
#endif

//#define USE_WEBSERVER
#ifdef USE_WEBSERVER
#define WEBSERVER_PORT 80
#endif

#define USE_MQTT
#ifdef USE_MQTT
// Nom du topic envoyé par Domoticz
#define TOPICIN "domoticz/out"
// Nom du topic écouté par Domoticz
#define TOPICOUT "domoticz/in"
// Adresse IP ou DNS du Broker.
#define MQTT_SERVER "192.168.1.2"
// Port du Brocker MQTT
#define MQTT_PORT 1883
// Login de connexion à MQTT.
#define MQTT_LOGIN "yohan"
// Mot de passe de connexion à MQTT.
#define MQTT_PASSWORD "poiazer123"
#endif


//#define USE_OTA
#ifdef USE_OTA
#define OTANAME "sonnette"
#define OTAPASSWORD "poiazer123"
#endif
#endif

#endif