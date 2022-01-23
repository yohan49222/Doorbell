#include "IOB_IOT/IOB_IOTWIFI.h"
#include "IOB_IOT.h"

bool IOB_IOTWIFI::Loop(IOB_IOT *iob)
{
     WifiConfig w = iob->getConfigWifi();
     if (WiFi.isConnected())
          return true;

     else if (w.ssid != emptyString && w.password != emptyString)
     {
          if (WiFi.waitForConnectResult() != WL_CONNECTED)
               return false;
     }
     return false;
}

void IOB_IOTWIFI::Begin(IOB_IOT *iob)
{
     Required req = iob->getRequired();
     WifiConfig w = iob->getConfigWifi();
     if (w.ssid != emptyString && w.password != emptyString)
     {
          WiFi.mode(WIFI_STA);

#ifdef USE_IPFIXE
          IpConfig ips = iob->getConfigIp();
          if (ips.ip.isSet() && ips.gateWay.isSet() && ips.subnet.isSet() && ips.dns.isSet())
               WiFi.config(ips.ip, ips.gateWay, ips.subnet, ips.dns);
#endif /* USE_IPFIXE */

          WiFi.setHostname(req.nomModule.c_str());
          WiFi.begin(w.ssid, w.password);

          onConnectedHandler = WiFi.onStationModeConnected(IOB_IOTWIFI::OnConnected);
          onGotIPHandler = WiFi.onStationModeGotIP(IOB_IOTWIFI::OnGotIP);
          onDisConnectedHandler = WiFi.onStationModeDisconnected(IOB_IOTWIFI::OnDisconnected);
     }
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