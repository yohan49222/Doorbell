#include "IOB_IOT/Helpers.h"

/* HELPPERS */
bool Helper::EqualString(String stest, String stestto)
{
    return stest.compareTo(stestto) == 0;
}

bool Helper::DefinedString(String stest)
{
    return stest.compareTo("") > 0;
}

bool Helper::DefinedInt(const int itest)
{
    return itest > 0;
}

IPAddress Helper::ParsedIpFromString(String sip)
{
    IPAddress ip;
    ip.fromString(sip);
    return ip;
}

String Helper::GenerateRamdomModuleNane()
{
    String _s = "ESP8266Client-";
    randomSeed(micros());
    _s += String(random(0xffff), HEX);
    return _s;
}