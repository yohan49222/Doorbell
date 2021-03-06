#include "IOB_IOT.h"

// Define the static Singleton pointer
IOB_IOT *IOB_IOT::inst_ = NULL;

IOB_IOT *IOB_IOT::GetInstance()
{
     if (inst_ == NULL)
     {
          inst_ = new IOB_IOT();
     }
     return (inst_);
}

IOB_IOT::~IOB_IOT()
{
}

IOB_IOT::IOB_IOT() : IotConfig()
#ifdef USE_WIFI
    ,IOB_IOTWIFI()
#ifdef USE_OTA
    ,IOB_IOTOTA()
#endif /* USE_OTA */
#ifdef USE_MQTT
    ,IOB_IOTMQTT()
#endif /* USE_MQTT */
#ifdef USE_WEBSERVER
    ,IOB_IOTWEBSERVER()
#endif /* USE_WEBSERVER */
#ifdef USE_HTTP
    ,IOB_IOTHTTP()
#endif /* USE_HTTP */
#endif /* USE_WIFI */
{

}

void IRAM_ATTR IOB_IOT::ButtonPressed()
{
     IOB_IOT *inst = IOB_IOT::GetInstance();
     if (millis() - inst->getRequired().debounceTime >= inst->debounceTimer)
     {
          inst->debounceTimer = millis();
          inst->buttonPresseCount += 1;
          //Serial.printf("Button has been pressed %u times\n", inst->buttonPresseCount);
     }
}

/* FONCTIONS PUBLIQUE */
void IOB_IOT::Run()
{
     Required req = getRequired();

     pinMode(req.relayPin, OUTPUT);
     pinMode(req.buttonPin, INPUT_PULLUP);
     attachInterrupt(req.buttonPin, ButtonPressed, FALLING);
     digitalWrite(req.relayPin, req.relayNcOrNo ? HIGH : LOW);

#ifdef USE_WIFI

#ifdef USE_OTA
     //IOB_IOTOTA::init(this);
#endif

     //IOB_IOTWIFI::Begin(this);

#ifdef USE_WEBSERVER
     //IOB_IOTWEBSERVER::init(this);
#endif

#ifdef USE_MQTT
     //IOB_IOTMQTT::init(this);
#endif

#endif /* USE_WIFI */
}

void IOB_IOT::Loop()
{
     Required req = getRequired();
     if (buttonPresseCount >= req.buttonPresseCountMax)
     {
          buttonPresseCount = 0;
          IOB_IOTButtonPressedEventArgs e = IOB_IOTButtonPressedEventArgs("button pressed");
          buttonPressedEventHandler.fire(e);
          if (!e.Handled())
          {
               digitalWrite(req.relayPin, req.relayNcOrNo ? LOW : HIGH);
               if(req.relayAutoOff)
               {
                    delay(req.relayAutoOffAfter);
                    digitalWrite(req.relayPin, req.relayNcOrNo ? HIGH : LOW);                    
               }

          }
     }


#ifdef USE_WIFI

     bool connected = IOB_IOTWIFI::Loop();
     if(!connected)
          return; /** break loop */

#ifdef USE_MQTT
          IOB_IOTMQTT::Loop();
#endif

#ifdef USE_OTA
          ArduinoOTA.begin();
#endif /* USE_OTA */

#ifdef USE_WEBSERVER
          IOB_IOTWEBSERVER::Loop();
#endif /* USE_WEBSERVER */

#ifdef USE_OTA
          ArduinoOTA.handle();
#endif /* USE_OTA */
#endif /* USE_WIFI */
}

void IOB_IOT::OnMqttRecep(MqttRecep handler)
{
#ifdef USE_WIFI
#ifdef USE_MQTT
     mqtt_Recep_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnMqttSend(MqttSend handler)
{
#ifdef USE_WIFI
#ifdef USE_MQTT
     mqtt_Send_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnWebSend(WebSend handler)
{
#ifdef USE_WIFI
#ifdef USE_WEBSERVER
     webServer_Response_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnWebRecep(WebRecep handler)
{
#ifdef USE_WIFI
#ifdef USE_WEBSERVER
     webServer_Request_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnHttpSend(HttpSend handler)
{
#ifdef USE_WIFI
#ifdef USE_HTTP
     http_Send_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnMqttStateChanged(MqttState handler)
{
#ifdef USE_WIFI
#ifdef USE_MQTT
     mqtt_State_Changed_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnWifiStateChanged(WifiState handler)
{
#ifdef USE_WIFI
     wifiStateChangedEventHandler.setHandler(handler);
#endif /* USE_WIFI */
}

void IOB_IOT::OnButtonPressed(BtPress handler)
{
     buttonPressedEventHandler.setHandler(handler);
}

void IOB_IOT::SendData(RelayState state)
{

#ifdef USE_WIFI
#if defined(USE_MQTT) or defined(USE_HTTP)
     bool sendSuccess = false;

#ifdef USE_MQTT
     if (WiFi.isConnected())
     {
          sendSuccess = IOB_IOTMQTT::Sendata(state);
          return;
     }
#endif /* USE MQTT */

#ifdef USE_HTTP
     if (!sendSuccess)
     {
          sendSuccess = IOB_IOTHTTP::Sendata(state);
     }
#endif /* USE HTTP */
#endif
#endif /* USE_WIFI */
}


