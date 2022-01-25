#include "IOB_IOT/SHARE/IotConfig.h"

IotConfig::IotConfig()
{
#ifdef RELAY_AUTO_OFF
     required.relayAutoOff = DefinedInt(RELAY_AUTO_OFF) ? RELAY_AUTO_OFF : 1;
#endif

#ifdef RELAY_AUTO_OFF_AFTER
     required.relayAutoOffAfter = DefinedInt(RELAY_AUTO_OFF_AFTER) ? RELAY_AUTO_OFF_AFTER: 200;
#endif

#ifdef RELAY_PIN
     required.relayPin = DefinedInt(RELAY_PIN) ? RELAY_PIN : 0;
#endif

#ifdef BUTTON_PIN
     required.buttonPin = DefinedInt(BUTTON_PIN) ? BUTTON_PIN : 2;
#endif

#ifdef NCORNO
     required.relayNcOrNo = DefinedInt(NCORNO) && NCORNO == 1 ? true : false;
#endif

#ifdef DEBOUNCE_TIME
     required.debounceTime = DefinedInt(DEBOUNCE_TIME) ? DEBOUNCE_TIME : 100;
#endif

#ifdef BUTTON_PRESS_COUNT_MAX
     required.buttonPresseCountMax = DefinedInt(BUTTON_PRESS_COUNT_MAX) ? BUTTON_PRESS_COUNT_MAX : 5;
#endif

#ifdef NOMMODULE
     required.nomModule = DefinedString((String)NOMMODULE) && !EqualString((String)NOMMODULE, "default") ? (String)NOMMODULE : GenerateRamdomModuleNane();
#endif

#ifdef IDXDEVICE
     required.idxDevice = IDXDEVICE;
#endif

}

Required IotConfig::getRequired() const
{
     return required;
}

