
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

IOB_IOT::IOB_IOT()
{
#ifdef RELAY_PIN
    sConfigIot.relayPin = DefinedInt(RELAY_PIN) ? RELAY_PIN : 0;
    pinMode(sConfigIot.relayPin, OUTPUT);
#endif

#ifdef BUTTON_PIN
    sConfigIot.buttonPin = DefinedInt(BUTTON_PIN) ? BUTTON_PIN : 2;
    pinMode(sConfigIot.buttonPin, INPUT_PULLUP);
#endif

#ifdef NCORNO
    sConfigIot.relayNcOrNo = NCORNO;
#endif

#ifdef DEBOUNCE_TIME
    sConfigIot.debounceTime = DefinedInt(DEBOUNCE_TIME) ? DEBOUNCE_TIME : 100;
#endif

#ifdef BUTTON_PRESS_COUNT_MAX
    sConfigIot.buttonPresseCountMax = DefinedInt(BUTTON_PRESS_COUNT_MAX) ? BUTTON_PRESS_COUNT_MAX : 5;
#endif

#ifdef USE_WIFI

#ifdef NOMMODULE
    sConfigIot.nomModule = DefinedString((String)NOMMODULE) && !EqualString((String)NOMMODULE, "default") ? (String)NOMMODULE : GenerateRamdomModuleNane();
#endif

#ifdef IDXDEVICE
    sConfigIot.idxDevice = IDXDEVICE;
#endif

#ifdef MYSSID
    sConfigWifi.ssid = DefinedString((String)MYSSID) && !EqualString((String)MYSSID, "my_ssid") ? (String)MYSSID : emptyString;
#endif

#ifdef MYSSID_PASSWORD
    sConfigWifi.password = DefinedString((String)MYSSID_PASSWORD) && !EqualString((String)MYSSID_PASSWORD, "my_ssid_password") ? (String)MYSSID_PASSWORD : emptyString;
#endif
    

#ifdef USE_IPFIXE
#ifdef IPFIXE
    sConfigIp.ip = IPAddress::isValid((String)IPFIXE) ? ParsedIpFromString((String)IPFIXE) : INADDR_NONE;
#endif

#ifdef GATEWAY
    sConfigIp.gateWay = IPAddress::isValid((String)GATEWAY) ? ParsedIpFromString((String)GATEWAY) : INADDR_NONE;
#endif

#ifdef SUBNET
    sConfigIp.subnet = IPAddress::isValid((String)SUBNET) ? ParsedIpFromString((String)SUBNET) : INADDR_NONE;
#endif

#ifdef DNS
    sConfigIp.dns = IPAddress::isValid((String)DNS) ? ParsedIpFromString((String)DNS) : INADDR_NONE;
#endif

#endif

#ifdef USE_OTA
#ifdef OTANAME
    sConfigOta.name = DefinedString((String)OTANAME) && !EqualString((String)OTANAME, "default") ? (String)OTANAME : sConfigIot.nomModule;
#endif

#ifdef OTAPASSWORD
    sConfigOta.password = DefinedString((String)OTAPASSWORD) && !EqualString((String)OTAPASSWORD, "password") ? (String)OTAPASSWORD : emptyString;
#endif
#endif

#ifdef USE_HTTP

#ifdef DOMOTIC_SERVER
    sConfigDomo.ip = IPAddress::isValid((String)DOMOTIC_SERVER) ? this->ParsedIpFromString((String)DOMOTIC_SERVER) : INADDR_NONE;
#else
    DomoticzServerIp = INADDR_NONE;
#endif

#ifdef DOMOTIC_PORT
    sConfigDomo.port = DefinedInt(DOMOTIC_PORT) ? DOMOTIC_PORT : 8080;
#else
    domoticzServerPort = 8080;
#endif
#endif

#ifdef USE_MQTT
#ifdef MQTT_SERVER
    sConfigMqtt.ip = IPAddress::isValid((String)MQTT_SERVER) ? this->ParsedIpFromString((String)MQTT_SERVER) : INADDR_NONE;
#endif

#ifdef MQTT_PORT
    sConfigMqtt.port = DefinedInt(MQTT_PORT) ? MQTT_PORT : 1883;
#endif

#ifdef TOPICIN
    sConfigMqtt.topicIn = DefinedString((String)TOPICIN) ? (String)TOPICIN : "domoticz/out";
#endif

#ifdef TOPICOUT
    sConfigMqtt.topicOut = DefinedString((String)TOPICOUT) ? ((String)TOPICOUT) : "domoticz/in";
#endif

#ifdef MQTT_LOGIN
    sConfigMqtt.login = DefinedString((String)MQTT_LOGIN) && !EqualString((String)MQTT_LOGIN, "login") ? (String)MQTT_LOGIN : emptyString;

#endif

#ifdef MQTT_PASSWORD
    sConfigMqtt.password = DefinedString((String)MQTT_PASSWORD) && !EqualString((String)MQTT_PASSWORD, "password") ? (String)MQTT_PASSWORD : emptyString;
#endif
#endif

#ifdef USE_WEBSERVER
#ifdef WEBSERVER_PORT
    webServerPort = DefinedInt(WEBSERVER_PORT) ? WEBSERVER_PORT : 80;
#else
    webServerPort = 80;
#endif
#endif

#endif
}

void IRAM_ATTR IOB_IOT::ButtonPressed()
{
    IOB_IOT *inst = IOB_IOT::GetInstance();
    if (millis() - inst->sConfigIot.debounceTime >= inst->debounceTimer)
    {
        inst->debounceTimer = millis();
        inst->buttonPresseCount += 1;
        Serial.printf("Button has been pressed %u times\n", inst->buttonPresseCount);
    }
}

/* HELPPERS */
bool IOB_IOT::EqualString(String stest, String stestto)
{
    return stest.compareTo(stestto) == 0;
}

bool IOB_IOT::DefinedString(String stest)
{
    return stest.compareTo("") > 0;
}

bool IOB_IOT::DefinedInt(const int itest)
{
    return itest > 0;
}

IPAddress IOB_IOT::ParsedIpFromString(String sip)
{
    IPAddress ip;
    ip.fromString(sip);
    return ip;
}

String IOB_IOT::GenerateRamdomModuleNane()
{
    String _s = "ESP8266Client-";
    randomSeed(micros());
    _s += String(random(0xffff), HEX);
    return _s;
}

/* FONCTIONS PUBLIQUE */

/**
 * @brief Lancement du proces
 *
 */
void IOB_IOT::Run()
{
    digitalWrite(sConfigIot.relayPin, sConfigIot.relayNcOrNo ? HIGH : LOW);

    attachInterrupt(sConfigIot.buttonPin, IOB_IOT::ButtonPressed, FALLING);

#ifdef USE_WIFI

    if ( CANUSEWIFI )
    {
        WiFi.mode(WIFI_STA);

#ifdef USE_IPFIXE
        if ( CANUSEIPFIXE )
            WiFi.config(sConfigIp.ip, sConfigIp.gateWay, sConfigIp.subnet, sConfigIp.dns);
#endif /* USE_IPFIXE */

        WiFi.setHostname(sConfigIot.nomModule.c_str());
        WiFi.begin(sConfigWifi.ssid, sConfigWifi.password);

        onConnectedHandler = WiFi.onStationModeConnected(IOB_IOT::OnConnected);
        onGotIPHandler = WiFi.onStationModeGotIP(IOB_IOT::OnGotIP);
        onDisConnectedHandler = WiFi.onStationModeDisconnected(IOB_IOT::OnDisconnected);
    }

#ifdef USE_OTA

    ArduinoOTA.setHostname(sConfigOta.name.c_str());
    if (CANUSEOTASECURE)
        ArduinoOTA.setPassword(sConfigOta.password.c_str());

    ArduinoOTA.onStart([]()
                       { Serial.println("Start"); });

    ArduinoOTA.onEnd([]()
                     { Serial.println("\nEnd"); });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });

    ArduinoOTA.onError([](ota_error_t error)
                       {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
    ArduinoOTA.begin();
#endif

#ifdef USE_WEBSERVER
    webServer.on("/switchOn", IOB_IOT::SwitchOn);
    webServer.on("/switchOff", IOB_IOT::SwitchOff);
    webServer.on("/DebugServeur.json", IOB_IOT::DebugServeur);
    webServer.begin();
#endif

#ifdef USE_MQTT
    if (CANUSEMQTT)
    {
        MQTT_Client.setBufferSize(512);
        MQTT_Client.setServer(sConfigMqtt.ip, sConfigMqtt.port);
        MQTT_Client.setClient(espClient);
        MQTT_Client.setCallback(IOB_IOT::CallbackMQTT);
    }
#endif

#endif /* USE_WIFI */
}

/**
 * @brief Boucle
 *
 */
void IOB_IOT::Loop()
{

#ifdef USE_WIFI
    // Si l'objet est connecté au réseau, on effectue les tâches qui doivent l'être dans ce cas
    if (WiFi.isConnected())
    {
#ifdef USE_MQTT
        unsigned long currentMillis = millis();
        if (CANUSEMQTT)
        {
            if (!MQTT_Client.connected() && ((currentMillis - sConfigMqtt.previousMillis) >= sConfigMqtt.intervalConnect))
            {
                ReconnectMQTT();
                sConfigMqtt.previousMillis = currentMillis;
            }
            else if (MQTT_Client.connected())
            {
                MQTT_Client.loop();
                if ((currentMillis - sConfigMqtt.previousMillis) >= sConfigMqtt.intervalConnect)
                {
                    sConfigMqtt.previousMillis = currentMillis;
                }
            }
        }
#endif
    }
    else if ( CANUSEWIFI )
    {
        if (WiFi.waitForConnectResult() != WL_CONNECTED)
            return;
#ifdef USE_OTA
        ArduinoOTA.begin();
#endif /* USE_OTA */
    }

#ifdef USE_WEBSERVER
    webServer.handleClient();
#endif /* USE_WEBSERVER */

#ifdef USE_OTA
    ArduinoOTA.handle();
#endif /* USE_OTA */

#endif /* USE_WIFI */

    if (buttonPresseCount >= sConfigIot.buttonPresseCountMax)
    {
        buttonPresseCount = 0;
        IOB_IOTButtonPressedEventArgs e = IOB_IOTButtonPressedEventArgs();
        buttonPressedEventHandler.fire(e);
        if (e.Handled())
        {
            digitalWrite(sConfigIot.relayPin, sConfigIot.relayNcOrNo ? LOW : HIGH);
            delay(200);
            digitalWrite(sConfigIot.relayPin, sConfigIot.relayNcOrNo ? HIGH : LOW);
        }
        Serial.println("button pressed");
    }
}

/**
 * @brief Evenement de changement d'etat reçu de la domotique
 * MQTT OU HTTP OU WEBPAGE
 *
 * @param handler void ChangeState(IOB_IOTEventArgs e) {}
 */
void IOB_IOT::OnMessageRecep(std::function<void(IOB_IOTMessageRecevedEventArgs &)> handler)
{
#ifdef USE_WIFI
    messageRecepEventHandler.setHandler(handler);
#endif /* USE_WIFI */
}

void IOB_IOT::OnMessageSend(std::function<void(IOB_IOTMessageSendedEventArgs &)> handler)
{
#ifdef USE_WIFI
    messageSendedEventHandler.setHandler(handler);
#endif /* USE_WIFI */
}

void IOB_IOT::OnMqttStateChanged(std::function<void(IOB_IOTMqttStateChangedEventArgs &)> handler)
{
#ifdef USE_WIFI
    mqttStateChangedEventHandler.setHandler(handler);
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

/**
 * @brief Envois le nouvel etat vers la domotique
 * en utilisant le protocole MQTT si disponible Ou le HTTP si connecté au Wifi
 *
 * @param state <int> 1 = On (autre = Off)
 */
void IOB_IOT::SendData(String state)
{
    SendData(state ==  "On" ? RelayState::ON : RelayState::OFF);
}

/**
 * @brief Envois le nouvel etat vers la domotique
 * en utilisant le protocole MQTT si disponible Ou le HTTP si connecté au Wifi
 *
 * @param state <String> On | Off
 */
void IOB_IOT::SendData(RelayState state)
{
#ifdef USE_WIFI
    bool sendSuccess = false;

#ifdef USE_MQTT
    if (CANSENDVIAMQTT)
    {
        String messJson;
        if (CreateJsonMessageForDomoticz(state, messJson))
        {
            IOB_IOTMessageSendedEventArgs e = IOB_IOTMessageSendedEventArgs(sConfigIot.idxDevice, state, SendProtole::MQTT, messJson);
            sendSuccess = SendData(messJson, SendDataMethod::SENDBY_MQTT_ONLY, e);
            messageSendedEventHandler.fire(e);
        }
        return;
    }
#endif /* USE MQTT */

#ifdef USE_HTTP
    if (CANSENDVIAHTTP && !sendSuccess)
    {

        String url;
        if (CreateHttpMessageForDomoticz(state, url))
        {
            IOB_IOTMessageSendedEventArgs e = IOB_IOTMessageSendedEventArgs(sConfigIot.idxDevice, state, SendProtole::HTTP, url);
            sendSuccess = SendData(url, SendDataMethod::SENDBY_HTTP_ONLY, e);
            messageSendedEventHandler.fire(e);
        }
    }
#endif /* USE HTTP */
    if (!sendSuccess)
    {
    }
#endif /* USE_WIFI */
}

/* FONCTIONS EN COMPILATION CONDITIONNEE */
#ifdef USE_WIFI

void IOB_IOT::OnConnected(const WiFiEventStationModeConnected &event)
{
    IOB_IOTWifiStateChangedEventArgs e = IOB_IOTWifiStateChangedEventArgs(1, "Connecté Adresse IP : " + WiFi.localIP().toString());
    IOB_IOT::GetInstance()->wifiStateChangedEventHandler.fire(e);
}

void IOB_IOT::OnDisconnected(const WiFiEventStationModeDisconnected &event)
{
    IOB_IOTWifiStateChangedEventArgs e = IOB_IOTWifiStateChangedEventArgs(0, "Déconnecté !!! :" + String(event.reason));
    IOB_IOT::GetInstance()->wifiStateChangedEventHandler.fire(e);
}

void IOB_IOT::OnGotIP(const WiFiEventStationModeGotIP &event)
{
    std::vector<String> messages = {
        String("Adresse IP : " + WiFi.localIP().toString()),
        String("Passerelle IP : " + WiFi.gatewayIP().toString()),
        String("DNS IP : " + WiFi.dnsIP().toString()),
        String("Puissance de réception : " + String(WiFi.RSSI()))};

    IOB_IOTWifiStateChangedEventArgs e = IOB_IOTWifiStateChangedEventArgs(1, messages);
    IOB_IOT::GetInstance()->wifiStateChangedEventHandler.fire(e);
}

bool IOB_IOT::SendData(String send, int sendVia, IOB_IOTMessageSendedEventArgs &e)
{

    bool sendSuccess = false;

    if (sendVia == SendDataMethod::SENDBY_HTTP_ONLY)
    {

#ifdef USE_HTTP

        httpClient.begin(espClient, send.c_str());
        int httpCode = 0;
        httpCode = httpClient.GET();
        if (httpCode > 0)
        {
            String payload = httpClient.getString();
            if (httpCode == 200)
            {
                sendSuccess = true;
            }
        }
        httpClient.end();

        e.AddMessage("Message envoyé à Domoticz en HTTP");

#endif /* USE_HTTP */
    }
    else if (sendVia == SendDataMethod::SENDBY_MQTT_ONLY)
    {

#ifdef USE_MQTT

        sendSuccess = MQTT_Client.publish(sConfigMqtt.topicOut.c_str(), send.c_str());
        e.AddMessage("Message envoyé à Domoticz en MQTT");

#endif /* USE_MQTT */
    }
    return sendSuccess;
}

bool IOB_IOT::CreateJsonMessageForDomoticz(RelayState state, String &out)
{
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    // assigantion des variables.
    root["command"] = "switchlight";
    root["idx"] = IDXDEVICE;
    root["switchcmd"] = RelayStateConverter::toString(state);

    String messageOut;

    // formatage de la chaine json
    int printed = root.printTo(messageOut);
    if (printed > 0)
    {
        /*
         Convertion du message en Char
         */
        char messageChar[messageOut.length() + 1];
        messageOut.toCharArray(messageChar, messageOut.length() + 1);

        // assignation de out
        out = String(messageChar);
        return true;
    }
    return false;
}

#ifdef USE_WEBSERVER

bool IOB_IOT::CreateJsonMessageForDebug(String &out)
{
    StaticJsonBuffer<1024> jsonBuffer;
    StaticJsonBuffer<1024> jsonBuffer2;
    JsonObject &root = jsonBuffer.createObject();

    // assigantion des variables

#ifdef USE_IPFIXE
    JsonObject &root2 = jsonBuffer2.createObject();
    root2["ipFixe"] = sConfigIp.ip.toString();
    root2["ipGateWay"] = sConfigIp.gateWay.toString();
    root2["ipSubnet"] = sConfigIp.subnet.toString();
    root2["ipDns"] = sConfigIp.dns.toString();
    root["ip"] = root2;
#endif

#ifdef USE_OTA
    JsonObject &root3 = jsonBuffer2.createObject();
    root3["OtaName"] = sConfigOta.name;
    root3["OtaPassword"] = sConfigOta.password;
    root["ota"] = root3;
#endif

    root["Ssid"] = sConfigWifi.ssid;
    root["SsidPassword"] = sConfigWifi.password;

#ifdef USE_HTTP
    root["DomoticzServer"] = sConfigDomo.ip.toString();
    root["DomoticzPort"] = String(sConfigDomo.port);
#endif

#ifdef USE_MQTT
    root["TopicIn"] = sConfigMqtt.topicIn;
    root["topicOut"] = sConfigMqtt.topicOut;
    root["MqttServer"] = sConfigMqtt.ip.toString();
    root["MqttPort"] = String(sConfigMqtt.port);
    root["MqttLogin"] = sConfigMqtt.login;
    root["MqttPassword"] = sConfigMqtt.password;
    root["NomModule"] = sConfigIot.nomModule;
    root["IdxDevice"] = String(sConfigIot.idxDevice);
#endif

    String messageOut;

    // formatage de la chaine json
    int printed = root.printTo(messageOut);
    if (printed > 0)
    {
        /*
         Convertion du message en Char
         */
        char messageChar[messageOut.length() + 1];
        messageOut.toCharArray(messageChar, messageOut.length() + 1);

        // assignation de out
        out = String(messageChar);
        return true;
    }
    return false;
}

void IOB_IOT::TraitRequestWeb(RelayState state)
{
    IPAddress clientIp = IOB_IOT::GetInstance()->webServer.client().remoteIP();
    IOB_IOTMessageRecevedEventArgs e = IOB_IOTMessageRecevedEventArgs(IOB_IOT::GetInstance()->sConfigIot.idxDevice, state, SendProtole::WEBSERVER);
    e.AddMessage("Reception commande " + e.StateString() + " de " + clientIp.toString());
    if (
#ifdef USE_HTTP
        clientIp != IOB_IOT::GetInstance()->sConfigDomo.ip
#else
        true
#endif
        &&
#ifdef USE_MQTT
        clientIp != IOB_IOT::GetInstance()->sConfigMqtt.ip
#else
        true
#endif

    )
    {
        IOB_IOT::GetInstance()->SendData(e.StateString());
        e.AddMessage("Envois commande " + e.StateString() + " a la domotique");
    }
    else
    {
        e.AddMessage("Pas d'envoi de commande a la domotique");
    }

    String messJson;
    if (IOB_IOT::GetInstance()->CreateJsonMessageForDomoticz(e.State(), messJson))
        IOB_IOT::GetInstance()->webServer.send(200, "text/json", messJson);

    e.AddMessage("Envois de la reponse HTTP " + messJson);
    IOB_IOT::GetInstance()->messageRecepEventHandler.fire(e);
}

void IOB_IOT::SwitchOn()
{
    TraitRequestWeb(RelayState::ON);
}

void IOB_IOT::SwitchOff()
{
    TraitRequestWeb(RelayState::OFF);
}

void IOB_IOT::DebugServeur()
{
    String messJson;
    if (IOB_IOT::GetInstance()->CreateJsonMessageForDebug(messJson))
        IOB_IOT::GetInstance()->webServer.send(200, "application/json", messJson);
}
#endif /* USE_WEBSERVER */

#ifdef USE_HTTP

bool IOB_IOT::CreateHttpMessageForDomoticz(RelayState state, String &out)
{
    String url = "http://";
    url += sConfigDomo.ip.toString();
    url += ":";
    url += sConfigDomo.port;
    url += "/json.htm?type=command&param=switchlight&idx=";
    url += sConfigIot.idxDevice;
    url += "&switchcmd=";
    url += RelayStateConverter::toString(state).c_str();
    out = url;
    return true;
}

#endif /* USE_HTTP */

#ifdef USE_MQTT

void IOB_IOT::ReconnectMQTT()
{
    IOB_IOTMqttStateChangedEventArgs e = IOB_IOTMqttStateChangedEventArgs();
    if (CANUSEMQTTSECURE && MQTT_Client.connect(sConfigIot.nomModule.c_str(), sConfigMqtt.login.c_str(), sConfigMqtt.password.c_str()))
    {
        e.State(1);
        e.AddMessage("connecté avec login/password");
        MQTT_Client.subscribe(sConfigMqtt.topicIn.c_str());
        e.AddMessage("Ready , wait in/out message");
    }
    else if (CANUSEMQTT && MQTT_Client.connect(sConfigIot.nomModule.c_str()))
    {
        e.State(1);
        e.AddMessage("Connecté sans login/password");
        MQTT_Client.subscribe(sConfigMqtt.topicIn.c_str());
        e.AddMessage("Ready , wait in/out message");
    }
    else
    {
        e.AddMessage("Erreur, rc=" + String(MQTT_Client.state()));
        e.AddMessage("Prochaine tentative dans (ms) : " + String(sConfigMqtt.intervalConnect));
    }
    mqttStateChangedEventHandler.fire(e);
}

void IOB_IOT::CallbackMQTT(char *topic, byte *message, unsigned int length)
{
    IOB_IOT::GetInstance()->ParseMqttMessage(topic, message, length);
}

void IOB_IOT::ParseMqttMessage(char *topic, byte *message, unsigned int length)
{
    // Message reçu du Broker.
    String string;
    // On vérifie qu'il vient bien de Domoticz.
    int valeur = strcmp(topic, sConfigMqtt.topicIn.c_str());
    if (valeur == 0)
    {
        for (unsigned int i = 0; i < length; i++)
        {
            string += ((char)message[i]);
        }

        StaticJsonBuffer<512> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(string);
        if (root.success())
        {
            int idx = root["idx"];
            int nvalue = root["nvalue"];
            if (idx == (int)sConfigIot.idxDevice)
            {
                IOB_IOTMessageRecevedEventArgs e = IOB_IOTMessageRecevedEventArgs(idx, RelayStateConverter::fromInt(nvalue) , SendProtole::MQTT);
                e.AddMessage("Message arrivé [topic] : " + String(topic));
                e.AddMessage("Recep " + String((nvalue == 1) ? "On" : "Off") + " ");
                IOB_IOT::GetInstance()->messageRecepEventHandler.fire(e);
                if (!e.Handled())
                {
                    if (nvalue == 1)
                    {
                        digitalWrite(sConfigIot.relayPin, sConfigIot.relayNcOrNo ? LOW : HIGH);
                    }
                    else
                    {
                        digitalWrite(sConfigIot.relayPin, sConfigIot.relayNcOrNo ? HIGH : LOW);
                    }
                }
            }
        }
    }
}

#endif /* USE_MQTT */

#endif /* USE_WIFI */
