#include "IOB_IOT/SHARE/IotConfig.h"

IotConfig::IotConfig()
{
#ifdef RELAY_AUTO_OFF
     required.relayAutoOff = DefinedInt(RELAY_AUTO_OFF) ? RELAY_AUTO_OFF : 1;
#endif

#ifdef RELAY_AUTO_OFF_AFTER
     required.relayAutoOffAfter = DefinedInt(RELAY_AUTO_OFF_AFTER) ? RELAY_AUTO_OFF_AFTER: 200;
#endif

#ifdef RELAY_PIN
     required.relayPin = DefinedInt(RELAY_PIN) ? RELAY_PIN : 0;
#endif

#ifdef BUTTON_PIN
     required.buttonPin = DefinedInt(BUTTON_PIN) ? BUTTON_PIN : 2;
#endif

#ifdef NCORNO
     required.relayNcOrNo = NCORNO;
#endif

#ifdef DEBOUNCE_TIME
     required.debounceTime = DefinedInt(DEBOUNCE_TIME) ? DEBOUNCE_TIME : 100;
#endif

#ifdef BUTTON_PRESS_COUNT_MAX
     required.buttonPresseCountMax = DefinedInt(BUTTON_PRESS_COUNT_MAX) ? BUTTON_PRESS_COUNT_MAX : 5;
#endif

#ifdef NOMMODULE
     required.nomModule = DefinedString((String)NOMMODULE) && !EqualString((String)NOMMODULE, "default") ? (String)NOMMODULE : GenerateRamdomModuleNane();
#endif

#ifdef IDXDEVICE
     required.idxDevice = IDXDEVICE;
#endif

#ifdef USE_WIFI
#ifdef MYSSID
     configWifi.ssid = DefinedString((String)MYSSID) && !EqualString((String)MYSSID, "my_ssid") ? (String)MYSSID : emptyString;
#endif

#ifdef MYSSID_PASSWORD
     configWifi.password = DefinedString((String)MYSSID_PASSWORD) && !EqualString((String)MYSSID_PASSWORD, "my_ssid_password") ? (String)MYSSID_PASSWORD : emptyString;
#endif

#ifdef USE_MQTT
#ifdef MQTT_SERVER
     mqtt.ip = IPAddress::isValid((String)MQTT_SERVER) ? ParsedIpFromString((String)MQTT_SERVER) : INADDR_NONE;
#endif

#ifdef MQTT_PORT
     mqtt.port = DefinedInt(MQTT_PORT) ? MQTT_PORT : 1883;
#endif

#ifdef TOPICIN
     mqtt.topicIn = DefinedString((String)TOPICIN) ? (String)TOPICIN : "domoticz/out";
#endif

#ifdef TOPICOUT
     mqtt.topicOut = DefinedString((String)TOPICOUT) ? ((String)TOPICOUT) : "domoticz/in";
#endif

#ifdef MQTT_LOGIN
     mqtt.login = DefinedString((String)MQTT_LOGIN) && !EqualString((String)MQTT_LOGIN, "login") ? (String)MQTT_LOGIN : emptyString;

#endif

#ifdef MQTT_PASSWORD
     mqtt.password = DefinedString((String)MQTT_PASSWORD) && !EqualString((String)MQTT_PASSWORD, "password") ? (String)MQTT_PASSWORD : emptyString;
#endif

#ifdef INTERVALCONNECT
     mqtt.intervalConnect = DefinedInt(INTERVALCONNECT) ? INTERVALCONNECT : 1000;
#endif

#endif

#ifdef USE_HTTP
#ifdef DOMOTIC_SERVER
     domo.ip = IPAddress::isValid((String)DOMOTIC_SERVER) ? ParsedIpFromString((String)DOMOTIC_SERVER) : INADDR_NONE;
#endif

#ifdef DOMOTIC_PORT
     domo.port = DefinedInt(DOMOTIC_PORT) ? DOMOTIC_PORT : 8080;
#endif
#endif

#ifdef USE_IPFIXE
#ifdef IPFIXE
     configIp.ip = IPAddress::isValid((String)IPFIXE) ? ParsedIpFromString((String)IPFIXE) : INADDR_NONE;
#endif

#ifdef GATEWAY
     configIp.gateWay = IPAddress::isValid((String)GATEWAY) ? ParsedIpFromString((String)GATEWAY) : INADDR_NONE;
#endif

#ifdef SUBNET
     configIp.subnet = IPAddress::isValid((String)SUBNET) ? ParsedIpFromString((String)SUBNET) : INADDR_NONE;
#endif

#ifdef DNS
     configIp.dns = IPAddress::isValid((String)DNS) ? ParsedIpFromString((String)DNS) : INADDR_NONE;
#endif

#endif

#ifdef USE_OTA
#ifdef OTANAME
     configOta.name = DefinedString((String)OTANAME) && !EqualString((String)OTANAME, "default") ? (String)OTANAME : required.nomModule;
#endif

#ifdef OTAPASSWORD
     configOta.password = DefinedString((String)OTAPASSWORD) && !EqualString((String)OTAPASSWORD, "password") ? (String)OTAPASSWORD : emptyString;
#endif
#endif

#ifdef USE_WEBSERVER
#ifdef WEBSERVER_PORT
     webServerPort = DefinedInt(WEBSERVER_PORT) ? WEBSERVER_PORT : 80;
#endif
#endif
#endif
}

Required IotConfig::getRequired()
{
     return required;
}

#ifdef USE_WIFI
WifiConfig IotConfig::getConfigWifi()
{
     return configWifi;
}
#ifdef USE_IPFIXE
IpConfig IotConfig::getConfigIp()
{
     return configIp;
}
#endif
#ifdef USE_OTA
OtaConfig IotConfig::getConfigOta()
{
     return configOta;
}
#endif
#ifdef USE_MQTT
void IotConfig::setPreviousMillis(unsigned long i)
{
     mqtt.previousMillis = i;
}
Mqtt IotConfig::getMqtt()
{
     return mqtt;
}
#endif
#ifdef USE_HTTP
Domotic IotConfig::getDomotic()
{
     return domo;
}
#endif
#ifdef USE_WEBSERVER
uint32_t IotConfig::getWebServerPort()
{
     return webServerPort;
}
#endif
#endif
