#ifndef IOB_IOT_H
#define IOB_IOT_H

#include "IOB_IOT/IOB_IOTMQTT.h"
#include "IOB_IOT/IOB_IOTHTTP.h"
#include "IOB_IOT/IOB_IOTOTA.h"
#include "IOB_IOT/IOB_IOTWEBSERVER.h"
#include "IOB_IOT/IotConfig.h"

class IOB_IOT : protected IOB_IOTHTTP, protected IOB_IOTMQTT, protected IOB_IOTOTA, protected IOB_IOTWEBSERVER
{
public:
     /*
     ok
     */
     using IOB_IOTMQTT::CanSendMqtt;
     using IOB_IOTMQTT::CanUseMqtt;
     using IOB_IOTMQTT::CanUseMqttSecure;
     using IOB_IOTMQTT::CreateJsonMessageForDomoticz;
     using IOB_IOTMQTT::get;
     using IOB_IOTMQTT::init;
     using IOB_IOTMQTT::LoopMqtt;
     using IOB_IOTMQTT::mqtt_Recep_EventHandler;
     using IOB_IOTMQTT::mqtt_Send_EventHandler;
     using IOB_IOTMQTT::mqtt_State_Changed_EventHandler;
     using IOB_IOTMQTT::ParseMqttMessage;
     using IOB_IOTMQTT::ReconnectMQTT;
     using IOB_IOTMQTT::Sendata;
     /* a void */
     using IOB_IOTHTTP::CreateHttpMessageForDomoticz;
     using IOB_IOTHTTP::getDomotic;
     using IOB_IOTHTTP::http_Send_EventHandler;
     using IOB_IOTHTTP::Sendata;

     using IOB_IOTWEBSERVER::CreateJsonMessageForDebug;
     using IOB_IOTWEBSERVER::init;
     using IOB_IOTWEBSERVER::Loop;
     using IOB_IOTWEBSERVER::webServer;
     using IOB_IOTWEBSERVER::webServer_Request_EventHandler;
     using IOB_IOTWEBSERVER::webServer_Response_EventHandler;

private:
     static IOB_IOT *inst_; // The one, single instance
     IOB_IOT();
     IOB_IOT(const IOB_IOT &);
     IOB_IOT &operator=(const IOB_IOT &);

#ifdef USE_WIFI

     IOB_IOTWifiStateEventHandler wifiStateChangedEventHandler;

     static void OnConnected(const WiFiEventStationModeConnected &event);
     static void OnDisconnected(const WiFiEventStationModeDisconnected &event);
     static void OnGotIP(const WiFiEventStationModeGotIP &event);

     WiFiEventHandler onConnectedHandler;
     WiFiEventHandler onGotIPHandler;
     WiFiEventHandler onDisConnectedHandler;

     bool SendData(String send, int sendVia, IOB_IOTMessageSendedEventArgs &e);

#endif /* USE_WIFI */


     IOB_IOTButtonPressedEventHandler buttonPressedEventHandler;
     volatile uint32_t debounceTimer = 0;
     uint32_t buttonPresseCount = 0;
     static void IRAM_ATTR ButtonPressed();
     

public:
     static IOB_IOT *GetInstance();
     ~IOB_IOT();
     void Loop();
     void Run();
     IotConfig getConf();
     void SendData(String state);
     void SendData(RelayState state);
     WiFiClient espClient;


     void OnMqttSend(std::function<void(IOB_IOTMessageSendedEventArgs &)> handler);
     void OnMqttRecep(std::function<void(IOB_IOTMessageRecevedEventArgs &)> handler);
     void OnMqttStateChanged(std::function<void(IOB_IOTMqttStateChangedEventArgs &)> handler);

     void OnWebSend(std::function<void(IOB_IOTMessageSendedEventArgs &)> handler);
     void OnWebRecep(std::function<void(IOB_IOTMessageRecevedEventArgs &)> handler);

     void OnHttpSend(std::function<void(IOB_IOTMessageSendedEventArgs &)> handler);

     void OnWifiStateChanged(std::function<void(IOB_IOTWifiStateChangedEventArgs &)> handler);
     void OnButtonPressed(std::function<void(IOB_IOTButtonPressedEventArgs &)> handler);
};

//#define CANUSEWIFI
//#define CANUSEMQTT IOB_IOTMQTT::getMqttIp().isSet() && IOB_IOTMQTT::getIdx() > 0
//#define CANUSEMQTTSECURE IOB_IOTMQTT::getMqtt(). != emptyString &&IOB_IOTMQTT::getMqttPassword() != emptyString &&CANUSEMQTT
//#define CANSENDVIAMQTT
//#define CANUSEHTTP IOB_IOTMQTT::getDomoIp().isSet() && IOB_IOTMQTT::get().getRequired().idxDevice > 0
//#define CANSENDVIAHTTP CANUSEHTTP &&WiFi.isConnected()
//#define CANUSEOTASECURE IOB_IOTMQTT::getConfigOta().password != emptyString

#endif /* IOB_IOT_H */