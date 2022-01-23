#ifndef IOB_IOT_H
#define IOB_IOT_H

#ifndef IOB_IOT_CONF_H
#include "IOB_IOT_Conf.h"
#endif /* IOB_IOT_CONF_H */

#include "IOB_IOT/SHARE/SharedFunction.h"
#include "IOB_IOT/SHARE/IotConfig.h"
#ifdef USE_WIFI
#include "IOB_IOT/IOB_IOTWIFI.h"
//#include <ESP8266WiFi.h>
#ifdef USE_MQTT
#include "IOB_IOT/IOB_IOTMQTT.h"
#endif
#ifdef USE_HTTP
#include "IOB_IOT/IOB_IOTHTTP.h"
#endif
#ifdef USE_OTA
#include "IOB_IOT/IOB_IOTOTA.h"
#endif
#ifdef USE_WEBSERVER
#include "IOB_IOT/IOB_IOTWEBSERVER.h"
#endif
#else
#include "Arduino.h"
#endif
#include "IOB_IOT/EVENTS/IOB_IOTEventArg.h"
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"
#include "IOB_IOT/EVENTS/IOB_IOTButtonPressed.h"



class IOB_IOT : public IotConfig, public SharedFunction
#ifdef USE_WIFI
    ,
                private IOB_IOTWIFI
#ifdef USE_HTTP
    ,
                private IOB_IOTHTTP
#endif
#ifdef USE_MQTT
    ,
                private IOB_IOTMQTT
#endif
#ifdef USE_OTA
    ,
                private IOB_IOTOTA
#endif
#ifdef USE_WEBSERVER
    ,
                private IOB_IOTWEBSERVER
#endif
#endif
{
public:
#ifdef USE_WIFI
     using IOB_IOTWIFI::espClient;
     using IOB_IOTWIFI::wifiStateChangedEventHandler;
     using IOB_IOTWIFI::Begin;
     using IOB_IOTWIFI::Loop;
#ifdef USE_OTA
     using IOB_IOTOTA::init;
#endif

#ifdef USE_MQTT
     using IOB_IOTMQTT::CanSendMqtt;
     using IOB_IOTMQTT::CanUseMqtt;
     using IOB_IOTMQTT::CanUseMqttSecure;
     using IOB_IOTMQTT::init;
     using IOB_IOTMQTT::LoopMqtt;
     using IOB_IOTMQTT::mqtt_Recep_EventHandler;
     using IOB_IOTMQTT::mqtt_Send_EventHandler;
     using IOB_IOTMQTT::mqtt_State_Changed_EventHandler;
     using IOB_IOTMQTT::ParseMqttMessage;
     using IOB_IOTMQTT::ReconnectMQTT;
     using IOB_IOTMQTT::Sendata;
#endif

#ifdef USE_HTTP
     using IOB_IOTHTTP::CreateHttpMessageForDomoticz;
     using IOB_IOTHTTP::http_Send_EventHandler;
     using IOB_IOTHTTP::Sendata;
#endif
#if defined(USE_HTTP) or defined(USE_WEBSERVER) or defined(USE_MQTT)
     using SharedFunction::CreateJsonMessageForDomoticz;
#endif
#ifdef USE_WEBSERVER
     using IOB_IOTWEBSERVER::CreateJsonMessageForDebug;
     using IOB_IOTWEBSERVER::init;
     using IOB_IOTWEBSERVER::Loop;
     using IOB_IOTWEBSERVER::webServer;
     using IOB_IOTWEBSERVER::webServer_Request_EventHandler;
     using IOB_IOTWEBSERVER::webServer_Response_EventHandler;
#endif
#endif

private:
     static IOB_IOT *inst_;
     IOB_IOT();
     IOB_IOT(const IOB_IOT &);
     IOB_IOT &operator=(const IOB_IOT &);

#ifdef USE_WIFI

     

     //static void OnConnected(const WiFiEventStationModeConnected &event);
     //static void OnDisconnected(const WiFiEventStationModeDisconnected &event);
     //static void OnGotIP(const WiFiEventStationModeGotIP &event);

     //WiFiEventHandler onConnectedHandler;
     //WiFiEventHandler onGotIPHandler;
     //WiFiEventHandler onDisConnectedHandler;

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

     void SendData(RelayState state);
#ifdef USE_WIFI
     //WiFiClient espClient;
     
#endif
     void OnMqttSend(MqttSend handler);
     void OnMqttRecep(MqttRecep handler);
     void OnMqttStateChanged(MqttState handler);
     void OnWebSend(WebSend handler);
     void OnWebRecep(WebRecep handler);
     void OnHttpSend(HttpSend handler);
     void OnWifiStateChanged(WifiState handler);
     void OnButtonPressed(BtPress handler);
};

#endif /* IOB_IOT_H */