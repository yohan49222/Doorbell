
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
    relayPin = DefinedInt(RELAY_PIN) ? RELAY_PIN : 0;
    pinMode(relayPin, OUTPUT);
#else
    relayPin = 0;
    pinMode(relayPin, OUTPUT);
#endif

#ifdef BUTTON_PIN
    buttonPin = DefinedInt(BUTTON_PIN) ? BUTTON_PIN : 2;
    pinMode(buttonPin, INPUT_PULLUP);
#else
    buttonPin = 2;
    pinMode(buttonPin, INPUT_PULLUP);
#endif

#ifdef NCORNO
    relayNcOrNo = NCORNO;
#else
    relayNcOrNo = true;
#endif

#ifdef DEBOUNCE_TIME
    debounceTime = DefinedInt(DEBOUNCE_TIME) ? DEBOUNCE_TIME : 100;
#else
    debounceTime = 100;
#endif

#ifdef BUTTON_PRESS_COUNT_MAX
    buttonPresseCountMax = DefinedInt(BUTTON_PRESS_COUNT_MAX) ? BUTTON_PRESS_COUNT_MAX : 2;
#else
    buttonPresseCountMax = 5;
#endif

#ifdef USE_WIFI

#ifdef NOMMODULE
    nomModule = DefinedString((String)NOMMODULE) && !EqualString((String)NOMMODULE, "default") ? (String)NOMMODULE : GenerateRamdomModuleNane();
#else
    nomModule = generateRamdomModuleNane();
#endif

#ifdef IDXDEVICE
    idxDevice = IDXDEVICE;
#else
    idxDevice = 0;
#endif

#ifdef MYSSID
    ssid = DefinedString((String)MYSSID) && !EqualString((String)MYSSID, "my_ssid") ? (String)MYSSID : emptyString;
#else
    ssid = emptyString;
#endif

#ifdef MYSSID_PASSWORD
    ssidPassword = DefinedString((String)MYSSID_PASSWORD) && !EqualString((String)MYSSID_PASSWORD, "my_ssid_password") ? (String)MYSSID_PASSWORD : emptyString;
#else
    ssidPassword = emptyString;
#endif
    

#ifdef USE_IPFIXE
#ifdef IPFIXE
    ipFixe = IPAddress::isValid((String)IPFIXE) ? ParsedIpFromString((String)IPFIXE) : INADDR_NONE;
#else
    ipFixe = INADDR_NONE;
#endif

#ifdef GATEWAY
    ipGateWay = IPAddress::isValid((String)GATEWAY) ? ParsedIpFromString((String)GATEWAY) : INADDR_NONE;
#else
    ipGateWay = INADDR_NONE;
#endif

#ifdef SUBNET
    ipSubnet = IPAddress::isValid((String)SUBNET) ? ParsedIpFromString((String)SUBNET) : INADDR_NONE;
#else
    ipSubnet = INADDR_NONE;
#endif

#ifdef DNS
    ipDns = IPAddress::isValid((String)DNS) ? ParsedIpFromString((String)DNS) : INADDR_NONE;
#else
    ipDns = INADDR_NONE;
#endif

#endif

#ifdef USE_OTA
#ifdef OTANAME
    otaName = DefinedString((String)OTANAME) && !EqualString((String)OTANAME, "default") ? (String)OTANAME : nomModule;
#else
    otaName = nomModule;
#endif

#ifdef OTAPASSWORD
    otaPassword = DefinedString((String)OTAPASSWORD) && !EqualString((String)OTAPASSWORD, "password") ? (String)OTAPASSWORD : emptyString;
    // canUseOtaPassword = otaPassword != emptyString;
#else
    otaPassword = emptyString;
    // canUseOtaPassword = false;
#endif
#endif

#ifdef USE_HTTP

#ifdef DOMOTIC_SERVER
    domoticzServerIp = IPAddress::isValid((String)DOMOTIC_SERVER) ? this->ParsedIpFromString((String)DOMOTIC_SERVER) : INADDR_NONE;
#else
    DomoticzServerIp = INADDR_NONE;
#endif

#ifdef DOMOTIC_PORT
    domoticzServerPort = DefinedInt(DOMOTIC_PORT) ? DOMOTIC_PORT : 8080;
#else
    domoticzServerPort = 8080;
#endif
#endif

#ifdef USE_MQTT
#ifdef MQTT_SERVER
    mqttServerIp = IPAddress::isValid((String)MQTT_SERVER) ? this->ParsedIpFromString((String)MQTT_SERVER) : INADDR_NONE;
#else
    mqttServerIp = INADDR_NONE;
#endif

#ifdef MQTT_PORT
    mqttPort = DefinedInt(MQTT_PORT) ? MQTT_PORT : 1883;
#else
    mqttPort = 1883;
#endif

#ifdef TOPICIN
    topicIn = DefinedString((String)TOPICIN) ? (String)TOPICIN : "domoticz/out";
#else
    topicIn = "domoticz/out";
#endif

#ifdef TOPICOUT
    topicOut = DefinedString((String)TOPICOUT) ? ((String)TOPICOUT) : "domoticz/in";
#else
    topicOut = "domoticz/in";
#endif

#ifdef MQTT_LOGIN
    mqttLogin = DefinedString((String)MQTT_LOGIN) && !EqualString((String)MQTT_LOGIN, "login") ? (String)MQTT_LOGIN : emptyString;
#else
    mqttLogin = emptyString;
#endif

#ifdef MQTT_PASSWORD
    mqttPassword = DefinedString((String)MQTT_PASSWORD) && !EqualString((String)MQTT_PASSWORD, "password") ? (String)MQTT_PASSWORD : emptyString;
#else
    mqttPassword = emptyString;
#endif
    // canUseMqttSecure = mqttLogin != emptyString && mqttPassword != emptyString && canUseMqtt;
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
    if (millis() - inst->debounceTime >= inst->debounceTimer)
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
    digitalWrite(relayPin, relayNcOrNo ? HIGH : LOW);

    attachInterrupt(buttonPin, IOB_IOT::ButtonPressed, FALLING);

#ifdef USE_WIFI

    if ( CANUSEWIFI )
    {
        WiFi.mode(WIFI_STA);

#ifdef USE_IPFIXE
        if ( CANUSEIPFIXE )
            WiFi.config(ipFixe, ipGateWay, ipSubnet, ipDns);
#endif /* USE_IPFIXE */

        WiFi.setHostname(nomModule.c_str());
        WiFi.begin(ssid, ssidPassword);

        onConnectedHandler = WiFi.onStationModeConnected(IOB_IOT::OnConnected);
        onGotIPHandler = WiFi.onStationModeGotIP(IOB_IOT::OnGotIP);
        onDisConnectedHandler = WiFi.onStationModeDisconnected(IOB_IOT::OnDisconnected);
    }

#ifdef USE_OTA

    ArduinoOTA.setHostname(otaName.c_str());
    if (CANUSEOTASECURE)
        ArduinoOTA.setPassword(otaPassword.c_str());

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
        MQTT_Client.setServer(mqttServerIp, mqttPort);
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
            if (!MQTT_Client.connected() && ((currentMillis - previousMillisMQTT) >= intervalConnectMQTT))
            {
                ReconnectMQTT();
                previousMillisMQTT = currentMillis;
            }
            else if (MQTT_Client.connected())
            {
                MQTT_Client.loop();
                if ((currentMillis - previousMillisMQTT) >= intervalConnectMQTT)
                {
                    previousMillisMQTT = currentMillis;
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

    if (buttonPresseCount >= buttonPresseCountMax)
    {
        buttonPresseCount = 0;
        IOB_IOTButtonPressedEventArgs e = IOB_IOTButtonPressedEventArgs();
        buttonPressedEventHandler.fire(e);
        if (e.Handled())
        {
            digitalWrite(relayPin, relayNcOrNo ? LOW : HIGH);
            delay(200);
            digitalWrite(relayPin, relayNcOrNo ? HIGH : LOW);
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
void IOB_IOT::OnRecevChangeState(std::function<void(IOB_IOTMessageRecevedEventArgs &)> handler)
{
#ifdef USE_WIFI
    changeStateEventHandler.setHandler(handler);
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
 * @param state <int> 1 = On (aiutre = Off)
 */
void IOB_IOT::SendData(SendData_t state)
{
    SendData(state == 1 ? "On" : "Off");
}

/**
 * @brief Envois le nouvel etat vers la domotique
 * en utilisant le protocole MQTT si disponible Ou le HTTP si connecté au Wifi
 *
 * @param state <String> On | Off
 */
void IOB_IOT::SendData(String state)
{
#ifdef USE_WIFI
    bool sendSuccess = false;

#ifdef USE_MQTT
    if (CANSENDVIAMQTT)
    {
        String messJson;
        if (CreateJsonMessageForDomoticz(state, messJson))
        {
            IOB_IOTMessageSendedEventArgs e = IOB_IOTMessageSendedEventArgs(idxDevice, 0, "MQTT", messJson);
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
            IOB_IOTMessageSendedEventArgs e = IOB_IOTMessageSendedEventArgs(idxDevice, 0, "HTTP", url);
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

        sendSuccess = MQTT_Client.publish(topicOut.c_str(), send.c_str());
        e.AddMessage("Message envoyé à Domoticz en MQTT");

#endif /* USE_MQTT */
    }
    return sendSuccess;
}

bool IOB_IOT::CreateJsonMessageForDomoticz(String state, String &out)
{
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    // assigantion des variables.
    root["command"] = "switchlight";
    root["idx"] = IDXDEVICE;
    root["switchcmd"] = state;

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
    root2["ipFixe"] = ipFixe.toString();
    root2["ipGateWay"] = ipGateWay.toString();
    root2["ipSubnet"] = ipSubnet.toString();
    root2["ipDns"] = ipDns.toString();
    root["ip"] = root2;
#endif

#ifdef USE_OTA
    JsonObject &root3 = jsonBuffer2.createObject();
    root3["OtaName"] = otaName;
    root3["OtaPassword"] = otaPassword;
    root["ota"] = root3;
#endif

    root["Ssid"] = ssid;
    root["SsidPassword"] = ssidPassword;

#ifdef USE_HTTP
    root["DomoticzServer"] = domoticzServerIp.toString();
    root["DomoticzPort"] = String(domoticzServerPort);
#endif

#ifdef USE_MQTT
    root["TopicIn"] = topicIn;
    root["topicOut"] = topicOut;
    root["MqttServer"] = mqttServerIp.toString();
    root["MqttPort"] = String(mqttPort);
    root["MqttLogin"] = mqttLogin;
    root["MqttPassword"] = mqttPassword;
    root["NomModule"] = nomModule;
    root["IdxDevice"] = String(idxDevice);
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

void IOB_IOT::TraitRequestWeb(int state)
{
    IPAddress clientIp = IOB_IOT::GetInstance()->webServer.client().remoteIP();
    IOB_IOTMessageRecevedEventArgs e = IOB_IOTMessageRecevedEventArgs(IOB_IOT::GetInstance()->idxDevice, state, "WEBSERVER");
    e.AddMessage("Reception commande " + e.StateString() + " de " + clientIp.toString());
    if (
#ifdef USE_HTTP
        clientIp != IOB_IOT::GetInstance()->domoticzServerIp
#else
        true
#endif
        &&
#ifdef USE_MQTT
        clientIp != IOB_IOT::GetInstance()->mqttServerIp
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
    if (IOB_IOT::GetInstance()->CreateJsonMessageForDomoticz(e.StateString(), messJson))
        IOB_IOT::GetInstance()->webServer.send(200, "text/json", messJson);

    e.AddMessage("Envois de la reponse HTTP " + messJson);
    IOB_IOT::GetInstance()->changeStateEventHandler.fire(e);
}

void IOB_IOT::SwitchOn()
{
    TraitRequestWeb(1);
}

void IOB_IOT::SwitchOff()
{
    TraitRequestWeb(0);
}

void IOB_IOT::DebugServeur()
{
    String messJson;
    if (IOB_IOT::GetInstance()->CreateJsonMessageForDebug(messJson))
        IOB_IOT::GetInstance()->webServer.send(200, "application/json", messJson);
}
#endif /* USE_WEBSERVER */

#ifdef USE_HTTP

bool IOB_IOT::CreateHttpMessageForDomoticz(String state, String &out)
{
    String url = "http://";
    url += domoticzServerIp.toString();
    url += ":";
    url += domoticzServerPort;
    url += "/json.htm?type=command&param=switchlight&idx=";
    url += idxDevice;
    url += "&switchcmd=";
    url += state.c_str();
    out = url;
    return true;
}

#endif /* USE_HTTP */

#ifdef USE_MQTT

void IOB_IOT::ReconnectMQTT()
{
    IOB_IOTMqttStateChangedEventArgs e = IOB_IOTMqttStateChangedEventArgs();
    if (CANUSEMQTTSECURE && MQTT_Client.connect(nomModule.c_str(), mqttLogin.c_str(), mqttPassword.c_str()))
    {
        e.State(1);
        e.AddMessage("connecté avec login/password");
        MQTT_Client.subscribe(topicIn.c_str());
        e.AddMessage("Ready , wait in/out message");
    }
    else if (CANUSEMQTT && MQTT_Client.connect(nomModule.c_str()))
    {
        e.State(1);
        e.AddMessage("Connecté sans login/password");
        MQTT_Client.subscribe(topicIn.c_str());
        e.AddMessage("Ready , wait in/out message");
    }
    else
    {
        e.AddMessage("Erreur, rc=" + String(MQTT_Client.state()));
        e.AddMessage("Prochaine tentative dans (ms) : " + String(intervalConnectMQTT));
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
    int valeur = strcmp(topic, topicIn.c_str());
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
            if (idx == (int)idxDevice)
            {
                IOB_IOTMessageRecevedEventArgs e = IOB_IOTMessageRecevedEventArgs(idx, nvalue, "MQTT");
                e.AddMessage("Message arrivé [topic] : " + String(topic));
                e.AddMessage("Recep " + String((nvalue == 1) ? "On" : "Off") + " ");
                IOB_IOT::GetInstance()->changeStateEventHandler.fire(e);
                if (!e.Handled())
                {
                    if (nvalue == 1)
                    {
                        digitalWrite(relayPin, relayNcOrNo ? LOW : HIGH);
                    }
                    else
                    {
                        digitalWrite(relayPin, relayNcOrNo ? HIGH : LOW);
                    }
                }
            }
        }
    }
}

#endif /* USE_MQTT */

#endif /* USE_WIFI */
