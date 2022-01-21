#ifndef IOTHELPER_H
#define IOTHELPER_H

#include "Arduino.h"
#include "IPAddress.h"

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