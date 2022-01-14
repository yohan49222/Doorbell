#ifndef IOB_IOT_h
#define IOB_IOT_h

#ifndef IOB_IOT_Conf_h
#include "IOB_IOT_Conf.h"
#endif

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

#define DEBUG true
#define LOG(line)               \
    {                           \
        if (DEBUG)              \
        {                       \
            Serial.print(line); \
        }                       \
    }
#define LOGLN(line)               \
    {                             \
        if (DEBUG)                \
        {                         \
            Serial.println(line); \
        }                         \
    }
#define LOGINLINE(line, var)     \
    {                            \
        if (DEBUG)               \
        {                        \
            Serial.print(line);  \
            Serial.println(var); \
        }                        \
    }

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
    String nomModule;
    int idxDevice;
    String ssid;
    String ssidPassword;
    bool canUseWifi = true;
    WiFiClient espClient;

#ifdef USE_IPFIXE
    IPAddress ipFixe;
    IPAddress ipGateWay;
    IPAddress ipSubnet;
    IPAddress ipDns;
    bool canUseIpFixe = true;
#endif /* USE_IPFIXE */

#ifdef USE_OTA
    String otaName;
    String otaPassword;
    bool canUseOta = true;
    bool canUseOtaPassword = true;
#endif /* USE_OTA */

#ifdef USE_HTTP
    IPAddress domoticzServerIp;
    int domoticzServerPort;
    HTTPClient httpClient;
    bool canUseHttp = true;
    bool CanSendViaHttp();
    bool CreateHttpMessageForDomoticz(String state, String &out);
#endif /* USE HTTP */

#ifdef USE_MQTT
    unsigned long previousMillisMQTT = 0;
    unsigned long intervalConnectMQTT = 1000;
    String topicIn;
    String topicOut;
    IPAddress mqttServerIp;
    int mqttPort;
    String mqttLogin;
    String mqttPassword;
    bool canUseMqtt = true;
    bool canUseMqttSecure = true;
    void ReconnectMQTT();
    bool CanSendViaMqtt();
    void ParseMqttMessage(char *topic, byte *message, unsigned int length);
    static void CallbackMQTT(char *topic, byte *message, unsigned int length);
    PubSubClient MQTT_Client;

#endif /* USE_MQTT */

#ifdef USE_WEBSERVER
    ESP8266WebServer webServer;
    int webServerPort;
    bool canUseWebServer = true;
    bool CreateJsonMessageForDebug(String &out);
    static void SwitchOn();
    static void SwitchOff();
    static void TraitRequestWeb(int state);
    static void DebugServeur();
#endif /* USE_WEBSERVER */

    IOB_IOTEventHandler<IOB_IOTMessageRecevedEventArgs> changeStateEventHandler;
    IOB_IOTEventHandler<IOB_IOTMessageSendedEventArgs> messageSendedEventHandler;
    IOB_IOTEventHandler<IOB_IOTMqttStateChangedEventArgs> mqttStateChangedEventHandler;
    IOB_IOTEventHandler<IOB_IOTWifiStateChangedEventArgs> wifiStateChangedEventHandler;
    

    static void OnConnected(const WiFiEventStationModeConnected &event);
    static void OnDisconnected(const WiFiEventStationModeDisconnected &event);
    static void OnGotIP(const WiFiEventStationModeGotIP &event);

    WiFiEventHandler onConnectedHandler;
    WiFiEventHandler onGotIPHandler;
    WiFiEventHandler onDisConnectedHandler;

    bool SendData(String send, int sendVia, IOB_IOTMessageSendedEventArgs &e);
    bool CreateJsonMessageForDomoticz(String state, String &out);
#endif /* USE_WIFI */
    IOB_IOTEventHandler<IOB_IOTButtonPressedEventArgs> buttonPressedEventHandler;
    int relayPin = 0;
    int buttonPin = 2;
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

public:
    static IOB_IOT *GetInstance();
    // IOB_IOT();
    ~IOB_IOT();
    void Loop();
    void Run();

    void SendData(String state);
    void SendData(int state);
    void OnMessageSend(std::function<void(IOB_IOTMessageSendedEventArgs &)> handler);
    void OnRecevChangeState(std::function<void(IOB_IOTMessageRecevedEventArgs &)> handler);
    void OnMqttStateChanged(std::function<void(IOB_IOTMqttStateChangedEventArgs &)> handler);
    void OnWifiStateChanged(std::function<void(IOB_IOTWifiStateChangedEventArgs &)> handler);


    void OnButtonPressed(std::function<void(IOB_IOTButtonPressedEventArgs &)> handler);
};

#endif /* IOB_IOT_h */