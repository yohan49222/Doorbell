#ifndef SHAREDFUNCTION_H
#define SHAREDFUNCTION_H

#include <ArduinoJson.h>
#include "IOB_IOT/EVENTS/IOB_IOTEvent.h"
class IOB_IOT;

class SharedFunction
{
public:
     bool CreateJsonMessageForDomoticz(IOB_IOT *iob, RelayState state, String &out);
};
#endif