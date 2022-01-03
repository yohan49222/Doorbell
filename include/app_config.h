#ifndef app_config_h 

#define app_config_h 

const char* TOPICIN  = "domoticz/out"; // Nom du topic envoyé par Domoticz            
const char* TOPICOUT = "domoticz/in"; // Nom du topic écouté par Domoticz
// Adresse IP ou DNS de la box domotic.
const char* DOMOTIC_SERVER = "192.168.1.2";//
// Port de la box domotic
const int DOMOTIC_PORT = 8080;

// Adresse IP ou DNS du Broker.
const char* MQTT_SERVER = "192.168.1.2";
// Port du Brocker MQTT
const int MQTT_PORT = 1883;
// Login de connexion à MQTT.
const char* MQTT_LOGIN = "yohan";
// Mot de passe de connexion à MQTT.
const char* MQTT_PASSWORD = "poiazer123";

// Nom usuel de ce module. Sera visible uniquement dans les Log Domoticz.
const char* NOMMODULE = "sonnette";
// Index du Device à actionner.            
const int IDXDEVICE = 13;

#endif  
