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

#define DEBUG false
#define LOG(line) { if(DEBUG) { Serial.print(line); } }
#define LOGLN(line) { if(DEBUG) { Serial.println(line); } }

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
    bool canUseIpFixe = true;
#endif

#ifdef USE_OTA
    String OtaName;
    String OtaPassword;
    bool canUseOta = true;
    bool canUseOtaPassword = true;
#endif

#ifdef USE_HTTP
    IPAddress DomoticzServerIp;
    int DomoticzServerPort;
    HTTPClient httpClient;
    bool canUseHttp = true;
    bool CanSendViaHttp();
    bool CreateHttpMessageForDomoticz(String state, String &out);
#endif

#ifdef USE_MQTT    
    unsigned long previousMillisMQTT = 0;
    unsigned long intervalConnectMQTT = 1000;
    String TopicIn;
    String TopicOut;
    IPAddress MqttServerIp;
    int MqttPort;
    String MqttLogin;
    String MqttPassword;
    bool canUseMqtt = true;
    bool canUseMqttSecure = true;
    void reconnectMQTT();
    bool CanSendViaMqtt();
    void parseMqttMessage(char *topic, byte *message, unsigned int length);
    static void callbackMQTT(char *topic, byte *message, unsigned int length);
    PubSubClient MQTT_Client; //(config->getMqttServer().c_str(), config->getMqttPort(), callbackMQTT, espClient);
    bool CreateJsonMessageForDomoticz(String state, String &out);
#endif

#ifdef USE_WEBSERVER
    int WebServerPort;
    ESP8266WebServer webServer;
    bool canUseWebServer = true;
    bool CreateJsonMessageForDebug(String &out);    
    static void switchOn();
    static void switchOff();
    static void DebugServeur();
#endif

    bool canUseWifi = true;

#endif

    bool EqualString(String stest, String stestto);
    bool definedString(String stest);
    bool definedInt(const int itest);
    IPAddress parsedIpFromString(String sip);
    String generateRamdomModuleNane();
    void SendData(String send, int sendVia);

    IOB_IOTEvent<IOB_IOTEventArgs> changeStateEvent;

    

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
public:
    static IOB_IOT *getInstance();
    // IOB_IOT();
    ~IOB_IOT();
    void loop();
    void SetUp();
    void SendData(String state);
    void SendData(int state);
    void OnRecevChangeState(std::function<void(IOB_IOTEventArgs &)> handler);
};

#endif