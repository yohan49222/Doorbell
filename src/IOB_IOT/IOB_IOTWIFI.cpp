#include "IOB_IOT/IOB_IOTWIFI.h"
#include "IOB_IOT.h"

#ifdef USE_WIFI

IOB_IOTWIFI::IOB_IOTWIFI() : IOB_IOTWIFIConfig(), IOB_IOTIpConfig()
{
     Required req = ((IOB_IOT*)this)->getRequired();
     if (ssid == emptyString || password == emptyString)
          return;

     WiFi.mode(WIFI_STA);

#ifdef USE_IPFIXE
     if (ip.isSet() && gateWay.isSet() && subnet.isSet() && dns.isSet())
          WiFi.config(ip, gateWay, subnet, dns);

#endif /* USE_IPFIXE */

     WiFi.setHostname(req.nomModule.c_str());
     WiFi.begin(ssid, password);

     onConnectedHandler = WiFi.onStationModeConnected(IOB_IOTWIFI::OnConnected);
     onGotIPHandler = WiFi.onStationModeGotIP(IOB_IOTWIFI::OnGotIP);
     onDisConnectedHandler = WiFi.onStationModeDisconnected(IOB_IOTWIFI::OnDisconnected);
}

bool IOB_IOTWIFI::Loop()
{
     if (WiFi.isConnected())
          return true;

     else if (ssid != emptyString && password != emptyString)
     {
          if (WiFi.waitForConnectResult() != WL_CONNECTED)
               return false;
     }
     return false;
}

void IOB_IOTWIFI::OnConnected(const WiFiEventStationModeConnected &event)
{
     IOB_IOTWifiStateChangedEventArgs e = IOB_IOTWifiStateChangedEventArgs(ConState::CONNECTED, "Connecté Adresse IP : " + WiFi.localIP().toString());
     IOB_IOT::GetInstance()->wifiStateChangedEventHandler.fire(e);
}

void IOB_IOTWIFI::OnDisconnected(const WiFiEventStationModeDisconnected &event)
{
     IOB_IOTWifiStateChangedEventArgs e = IOB_IOTWifiStateChangedEventArgs(ConState::DISCONNECETD, "Déconnecté !!! :" + String(event.reason));
     IOB_IOT::GetInstance()->wifiStateChangedEventHandler.fire(e);
}

void IOB_IOTWIFI::OnGotIP(const WiFiEventStationModeGotIP &event)
{
     std::vector<String> messages = {
         String("Adresse IP : " + WiFi.localIP().toString()),
         String("Passerelle IP : " + WiFi.gatewayIP().toString()),
         String("DNS IP : " + WiFi.dnsIP().toString()),
         String("Puissance de réception : " + String(WiFi.RSSI()))};

     IOB_IOTWifiStateChangedEventArgs e = IOB_IOTWifiStateChangedEventArgs(ConState::CONNECTED, messages);
     IOB_IOT::GetInstance()->wifiStateChangedEventHandler.fire(e);

}

#endif