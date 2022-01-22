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
{

}

void IRAM_ATTR IOB_IOT::ButtonPressed()
{
     IOB_IOT *inst = IOB_IOT::GetInstance();
     if (millis() - inst->getRequired().debounceTime >= inst->debounceTimer)
     {
          inst->debounceTimer = millis();
          inst->buttonPresseCount += 1;
          Serial.printf("Button has been pressed %u times\n", inst->buttonPresseCount);
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
     IOB_IOTOTA::init(this);
#endif
     WifiConfig w = getConfigWifi();
     if (w.ssid != emptyString && w.password != emptyString)
     {
          WiFi.mode(WIFI_STA);

#ifdef USE_IPFIXE
          IpConfig ips = getConfigIp();
          if (ips.ip.isSet() && ips.gateWay.isSet() && ips.subnet.isSet() && ips.dns.isSet())
               WiFi.config(ips.ip, ips.gateWay, ips.subnet, ips.dns);
#endif /* USE_IPFIXE */

          WiFi.setHostname(req.nomModule.c_str());
          WiFi.begin(w.ssid, w.password);

          onConnectedHandler = WiFi.onStationModeConnected(IOB_IOT::OnConnected);
          onGotIPHandler = WiFi.onStationModeGotIP(IOB_IOT::OnGotIP);
          onDisConnectedHandler = WiFi.onStationModeDisconnected(IOB_IOT::OnDisconnected);
     }

#ifdef USE_WEBSERVER
     IOB_IOTWEBSERVER::init(this);
#endif

#ifdef USE_MQTT
     if (CanUseMqtt(this))
          IOB_IOTMQTT::init(this);
#endif

#endif /* USE_WIFI */
}

void IOB_IOT::Loop()
{
#ifdef USE_WIFI

     WifiConfig w = getConfigWifi();
     if (WiFi.isConnected())
     {
#ifdef USE_MQTT
          LoopMqtt(this);
#endif
     }
     else if (w.ssid != emptyString && w.password != emptyString)
     {
          if (WiFi.waitForConnectResult() != WL_CONNECTED)
               return;
#ifdef USE_OTA
          ArduinoOTA.begin();
#endif /* USE_OTA */
     }

#ifdef USE_WEBSERVER
     IOB_IOTWEBSERVER::Loop(this);
#endif /* USE_WEBSERVER */

#ifdef USE_OTA
     ArduinoOTA.handle();
#endif /* USE_OTA */

#endif /* USE_WIFI */

     Required req = getRequired();
     if (buttonPresseCount >= req.buttonPresseCountMax)
     {
          buttonPresseCount = 0;
          IOB_IOTButtonPressedEventArgs e = IOB_IOTButtonPressedEventArgs();
          buttonPressedEventHandler.fire(e);
          if (!e.Handled() && req.relayAutoOff)
          {
               digitalWrite(req.relayPin, req.relayNcOrNo ? LOW : HIGH);
               delay(req.relayAutoOffAfter);
               digitalWrite(req.relayPin, req.relayNcOrNo ? HIGH : LOW);
          }
          Serial.println("button pressed");
     }
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
     if (WiFi.isConnected() && CanSendMqtt(this))
     {
          sendSuccess = IOB_IOTMQTT::Sendata(this, state);
          return;
     }
#endif /* USE MQTT */

#ifdef USE_HTTP
     if (!sendSuccess && getDomotic().ip.isSet() && getRequired().idxDevice > 0)
     {
          sendSuccess = IOB_IOTHTTP::Sendata(this, state, espClient);
     }
#endif /* USE HTTP */
#endif
#endif /* USE_WIFI */
}

/* FONCTIONS EN COMPILATION CONDITIONNEE */
#ifdef USE_WIFI

void IOB_IOT::OnConnected(const WiFiEventStationModeConnected &event)
{
     IOB_IOTWifiStateChangedEventArgs e = IOB_IOTWifiStateChangedEventArgs(ConState::CONNECTED, "Connecté Adresse IP : " + WiFi.localIP().toString());
     IOB_IOT::GetInstance()->wifiStateChangedEventHandler.fire(e);
}

void IOB_IOT::OnDisconnected(const WiFiEventStationModeDisconnected &event)
{
     IOB_IOTWifiStateChangedEventArgs e = IOB_IOTWifiStateChangedEventArgs(ConState::DISCONNECETD, "Déconnecté !!! :" + String(event.reason));
     IOB_IOT::GetInstance()->wifiStateChangedEventHandler.fire(e);
}

void IOB_IOT::OnGotIP(const WiFiEventStationModeGotIP &event)
{
     std::vector<String> messages = {
         String("Adresse IP : " + WiFi.localIP().toString()),
         String("Passerelle IP : " + WiFi.gatewayIP().toString()),
         String("DNS IP : " + WiFi.dnsIP().toString()),
         String("Puissance de réception : " + String(WiFi.RSSI()))};

     IOB_IOTWifiStateChangedEventArgs e = IOB_IOTWifiStateChangedEventArgs(ConState::CONNECTED, messages);
     IOB_IOT::GetInstance()->wifiStateChangedEventHandler.fire(e);
}

#endif /* USE_WIFI */
