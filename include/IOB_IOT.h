#ifndef IOB_IOT_H
#define IOB_IOT_H

#ifndef IOB_IOT_CONF_H
#include "IOB_IOT_Conf.h"
#endif /* IOB_IOT_CONF_H */

#ifdef USE_WIFI

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#ifdef USE_WEBSERVER
#include <ESP8266WebServer.h>
#endif /* USE_WEBSERVER */
#ifdef USE_HTTP
#include <ESP8266HTTPClient.h>
#endif
#ifdef USE_MQTT
#include <PubSubClient.h>
#endif /* USE_MQTT */

#else 
#ifndef Arduino_h
#include "Arduino.h"
#include "IPAddress.h"
#endif
#endif /* USE_WIFI */

#ifndef IOB_IOTEVENT_H
#include "IOB_IOTEvent.h"
#endif /* IOB_IOTEVENT_H */

#ifndef IOB_IOTBUTTONPRESSED_H
#include "IOB_IOTButtonPressed.h"
#endif /* IOB_IOTBUTTONPRESSED_H */



class IOB_IOT
{
private:
    static IOB_IOT *inst_; // The one, single instance
    IOB_IOT();
    IOB_IOT(const IOB_IOT &);
    IOB_IOT &operator=(const IOB_IOT &);

#ifdef USE_WIFI

    String nomModule;
    uint32_t idxDevice;
    String ssid;
    String ssidPassword;
    //bool canUseWifi = true;
    WiFiClient espClient;

#ifdef USE_IPFIXE
    IPAddress ipFixe;
    IPAddress ipGateWay;
    IPAddress ipSubnet;
    IPAddress ipDns;
#endif /* USE_IPFIXE */

#ifdef USE_OTA
    String otaName;
    String otaPassword;
#endif /* USE_OTA */

#ifdef USE_HTTP
    IPAddress domoticzServerIp;
    uint32_t domoticzServerPort;
    HTTPClient httpClient;
    bool CreateHttpMessageForDomoticz(String state, String &out);
#endif /* USE HTTP */

#ifdef USE_MQTT
    unsigned long previousMillisMQTT = 0;
    unsigned long intervalConnectMQTT = 1000;
    String topicIn;
    String topicOut;
    IPAddress mqttServerIp;
    uint32_t mqttPort;
    String mqttLogin;
    String mqttPassword;
    void ReconnectMQTT();
    void ParseMqttMessage(char *topic, byte *message, unsigned int length);
    static void CallbackMQTT(char *topic, byte *message, unsigned int length);
    PubSubClient MQTT_Client;
#endif /* USE_MQTT */

#ifdef USE_WEBSERVER
    ESP8266WebServer webServer;
    unsigned int webServerPort;
    bool CreateJsonMessageForDebug(String &out);
    static void SwitchOn();
    static void SwitchOff();
    static void TraitRequestWeb(int state);
    static void DebugServeur();
#endif /* USE_WEBSERVER */

    IOB_IOTMessageRecevedEventHandler changeStateEventHandler;
    IOB_IOTMessageSendedventHandler messageSendedEventHandler;
    IOB_IOTMqttStateEventHandler mqttStateChangedEventHandler;
    IOB_IOTWifiStateEventHandler wifiStateChangedEventHandler;
    

    static void OnConnected(const WiFiEventStationModeConnected &event);
    static void OnDisconnected(const WiFiEventStationModeDisconnected &event);
    static void OnGotIP(const WiFiEventStationModeGotIP &event);

    WiFiEventHandler onConnectedHandler;
    WiFiEventHandler onGotIPHandler;
    WiFiEventHandler onDisConnectedHandler;

    bool SendData(String send, int sendVia, IOB_IOTMessageSendedEventArgs &e);
    bool CreateJsonMessageForDomoticz(String state, String &out);

#endif /* USE_WIFI */

    IOB_IOTButtonPressedEventHandler buttonPressedEventHandler;
    uint32_t relayPin = 0;
    uint32_t buttonPin = 2;
    bool relayNcOrNo = true;

    bool EqualString(String stest, String stestto);
    bool DefinedString(String stest);
    bool DefinedInt(const int itest);
    IPAddress ParsedIpFromString(String sip);
    String GenerateRamdomModuleNane();


    volatile uint32_t debounceTimer = 0;
    uint32_t debounceTime = 0;
    uint32_t buttonPresseCountMax = 0;
    uint32_t buttonPresseCount = 0;
    static void IRAM_ATTR ButtonPressed();
void SendData(SendData_t state);
public:
    static IOB_IOT *GetInstance();
    ~IOB_IOT();
    void Loop();
    void Run();
    void SendData(String state);
    

    void OnMessageSend(std::function<void(IOB_IOTMessageSendedEventArgs &)> handler);
    void OnRecevChangeState(std::function<void(IOB_IOTMessageRecevedEventArgs &)> handler);
    void OnMqttStateChanged(std::function<void(IOB_IOTMqttStateChangedEventArgs &)> handler);
    void OnWifiStateChanged(std::function<void(IOB_IOTWifiStateChangedEventArgs &)> handler);


    void OnButtonPressed(std::function<void(IOB_IOTButtonPressedEventArgs &)> handler);
};


#define CANUSEWIFI ssid != emptyString && ssidPassword != emptyString
#define CANUSEMQTT mqttServerIp.isSet() && idxDevice > 0
#define CANUSEMQTTSECURE mqttLogin != emptyString && mqttPassword != emptyString && CANUSEMQTT
#define CANSENDVIAMQTT CANUSEMQTT && WiFi.isConnected() && MQTT_Client.connected()
#define CANUSEHTTP domoticzServerIp.isSet() && idxDevice > 0
#define CANSENDVIAHTTP CANUSEHTTP && WiFi.isConnected()
#define CANUSEOTASECURE otaPassword != emptyString
#define CANUSEIPFIXE ipFixe.isSet() && ipGateWay.isSet() && ipSubnet.isSet() && ipDns.isSet()

#endif /* IOB_IOT_H */