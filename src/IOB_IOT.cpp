
#include "IOB_IOT.h"

// Define the static Singleton pointer
IOB_IOT *IOB_IOT::inst_ = NULL;

IOB_IOT *IOB_IOT::getInstance()
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
#ifdef USE_WIFI

#ifdef NOMMODULE
    NomModule = definedString((String)NOMMODULE) && !EqualString((String)NOMMODULE, "default") ? (String)NOMMODULE : generateRamdomModuleNane();
#else
    NomModule = generateRamdomModuleNane();
#endif

#ifdef IDXDEVICE
    IdxDevice = IDXDEVICE;
#else
    IdxDevice = 0;
#endif

#ifdef MYSSID
    Ssid = definedString((String)MYSSID) && !EqualString((String)MYSSID, "my_ssid") ? (String)MYSSID : emptyString;
#else
    Ssid = emptyString;
#endif

#ifdef MYSSID_PASSWORD
    SsidPassword = definedString((String)MYSSID_PASSWORD) && !EqualString((String)MYSSID_PASSWORD, "my_ssid_password") ? (String)MYSSID_PASSWORD : emptyString;
#else
    SsidPassword = emptyString;
#endif
    canUseWifi = Ssid != emptyString && SsidPassword != emptyString;

#ifdef USE_IPFIXE
#ifdef IPFIXE
    ipFixe = IPAddress::isValid((String)IPFIXE) ? parsedIpFromString((String)IPFIXE) : INADDR_NONE;
#else
    ipFixe = INADDR_NONE;
#endif

#ifdef GATEWAY
    ipGateWay = IPAddress::isValid((String)GATEWAY) ? parsedIpFromString((String)GATEWAY) : INADDR_NONE;
#else
    ipGateWay = INADDR_NONE;
#endif

#ifdef SUBNET
    ipSubnet = IPAddress::isValid((String)SUBNET) ? parsedIpFromString((String)SUBNET) : INADDR_NONE;
#else
    ipSubnet = INADDR_NONE;
#endif

#ifdef DNS
    ipDns = IPAddress::isValid((String)DNS) ? parsedIpFromString((String)DNS) : INADDR_NONE;
#else
    ipDns = INADDR_NONE;
#endif
    canUseIpFixe = ipFixe.isSet() && ipGateWay.isSet() && ipSubnet.isSet() && ipDns.isSet();
#endif

#ifdef USE_OTA
    canUseOta = true;
#ifdef OTANAME
    OtaName = definedString((String)OTANAME) && !EqualString((String)OTANAME, "default") ? (String)OTANAME : NomModule;
#else
    OtaName = NomModule;
#endif

#ifdef OTAPASSWORD
    OtaPassword = definedString((String)OTAPASSWORD) && !EqualString((String)OTAPASSWORD, "password") ? (String)OTAPASSWORD : emptyString;
    canUseOtaPassword = OtaPassword != emptyString;
#else
    OtaPassword = emptyString;
    canUseOtaPassword = false;
#endif
#endif

#ifdef USE_HTTP


#ifdef DOMOTIC_SERVER
    DomoticzServerIp = IPAddress::isValid((String)DOMOTIC_SERVER) ? this->parsedIpFromString((String)DOMOTIC_SERVER) : INADDR_NONE;
    canUseHttp = DomoticzServerIp.isSet() && IdxDevice > 0;
#else
    DomoticzServerIp = INADDR_NONE;
#endif

#ifdef DOMOTIC_PORT
    DomoticzServerPort = definedInt(DOMOTIC_PORT) ? DOMOTIC_PORT : 8080;
#else
    DomoticzServerPort = 8080;
#endif
#endif

#ifdef USE_MQTT
#ifdef MQTT_SERVER
    MqttServerIp = IPAddress::isValid((String)MQTT_SERVER) ? this->parsedIpFromString((String)MQTT_SERVER) : INADDR_NONE;
    canUseMqtt = MqttServerIp.isSet() && IdxDevice > 0;
#else
    MqttServerIp = INADDR_NONE;
#endif

#ifdef MQTT_PORT
    MqttPort = definedInt(MQTT_PORT) ? MQTT_PORT : 1883;
#else
    MqttPort = 1883;
#endif

#ifdef TOPICIN
    TopicIn = definedString((String)TOPICIN) ? (String)TOPICIN : "domoticz/out";
#else
    TopicIn = "domoticz/out";
#endif

#ifdef TOPICOUT
    TopicOut = definedString((String)TOPICOUT) ? ((String)TOPICOUT) : "domoticz/in";
#else
    TopicOut = "domoticz/in";
#endif


#ifdef MQTT_LOGIN
    MqttLogin = definedString((String)MQTT_LOGIN) && !EqualString((String)MQTT_LOGIN, "login") ? (String)MQTT_LOGIN : emptyString;
#else
    MqttLogin = emptyString;
#endif

#ifdef MQTT_PASSWORD
    MqttPassword = definedString((String)MQTT_PASSWORD) && !EqualString((String)MQTT_PASSWORD, "password") ? (String)MQTT_PASSWORD : emptyString;
#else
    MqttPassword = emptyString;
#endif
    canUseMqttSecure = MqttLogin != emptyString && MqttPassword != emptyString && canUseMqtt;
#endif

#ifdef USE_WEBSERVER
#ifdef WEBSERVER_PORT
    WebServerPort = definedInt(WEBSERVER_PORT) ? WEBSERVER_PORT : 80;
#else
    WebServerPort = 80;
#endif
#endif

#endif
}

void IOB_IOT::loop()
{

#ifdef USE_WIFI
    unsigned long currentMillis = millis();
    if (currentMillis
#ifdef USE_MQTT
            - previousMillisMQTT >=
        intervalConnectMQTT
#endif
    )
    {
        // Si l'objet est connecté au réseau, on effectue les tâches qui doivent l'être dans ce cas
        if (WiFi.isConnected())
        {
#ifdef USE_MQTT
            if (canUseMqtt)
            {
                if (!MQTT_Client.connected())
                {
                    Serial.println("MQTT déconnecté, on reconnecte !");
                    reconnectMQTT();
                }
                else
                {
                    Serial.print(".");
                    MQTT_Client.loop();
                }
            }
#endif // USE_MQTT
        }
        else if (canUseWifi)
        {
            Serial.println("wifi error");
            if (WiFi.waitForConnectResult() != WL_CONNECTED)
                return;
#ifdef USE_OTA
            ArduinoOTA.begin();
#endif // USE_OTA
        }
#ifdef USE_MQTT
        previousMillisMQTT = currentMillis;
#endif
    }
#endif

#ifdef USE_WEBSERVER
    webServer.handleClient();
#endif

#ifdef USE_OTA
    ArduinoOTA.handle();
#endif // USE_OTA
}

String IOB_IOT::generateRamdomModuleNane()
{
    String _s = "ESP8266Client-";
    randomSeed(micros());
    _s += String(random(0xffff), HEX);
    return _s;
}

void IOB_IOT::SetUp()
{
    if (canUseWifi)
    {
        WiFi.mode(WIFI_STA);

#ifdef USE_IPFIXE
        if (canUseIpFixe)
            WiFi.config(ipFixe, ipGateWay, ipSubnet, ipDns);
#endif

        WiFi.setHostname(NomModule.c_str());
        WiFi.begin(Ssid, SsidPassword);

        onConnectedHandler = WiFi.onStationModeConnected(IOB_IOT::onConnected);
        onGotIPHandler = WiFi.onStationModeGotIP(IOB_IOT::onGotIP);
        onDisConnectedHandler = WiFi.onStationModeDisconnected(IOB_IOT::onDisconnected);
    }

#ifdef USE_OTA
    if (canUseOta)
    {
        ArduinoOTA.setHostname(OtaName.c_str());
        if (canUseOtaPassword)
            ArduinoOTA.setPassword(OtaPassword.c_str());

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
    }
#endif

#ifdef USE_WEBSERVER
    if (canUseWebServer)
    {
        webServer.on("/switchOn", IOB_IOT::switchOn);
        webServer.on("/switchOff", IOB_IOT::switchOff);
        webServer.on("/DebugServeur", IOB_IOT::DebugServeur);
        webServer.begin();
    }
#endif

#ifdef USE_MQTT
    if (canUseMqtt)
    {
        MQTT_Client.setServer(MqttServerIp, MqttPort);
        MQTT_Client.setClient(espClient);
        MQTT_Client.setCallback(IOB_IOT::callbackMQTT);
    }
#endif
}
#ifdef USE_MQTT
void IOB_IOT::reconnectMQTT()
{
#ifdef USE_WIFI
#ifdef USE_MQTT
    if (WiFi.isConnected())
    {
        // Boucle jusqu'à la connexion MQTT
        while (!MQTT_Client.connected())
        {
            Serial.println("Tentative de connexion MQTT...");
            // Tentative de connexion
            if (canUseMqttSecure && MQTT_Client.connect(NomModule.c_str(), MqttLogin.c_str(), MqttPassword.c_str()))
            {
                Serial.println("MQTT connecté avec login/password");
                MQTT_Client.subscribe(TopicIn.c_str());
                // MQTT_Client.subscribe("#");
                Serial.println("MQTT Ready , wait in/out message");
            }
            else if (canUseMqtt && MQTT_Client.connect(NomModule.c_str()))
            {
                Serial.println("MQTT connecté sans login/password");
                MQTT_Client.subscribe(TopicIn.c_str());
                Serial.println("MQTT Ready , wait in/out message");
            }
            else
            {
                Serial.print("Erreur, rc=");
                Serial.print(MQTT_Client.state());
                Serial.println(" prochaine tentative dans 2s");
                return;
            }
            if (!WiFi.isConnected())
                return;
        }
    }
#endif // USE_MQTT
#endif
}

void IOB_IOT::callbackMQTT(char *topic, byte *message, unsigned int length)
{
    IOB_IOT::getInstance()->parseMqttMessage(topic, message, length);
}

void IOB_IOT::parseMqttMessage(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrivé [");
    Serial.print(topic);
    Serial.println("] ");

    // Message reçu du Broker.
    String string;
    // On vérifie qu'il vient bien de Domoticz.
    int valeur = strcmp(topic, TopicIn.c_str());
    if (valeur == 0)
    {
        Serial.print("Message arrivé [");
        Serial.print(topic);
        Serial.print("] ");
        for (int i = 0; i < length; i++)
        {
            string += ((char)message[i]);
        }
        // Affiche le message entrant - display incoming message
        Serial.println(string);

        // Parse l'objet JSON nommé "root"
        StaticJsonBuffer<512> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(string);
        if (root.success())
        {
            int idx = root["idx"];
            int nvalue = root["nvalue"];
            if (idx == 13)
            {
                IOB_IOTEventArgs e = IOB_IOTEventArgs(idx, nvalue);
                IOB_IOT::getInstance()->changeStateEvent.fire(e);
            }
        }
        else
        {
            Serial.println("Erreur de lecture du JSON !");
        }
    }
}

bool IOB_IOT::CanSendViaMqtt()
{
    return canUseMqtt && WiFi.isConnected() && MQTT_Client.connected();
}

#endif

void IOB_IOT::SendData(String send, int sendVia)
{
    if (sendVia == SendDataMethod::SENDBY_HTTP_ONLY)
    {
#ifdef USE_HTTP
        httpClient.begin(espClient, send.c_str());
        int httpCode = 0;
        httpCode = httpClient.GET();
        if (httpCode > 0)
        {
            String payload = httpClient.getString();
            Serial.println(payload);
        }
        httpClient.end();
        Serial.println("Message envoyé à Domoticz en HTTP");
#endif
    }
    else if (sendVia == SendDataMethod::SENDBY_MQTT_ONLY)
    {
#ifdef USE_MQTT
        MQTT_Client.publish(TopicOut.c_str(), send.c_str());
        Serial.println("Message envoyé à Domoticz en MQTT");
#endif
    }
}

#ifdef USE_HTTP
bool IOB_IOT::CanSendViaHttp()
{
    return canUseHttp && WiFi.isConnected();
}
#endif

void IOB_IOT::SendData(String state)
{
#ifdef USE_MQTT
    if (CanSendViaMqtt())
    {
        String messJson;
        if (CreateJsonMessageForDomoticz(state, messJson))
            SendData(messJson, SendDataMethod::SENDBY_MQTT_ONLY);

        return;
    }
#endif

#ifdef USE_HTTP
    if (CanSendViaHttp())
    {
        String url;
        if (CreateHttpMessageForDomoticz(state, url))
        {
            Serial.print("URL: ");
            Serial.println(url.c_str());
            SendData(url, SendDataMethod::SENDBY_HTTP_ONLY);
        }
    }
#endif
}

#ifdef USE_WEBSERVER
void IOB_IOT::switchOn()
{
    Serial.println("Reception HTTP sur l'url /SWITCHON");

    IPAddress clientIp = IOB_IOT::getInstance()->webServer.client().remoteIP();
    if (
#ifdef USE_HTTP
        clientIp != IOB_IOT::getInstance()->DomoticzServerIp
#else
        true
#endif
        &&
#ifdef USE_MQTT
        clientIp != IOB_IOT::getInstance()->MqttServerIp
#else
        true
#endif

    )
    {
        IOB_IOT::getInstance()->SendData("On");
    }

    IOB_IOTEventArgs e = IOB_IOTEventArgs(IOB_IOT::getInstance()->IdxDevice, 1);
    IOB_IOT::getInstance()->changeStateEvent.fire(e);
    String messJson;
    Serial.println("Envois de la reponse HTTP");
    if (IOB_IOT::getInstance()->CreateJsonMessageForDomoticz("On", messJson))
        IOB_IOT::getInstance()->webServer.send(200, "text/json", messJson);
}

void IOB_IOT::switchOff()
{
    Serial.println("Reception HTTP sur l'url /SWITCHON");
    IPAddress clientIp = IOB_IOT::getInstance()->webServer.client().remoteIP();
    if (
#ifdef USE_HTTP
        clientIp != IOB_IOT::getInstance()->DomoticzServerIp
#else
        true
#endif
        &&
#ifdef USE_MQTT
        clientIp != IOB_IOT::getInstance()->MqttServerIp
#else
        true
#endif

    )
    {
        IOB_IOT::getInstance()->SendData("Off");
    }

    IOB_IOTEventArgs e = IOB_IOTEventArgs(IOB_IOT::getInstance()->IdxDevice, 0);
    IOB_IOT::getInstance()->changeStateEvent.fire(e);

    String messJson;
    Serial.println("Envois de la reponse HTTP");
    if (IOB_IOT::getInstance()->CreateJsonMessageForDomoticz("Off", messJson))
        IOB_IOT::getInstance()->webServer.send(200, "text/json", messJson);
}

void IOB_IOT::DebugServeur()
{
    String messJson;
    Serial.println("Envois de la reponse HTTP");
    if (IOB_IOT::getInstance()->CreateJsonMessageForDebug(messJson))
        IOB_IOT::getInstance()->webServer.send(200, "text/json", messJson);
}
#endif

bool IOB_IOT::EqualString(String stest, String stestto)
{
    return stest.compareTo(stestto) == 0;
}

bool IOB_IOT::definedString(String stest)
{
    return stest.compareTo("") > 0;
}

bool IOB_IOT::definedInt(const int itest)
{
    return itest > 0;
}

IPAddress IOB_IOT::parsedIpFromString(String sip)
{
    IPAddress ip;
    ip.fromString(sip);
    return ip;
}

/*
bool IOB_IOT::CanUseMqtt()
{
    return canUseMqtt;
}

bool IOB_IOT::CanUseMqttSecure()
{
    return canUseMqttSecure;
}

bool IOB_IOT::CanUseHttp()
{
    return canUseHttp;
}

bool IOB_IOT::CanUseWifi()
{
    return canUseWifi;
}

String IOB_IOT::getMqttServer()
{
    return MqttServerIp.toString();
}

int IOB_IOT::getMqttPort()
{
    return MqttPort;
}
String IOB_IOT::getTopicOut()
{
    return TopicOut;
}
String IOB_IOT::getNomModule()
{
    return NomModule;
}
String IOB_IOT::getMqttLogin()
{
    return MqttLogin;
}
String IOB_IOT::getMqttPassword()
{
    return MqttPassword;
}
*/
#ifdef USE_WEBSERVER
bool IOB_IOT::CreateJsonMessageForDebug(String &out)
{
    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    // assigantion des variables

#ifdef USE_IPFIXE
    root["ipFixe"] = ipFixe.toString();
    root["ipGateWay"] = ipGateWay.toString();
    root["ipSubnet"] = ipSubnet.toString();
    root["ipDns"] = ipDns.toString();
#endif

#ifdef USE_OTA
    root["OtaName"] = OtaName;
    root["OtaPassword"] = OtaPassword;
#endif

    root["Ssid"] = Ssid;
    root["SsidPassword"] = SsidPassword;
    root["TopicIn"] = TopicIn;
    root["topicOut"] = TopicOut;
#ifdef USE_HTTP
    root["DomoticzServer"] = DomoticzServerIp.toString();
    root["DomoticzPort"] = String(DomoticzServerPort);
#endif

#ifdef USE_MQTT
    root["MqttServer"] = MqttServerIp.toString();
    root["MqttPort"] = String(MqttPort);
    root["MqttLogin"] = MqttLogin;
    root["MqttPassword"] = MqttPassword;
    root["NomModule"] = NomModule;
    root["IdxDevice"] = String(IdxDevice);
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
#endif

#ifdef USE_HTTP
bool IOB_IOT::CreateHttpMessageForDomoticz(String state, String &out)
{
    String url = "http://";
    url += DomoticzServerIp.toString();
    url += ":";
    url += DomoticzServerPort;
    url += "/json.htm?type=command&param=switchlight&idx=";
    url += IdxDevice;
    url += "&switchcmd=";
    url += state.c_str();
    out = url;
    return true;
}
#endif

#ifdef USE_MQTT
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
#endif


void IOB_IOT::onConnected(const WiFiEventStationModeConnected &event)
{
    Serial.println("WiFi connecté");
    Serial.println("Adresse IP : " + WiFi.localIP().toString());
}

void IOB_IOT::onDisconnected(const WiFiEventStationModeDisconnected &event)
{
    Serial.println("WiFi Déconnecté");
    Serial.print("Raison : ");
    Serial.println(event.reason);
}

void IOB_IOT::onGotIP(const WiFiEventStationModeGotIP &event)
{
    Serial.println("Adresse IP : " + WiFi.localIP().toString());
    Serial.println("Passerelle IP : " + WiFi.gatewayIP().toString());
    Serial.println("DNS IP : " + WiFi.dnsIP().toString());
    Serial.print("Puissance de réception : ");
    Serial.println(WiFi.RSSI());
}

void IOB_IOT::OnRecevChangeState(std::function<void(IOB_IOTEventArgs &)> handler)
{
    changeStateEvent.addListener(handler);
}