#ifndef IOB_IOT_h
#define IOB_IOT_h

///#include "IOB_IOT_public.h"
#include "IOB_IOT_private.h"
#include "wifi_config.h"

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#ifdef USE_WEBSERVER
#include <ESP8266WebServer.h>
#endif // USE_WEBSERVER

#ifdef USE_HTTP
#include <ESP8266HTTPClient.h>
#endif

#ifdef USE_MQTT
#include <PubSubClient.h>
#endif // USE_MQTT

#include "IOB_IOTEvent.h"

enum SendDataMethod : int
{
    SENDBY_HTTP_ONLY = 1,
    SENDBY_MQTT_ONLY = 2
};

class IOB_IOT
{
private:
    static IOB_IOT *inst_; // The one, single instance
    IOB_IOT();
    IOB_IOT(const IOB_IOT &);
    IOB_IOT &operator=(const IOB_IOT &);

#ifdef USE_WIFI
    String NomModule;
    int IdxDevice;
    String Ssid;
    String SsidPassword;
    WiFiClient espClient;

#ifdef USE_IPFIXE
    IPAddress ipFixe;
    IPAddress ipGateWay;
    IPAddress ipSubnet;
    IPAddress ipDns;
#endif

#ifdef USE_OTA
    String OtaName;
    String OtaPassword;
#endif

#ifdef USE_HTTP
    String TopicIn;
    String TopicOut;
    IPAddress DomoticzServerIp;
    int DomoticzServerPort;
    HTTPClient httpClient;
#endif

#ifdef USE_MQTT
    IPAddress MqttServerIp;
    int MqttPort;
    String MqttLogin;
    String MqttPassword;

    void reconnectMQTT();
    PubSubClient MQTT_Client; //(config->getMqttServer().c_str(), config->getMqttPort(), callbackMQTT, espClient);
#endif

#ifdef USE_WEBSERVER
    int WebServerPort;
    ESP8266WebServer webServer;
#endif

    bool canUseWifi = true;
    bool canUseIpFixe = true;
    bool canUseMqtt = true;
    bool canUseMqttSecure = true;
    bool canUseHttp = true;
    bool canUseOta = true;
    bool canUseOtaPassword = true;
    bool canUseWebServer = true;

#else
    bool canUseWifi = false;
    bool canUseIpFixe = false;
    bool canUseMqtt = false;
    bool canUseMqttSecure = false;
    bool canUseHttp = false;
    bool canUseOta = false;
    bool canUseOtaPassword = false;
    bool canUseWebServer = false;
#endif
    unsigned long previousMillisMQTT = 0;
    unsigned long intervalConnectMQTT = 1000;
    bool EqualString(String stest, String stestto);
    bool definedString(String stest);
    bool definedInt(const int itest);
    IPAddress parsedIpFromString(String sip);
    String generateRamdomModuleNane();
    void SendData(String send, int sendVia);
    bool CanSendViaMqtt();
    bool CanSendViaHttp();
    void parseMqttMessage(char *topic, byte *message, unsigned int length);
    IOB_IOTEvent<IOB_IOTEventArgs> changeStateEvent;

public:
    static IOB_IOT *getInstance();
    // IOB_IOT();
    ~IOB_IOT();
    void loop();
    /*bool CanUseWifi();
    bool CanUseMqtt();
    bool CanUseMqttSecure();
    bool CanUseHttp();*/
    static void callbackMQTT(char *topic, byte *message, unsigned int length);
    void SendData(String state);
#ifdef USE_WIFI
    void SetUp();

    static void switchOn();
    static void switchOff();
    static void DebugServeur();

    bool CreateJsonMessageForDebug(String &out);
    bool CreateHttpMessageForDomoticz(String state, String &out);
    bool CreateJsonMessageForDomoticz(String state, String &out);

    static void onConnected(const WiFiEventStationModeConnected &event);
    static void onDisconnected(const WiFiEventStationModeDisconnected &event);
    static void onGotIP(const WiFiEventStationModeGotIP &event);

    WiFiEventHandler onConnectedHandler;
    WiFiEventHandler onGotIPHandler;
    WiFiEventHandler onDisConnectedHandler;
    /*
        String getMqttServer();
        int getMqttPort();
        String getTopicOut();
        String getNomModule();
        String getMqttLogin();
        String getMqttPassword();
    */

    void OnRecevChangeState(std::function<void(IOB_IOTEventArgs &)> handler);
#endif
};

#endif
