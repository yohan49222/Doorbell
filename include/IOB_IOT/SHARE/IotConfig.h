#ifndef IOTCONFIG_H
#define IOTCONFIG_H

#ifndef IOB_IOT_CONF_H
#include "IOB_IOT_Conf.h"
#endif /* IOB_IOT_CONF_H */

#include "IOB_IOT/SHARE/Helpers.h"

struct Required
{
     String nomModule = emptyString;
     uint32_t idxDevice = 0;
     uint32_t relayPin = 0;
     uint32_t buttonPin = 2;
     bool relayNcOrNo = true;
     uint32_t debounceTime = 0;
     uint32_t buttonPresseCountMax = 0;
     uint32_t relayAutoOff = 1;
     unsigned long relayAutoOffAfter = 200;
};

class IotConfig : private Helper
{
private:
     using Helper::DefinedInt;
     using Helper::DefinedString;
     using Helper::EqualString;
     using Helper::GenerateRamdomModuleNane;
     using Helper::ParsedIpFromString;
     Required required;
public:
     IotConfig();
     Required getRequired() const;
};
#endif