#ifndef IOB_IOTEVENT_H
#define IOB_IOTEVENT_H

#ifndef IOB_IOTEVENTARG_H
#include "IOB_IOTEventArg.h"
#endif /* IOB_IOTEVENTARG_H */

class IOB_IOTMessageSendedEventArgs : public IOB_IOTEventArg
{
private:
  int _idx;
  int _state;
  String _protocole;

public:
  IOB_IOTMessageSendedEventArgs(int idx, int state, String protocole)
  {
    _idx = idx;
    _state = state;
    _protocole = protocole;
  }
  IOB_IOTMessageSendedEventArgs(int idx, int state, String protocole, String message) 
  {
    _state = 0;
    AddMessage(message);
  }
  IOB_IOTMessageSendedEventArgs(int idx, int state, String protocole, std::vector<String> messages)
  {
    _state = 0;
    AddMessages(messages);
  }
  int Idx()
  {
    return _idx;
  }
  int State()
  {
    return _state;
  }
  void State(int state)
  {
    _state = state;
  }
  String StateString()
  {
    return _state == 1 ? "On" : "Off";
  }
  String Protocole()
  {
    return _protocole;
  }
};

class IOB_IOTMessageRecevedEventArgs : public IOB_IOTMessageSendedEventArgs
{
private:
  bool _handled = false;

public:
  IOB_IOTMessageRecevedEventArgs(int idx, int state, String protocole) : IOB_IOTMessageSendedEventArgs(idx, state, protocole)
  {
  }
  void Handled(bool handled)
  {
    _handled = handled;
  }
  bool Handled()
  {
    return _handled;
  }
};



class IOB_IOTWifiStateChangedEventArgs : public IOB_IOTEventArg
{
private:
  int _state;

public:
  IOB_IOTWifiStateChangedEventArgs()
  {
    _state = 0;
  }
  IOB_IOTWifiStateChangedEventArgs(int state)
  {
    _state = state;
  }
  IOB_IOTWifiStateChangedEventArgs(int state, String message)
  {
    _state = state;
    AddMessage(message);
  }
  IOB_IOTWifiStateChangedEventArgs(String message)
  {
    _state = 0;
    AddMessage(message);
  }
  IOB_IOTWifiStateChangedEventArgs(std::vector<String> messages)
  {
    _state = 0;
    AddMessages(messages);
  }
  IOB_IOTWifiStateChangedEventArgs(int state, std::vector<String> messages)
  {
    _state = state;
    AddMessages(messages);
  }
  int State()
  {
    return _state;
  }
};

class IOB_IOTMqttStateChangedEventArgs : public IOB_IOTEventArg
{
private:
  int _state;

public:
  IOB_IOTMqttStateChangedEventArgs()
  {
    _state = 0;
  }
  IOB_IOTMqttStateChangedEventArgs(int state)
  {
    _state = state;
  }
  IOB_IOTMqttStateChangedEventArgs(int state, String message)
  {
    _state = state;
    AddMessage(message);
  }
  int State()
  {
    return _state;
  }
  void State(int state)
  {
    _state = state;
  }
};




enum SendDataMethod
{
    SENDBY_HTTP_ONLY = 1,
    SENDBY_MQTT_ONLY = 2
};

typedef SendDataMethod SendData_t;

typedef IOB_IOTEventHandler<IOB_IOTWifiStateChangedEventArgs> IOB_IOTWifiStateEventHandler;
typedef IOB_IOTEventHandler<IOB_IOTMqttStateChangedEventArgs> IOB_IOTMqttStateEventHandler;
typedef IOB_IOTEventHandler<IOB_IOTMessageSendedEventArgs> IOB_IOTMessageSendedventHandler;
typedef IOB_IOTEventHandler<IOB_IOTMessageRecevedEventArgs> IOB_IOTMessageRecevedEventHandler;


#endif /* IOB_IOTEVENT_H */