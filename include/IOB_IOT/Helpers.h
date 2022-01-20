#ifndef IOTHELPER_H
#define IOTHELPER_H

#ifndef Arduino_h
#include "Arduino.h"
#include "IPAddress.h"
#endif

class Helper
{
public:
     bool EqualString(String stest, String stestto);
     bool DefinedString(String stest);
     bool DefinedInt(const int itest);
     IPAddress ParsedIpFromString(String sip);
     String GenerateRamdomModuleNane();
};
#endif