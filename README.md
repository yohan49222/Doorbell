



#define USE_WIFI    // Commente cette ligne si tu ne veut pas que ton module utilise le wifi
                    // Il sera alors completement autonome et actionnable uniquement via le bouton poussoire
#ifdef USE_WIFI
#include "wifi_config.h"


/*
Nom usuel de ce module
Si vous laissez la valeur par defaut (default) , 
le nom du module sera auto-généré en esp8266-XXXX
*/
#define NOMMODULE "default"
/*
 iDX du Device.
 Neccésaire pour la communication MQTT et/ou HTTP 
*/
#define IDXDEVICE 13



#define USE_IPFIXE  // Commente cette ligne si tu ne veux pas utiliser une ip fixe pour ton module
#ifdef USE_IPFIXE
#define IPFIXE "192.168.1.51"
#define GATEWAY "192.168.1.1"
#define SUBNET "255.255.255.0"
#define DNS "192.168.1.1"
#endif


#define USE_HTTP    // Commente cette ligne si tu ne veux pas que le module utilise le http
                    // au cas où le serveur MQTT serait indisponnible ou désactivé en dessous
#ifdef USE_HTTP
// Adresse IP ou DNS de la box domotic.
#define DOMOTIC_SERVER "0.0.0.0"
 // Port de la box domotic
#define DOMOTIC_PORT 8080
#endif

// Commente juste la ligne ci-dessous si tu ne veux pas que ton module reponde aux requettes web
#define USE_WEBSERVER   
// http://<ip_du_module>/switchOn       pour allumer le module
// http://<ip_du_module>/switchOff      pour l'eteindre
// http://<ip_du_module>/DebugServeur   affiche la configuraion du module dans ton navigateur web en json
#ifdef USE_WEBSERVER
#define WEBSERVER_PORT 80
#endif

// Commente juste la ligne ci-dessous si tu ne veux pas que ton module utilise un serveur MQTT
#define USE_MQTT
#ifdef USE_MQTT
// Nom du topic envoyé par Domoticz
#define TOPICIN "domoticz/out"
// Nom du topic écouté par Domoticz
#define TOPICOUT "domoticz/in"
// Adresse IP du Broker.
#define MQTT_SERVER "0.0.0.0"
// Port du Brocker MQTT
#define MQTT_PORT 1883
// Login de connexion à MQTT. (optionel selon la configuration du broker MQTT)
// laisser (login) si inutile
#define MQTT_LOGIN "login"
// Mot de passe de connexion à MQTT. (optionel selon la configuration du broker MQTT)
// laisser (password) si inutile
#define MQTT_PASSWORD "password"
#endif

// Commente juste la ligne ci-dessous si tu ne veux pas que ton module utilise l'OTA
// MISE A JOUR du firmware en Wifi
#define USE_OTA
#ifdef USE_OTA
// laisser (default) et il prendra automatiquement le nom du module
#define OTANAME "default"
// laisser (password) et aucun mot de passe ne sera demandé pour la mise a jour via l'OTA
#define OTAPASSWORD "password"
#endif
#endif

#endif