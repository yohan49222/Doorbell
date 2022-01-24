#ifndef IOB_IOTBUTTONPRESSED_H
#define IOB_IOTBUTTONPRESSED_H

#include "IOB_IOTEventArg.h"

class IOB_IOTButtonPressedEventArgs : public IOB_IOTEventArg
{
public:
     IOB_IOTButtonPressedEventArgs(String message) : IOB_IOTEventArg(message)
     {
     }
     IOB_IOTButtonPressedEventArgs(std::vector<String> messages) : IOB_IOTEventArg(messages)
     {
     }
};

typedef IOB_IOTEventHandler<IOB_IOTButtonPressedEventArgs> IOB_IOTButtonPressedEventHandler;
typedef std::function<void(IOB_IOTButtonPressedEventArgs &)> BtPress;

typedef std::function<void( IOB_IOTMessageSendedEventArgs &)> MqttSend;
typedef std::function<void( IOB_IOTMessageRecevedEventArgs &)> MqttRecep;
typedef std::function<void( IOB_IOTMqttStateChangedEventArgs &)> MqttState;
typedef std::function<void( IOB_IOTMessageSendedEventArgs &)> WebSend;
typedef std::function<void( IOB_IOTMessageRecevedEventArgs &)> WebRecep;
typedef std::function<void( IOB_IOTMessageSendedEventArgs &)> HttpSend;
typedef std::function<void( IOB_IOTWifiStateChangedEventArgs &)> WifiState;

#endif /* IOB_IOTBUTTOBPRESSED_H */