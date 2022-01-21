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
     // IotConfig *conf = IotConfig::GetInstance();
     // if (millis() - conf->getRequired().debounceTime >= inst->debounceTimer)
     if (millis() - inst->getRequired().debounceTime >= inst->debounceTimer)
     {
          inst->debounceTimer = millis();
          inst->buttonPresseCount += 1;
          Serial.printf("Button has been pressed %u times\n", inst->buttonPresseCount);
     }
}

/* FONCTIONS PUBLIQUE */

/**
 * @brief Lancement du proces
 *
 */
void IOB_IOT::Run()
{
     IOB_IOT *inst = IOB_IOT::GetInstance();
     Required req = inst->getRequired();
     WifiConfig w = inst->getConfigWifi();
     pinMode(req.relayPin, OUTPUT);
     pinMode(req.buttonPin, INPUT_PULLUP);
     attachInterrupt(req.buttonPin, ButtonPressed, FALLING);
     digitalWrite(req.relayPin, req.relayNcOrNo ? HIGH : LOW);

#ifdef USE_WIFI
#ifdef USE_OTA
     IOB_IOTOTA::init();
#endif

         if (w.ssid != emptyString && w.password != emptyString)
     {
          WiFi.mode(WIFI_STA);

#ifdef USE_IPFIXE
          IpConfig ips = inst->getConfigIp();
          if (ips.ip.isSet() && ips.gateWay.isSet() && ips.subnet.isSet() && ips.dns.isSet())
               WiFi.config(ips.ip, ips.gateWay, ips.subnet, ips.dns);
#endif /* USE_IPFIXE */

          WiFi.setHostname(req.nomModule.c_str());
          WifiConfig w = inst->getConfigWifi();
          WiFi.begin(w.ssid, w.password);

          onConnectedHandler = WiFi.onStationModeConnected(IOB_IOT::OnConnected);
          onGotIPHandler = WiFi.onStationModeGotIP(IOB_IOT::OnGotIP);
          onDisConnectedHandler = WiFi.onStationModeDisconnected(IOB_IOT::OnDisconnected);
     }

#ifdef USE_WEBSERVER
     IOB_IOTWEBSERVER::init();
     // webServer.on("/switchOn", IOB_IOT::SwitchOn);
     // webServer.on("/switchOff", IOB_IOT::SwitchOff);
     // webServer.on("/DebugServeur.json", IOB_IOT::DebugServeur);
     // webServer.begin();
#endif

#ifdef USE_MQTT
     if (CanUseMqtt())
     {
          IOB_IOTMQTT::init();
     }
#endif

#endif /* USE_WIFI */
}

void IOB_IOT::Loop()
{
     IOB_IOT *inst = IOB_IOT::GetInstance();
#ifdef USE_WIFI

     WifiConfig w = inst->getConfigWifi();
     // Si l'objet est connecté au réseau, on effectue les tâches qui doivent l'être dans ce cas
     if (WiFi.isConnected())
     {
#ifdef USE_MQTT
          LoopMqtt();
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
     IOB_IOTWEBSERVER::Loop();
     // webServer.handleClient();
#endif /* USE_WEBSERVER */

#ifdef USE_OTA
     ArduinoOTA.handle();
#endif /* USE_OTA */

#endif /* USE_WIFI */

     Required req = inst->getRequired();
     if (buttonPresseCount >= req.buttonPresseCountMax)
     {
          buttonPresseCount = 0;
          IOB_IOTButtonPressedEventArgs e = IOB_IOTButtonPressedEventArgs();
          buttonPressedEventHandler.fire(e);
          if (e.Handled())
          {
               digitalWrite(req.relayPin, req.relayNcOrNo ? LOW : HIGH);
               delay(200);
               digitalWrite(req.relayPin, req.relayNcOrNo ? HIGH : LOW);
          }
          Serial.println("button pressed");
     }
}

void IOB_IOT::OnMqttRecep(std::function<void(IOB_IOTMessageRecevedEventArgs &)> handler)
{
#ifdef USE_WIFI
#ifdef USE_MQTT
     mqtt_Recep_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnMqttSend(std::function<void(IOB_IOTMessageSendedEventArgs &)> handler)
{
#ifdef USE_WIFI
#ifdef USE_MQTT
     mqtt_Send_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnWebSend(std::function<void(IOB_IOTMessageSendedEventArgs &)> handler)
{
#ifdef USE_WIFI
#ifdef USE_WEBSERVER
     webServer_Response_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnWebRecep(std::function<void(IOB_IOTMessageRecevedEventArgs &)> handler)
{
#ifdef USE_WIFI
#ifdef USE_WEBSERVER
     webServer_Request_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnHttpSend(std::function<void(IOB_IOTMessageSendedEventArgs &)> handler)
{
#ifdef USE_WIFI
#ifdef USE_HTTP
     http_Send_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnMqttStateChanged(std::function<void(IOB_IOTMqttStateChangedEventArgs &)> handler)
{
#ifdef USE_WIFI
#ifdef USE_MQTT
     mqtt_State_Changed_EventHandler.setHandler(handler);
#endif
#endif /* USE_WIFI */
}

void IOB_IOT::OnWifiStateChanged(std::function<void(IOB_IOTWifiStateChangedEventArgs &)> handler)
{
#ifdef USE_WIFI
     wifiStateChangedEventHandler.setHandler(handler);
#endif /* USE_WIFI */
}

void IOB_IOT::OnButtonPressed(std::function<void(IOB_IOTButtonPressedEventArgs &)> handler)
{
     buttonPressedEventHandler.setHandler(handler);
}

void IOB_IOT::SendData(RelayState state)
{

#ifdef USE_WIFI
#if defined(USE_MQTT) or defined(USE_HTTP)
     IOB_IOT *inst = IOB_IOT::GetInstance();
     bool sendSuccess = false;

#ifdef USE_MQTT
     if (WiFi.isConnected() && CanSendMqtt())
     {
          sendSuccess = IOB_IOTMQTT::Sendata(state);
          return;
     }
#endif /* USE MQTT */

#ifdef USE_HTTP
     if (!sendSuccess && inst->getDomotic().ip.isSet() && inst->getRequired().idxDevice > 0)
     {
          sendSuccess = IOB_IOTHTTP::Sendata(state, espClient);
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
