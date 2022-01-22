#include "IOB_IOT/SHARE/SharedFunction.h"
#include "IOB_IOT.h"

bool SharedFunction::CreateJsonMessageForDomoticz(IOB_IOT *iob, RelayState state, String &out)
{

     StaticJsonBuffer<256> jsonBuffer;
     JsonObject &root = jsonBuffer.createObject();

     // assigantion des variables.
     root["command"] = "switchlight";
     root["idx"] = iob->getRequired().idxDevice;
     root["switchcmd"] = RelayStateConverter::toString(state);

     String messageOut;

     // formatage de la chaine json
     int printed = root.printTo(messageOut);
     if (printed > 0)
     {
          // Convertion du message en Char
          char messageChar[messageOut.length() + 1];
          messageOut.toCharArray(messageChar, messageOut.length() + 1);

          // assignation de out
          out = String(messageChar);
          return true;
     }
     return false;
}