#ifndef app_config_private_h
#define app_config_private_h
#define USE_WIFI
#ifdef USE_WIFI
#include "wifi_config.h"
#endif

// Nom usuel de ce module. Sera visible uniquement dans les Log Domoticz.
#define NOMMODULE "sonnette"
// Index du Device à actionner.
#define IDXDEVICE 13



#define USE_IPFIXE
#define IPFIXE "192.168.1.51"
#define GATEWAY "192.168.1.1"
#define SUBNET "255.255.255.0"
#define DNS "192.168.1.1"



#define USE_HTTP
// Nom du topic envoyé par Domoticz
#define TOPICIN "domoticz/out"
// Nom du topic écouté par Domoticz
#define TOPICOUT "domoticz/in"
// Adresse IP ou DNS de la box domotic.
#define DOMOTIC_SERVER "192.168.1.2"
 // Port de la box domotic
#define DOMOTIC_PORT 8080


#define USE_WEBSERVER
#define WEBSERVER_PORT 80


#define USE_MQTT
// Adresse IP ou DNS du Broker.
#define MQTT_SERVER "192.168.1.2"
// Port du Brocker MQTT
#define MQTT_PORT 1883
// Login de connexion à MQTT.
#define MQTT_LOGIN "yohan"
// Mot de passe de connexion à MQTT.
#define MQTT_PASSWORD "poiazer123"

#define USE_OTA
#define OTANAME "default"
#define OTAPASSWORD "password"
#endif