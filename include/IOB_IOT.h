#ifndef IOB_IOT_H
#define IOB_IOT_H

#ifndef IOB_IOT_CONF_H
#include "IOB_IOT_Conf.h"
#endif /* IOB_IOT_CONF_H */

#include "IOB_IOT/SHARE/SharedFunction.h"
#include "IOB_IOT/SHARE/IotConfig.h"

#ifdef USE_WIFI
#include "IOB_IOT/IOB_IOTWIFI.h"
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



class IOB_IOT : public SharedFunction, public IotConfig 
#ifdef USE_WIFI
    ,private IOB_IOTWIFI
#ifdef USE_OTA
    ,private IOB_IOTOTA
#endif /* USE_OTA */
#ifdef USE_MQTT
    ,private IOB_IOTMQTT
#endif /* USE_MQTT */
#ifdef USE_WEBSERVER
    ,private IOB_IOTWEBSERVER
#endif /* USE_WEBSERVER */
#ifdef USE_HTTP
    ,private IOB_IOTHTTP
#endif /* USE_HTTP */
#endif /* USE_WIFI */
{

public:

#ifdef USE_WIFI

     using IOB_IOTWIFI::espClient;
     using IOB_IOTWIFI::wifiStateChangedEventHandler;

#ifdef USE_OTA
     //using IOB_IOTOTA::init;
#endif /* USE_OTA */

#ifdef USE_MQTT

     using IOB_IOTMQTT::mqtt_Recep_EventHandler;
     using IOB_IOTMQTT::mqtt_Send_EventHandler;
     using IOB_IOTMQTT::mqtt_State_Changed_EventHandler;
     using IOB_IOTMQTT::ParseMqttMessage; //static
     using IOB_IOTMQTT::GetTopicIn; // required by static methode IOB_IOTMQTT::ParseMqttMessage
     using IOB_IOTMQTT::GetMqttIp;

#endif /* USE_MQTT */

#ifdef USE_HTTP
     using IOB_IOTHTTP::http_Send_EventHandler;
     using IOB_IOTHTTP::GetHttpIp;

#endif /* USE_HTTP */

#if defined(USE_HTTP) or defined(USE_WEBSERVER) or defined(USE_MQTT)
     using SharedFunction::CreateJsonMessageForDomoticz;
#endif /* */

#ifdef USE_WEBSERVER
     using IOB_IOTWEBSERVER::CreateJsonMessageForDebug; //static
     using IOB_IOTWEBSERVER::webServer;
     using IOB_IOTWEBSERVER::webServer_Request_EventHandler;
     using IOB_IOTWEBSERVER::webServer_Response_EventHandler;
#endif /* USE_WEBSERVER */

#endif /* USE_WIFI */

     static IOB_IOT *GetInstance();
     ~IOB_IOT();
     void Loop();
     void Run();

     void SendData(RelayState state);

     void OnMqttSend(MqttSend handler);
     void OnMqttRecep(MqttRecep handler);
     void OnMqttStateChanged(MqttState handler);
     void OnWebSend(WebSend handler);
     void OnWebRecep(WebRecep handler);
     void OnHttpSend(HttpSend handler);
     void OnWifiStateChanged(WifiState handler);
     void OnButtonPressed(BtPress handler);


private:
     static IOB_IOT *inst_;
     IOB_IOT();
     IOB_IOT(const IOB_IOT &);
     IOB_IOT &operator=(const IOB_IOT &);
     IOB_IOTButtonPressedEventHandler buttonPressedEventHandler;
     volatile uint32_t debounceTimer = 0;
     uint32_t buttonPresseCount = 0;
     static void IRAM_ATTR ButtonPressed();
};

#endif /* IOB_IOT_H */