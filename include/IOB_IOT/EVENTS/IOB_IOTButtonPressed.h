#ifndef IOB_IOTBUTTONPRESSED_H
#define IOB_IOTBUTTONPRESSED_H

#include "IOB_IOTEventArg.h"

class IOB_IOTButtonPressedEventArgs : public IOB_IOTEventArg
{
private:
  bool _handled = false;

public:
  void Handled(bool handled)
  {
    _handled = handled;
  }
  bool Handled()
  {
    return _handled;
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