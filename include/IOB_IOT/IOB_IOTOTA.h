#ifndef IOB_IOTOTA_H
#define IOB_IOTOTA_H

#include <ArduinoOTA.h>
#include "IOB_IOT/SHARE/Helpers.h"
class IOB_IOT;
class IOB_IOTOTA;

class IOB_IOTOTAConfig : Helper
{
private:
     String name = emptyString;
     String password = emptyString;
public:
     friend IOB_IOTOTA;
     IOB_IOTOTAConfig()
     {
#ifdef OTANAME
     name = DefinedString((String)OTANAME) && !EqualString((String)OTANAME, "default") ? (String)OTANAME : emptyString;
#endif

#ifdef OTAPASSWORD
     password = DefinedString((String)OTAPASSWORD) && !EqualString((String)OTAPASSWORD, "password") ? (String)OTAPASSWORD : emptyString;
#endif
     }
};

class IOB_IOTOTA : IOB_IOTOTAConfig
{
public:
     IOB_IOTOTA();
     //void init(IOB_IOT *iob);
};
#endif