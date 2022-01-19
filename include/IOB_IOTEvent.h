#ifndef IOB_IOTEVENT_H
#define IOB_IOTEVENT_H

#ifndef IOB_IOTEVENTARG_H
#include "IOB_IOTEventArg.h"
#endif /* IOB_IOTEVENTARG_H */
enum SendDataMethod
{
     SENDBY_MQTT_ONLY = 0,
     SENDBY_HTTP_ONLY = 1
};
enum SendProtole
{
     HTTP = 1,
     MQTT = 2,
     WEBSERVER = 3
};

class SendProtoleConverter
{
public:
     static String toString(SendProtole s)
     {
          if (s == SendProtole::HTTP)
               return "HTTP";

          if (s == SendProtole::MQTT)
               return "MQTT";

          return "WEBSERVER";
     }

     static SendProtole fromString(String s)
     {
          s.toUpperCase();
          if (s.compareTo("HTTP") == 0)
               return SendProtole::HTTP;

          if (s.compareTo("MQTT") == 0)
               return SendProtole::MQTT;

          return SendProtole::WEBSERVER;
     }
};

enum RelayState
{
     OFF = 0,
     ON = 1
};

class RelayStateConverter
{
public:
     static String toString(RelayState s)
     {
          if (s == RelayState::ON)
               return "On";

          return "Off";
     }

     static RelayState fromString(String s)
     {
          s.toUpperCase();
          if (s.compareTo("ON") == 0)
               return RelayState::ON;

          return RelayState::OFF;
     }

     static int toInt(RelayState s)
     {
          if (s == RelayState::ON)
               return 1;

          return 0;
     }

     static RelayState fromInt(int i)
     {
          return (i == 1) ? RelayState::ON : RelayState::OFF;
     }
};

class IOB_IOTMessageSendedEventArgs : public IOB_IOTEventArg
{
private:
     int _idx;
     RelayState _state;
     SendProtole _protocole;

public:
     IOB_IOTMessageSendedEventArgs(int idx, RelayState state, SendProtole protocole) : IOB_IOTEventArg()
     {
          _idx = idx;
          _state = state;
          _protocole = protocole;
     }
     IOB_IOTMessageSendedEventArgs(int idx, RelayState state, SendProtole protocole, String message) : IOB_IOTEventArg(message)
     {
          _state = state;
     }
     IOB_IOTMessageSendedEventArgs(int idx, RelayState state, SendProtole protocole, std::vector<String> messages) : IOB_IOTEventArg(messages)
     {
          _state = state;
     }
     int Idx()
     {
          return _idx;
     }
     RelayState State()
     {
          return _state;
     }
     void State(RelayState state)
     {
          _state = state;
     }
     String StateString()
     {
          return RelayStateConverter::toString(_state);
     }
     SendProtole Protocole()
     {
          return _protocole;
     }
     String SendProtoleString()
     {
          return SendProtoleConverter::toString(_protocole);
     }
};

class IOB_IOTMessageRecevedEventArgs : public IOB_IOTEventArg
{
private:
     int _idx;
     RelayState _state;
     SendProtole _protocole;

public:
     IOB_IOTMessageRecevedEventArgs(int idx, RelayState state, SendProtole protocole) : IOB_IOTEventArg()
     {
          _idx = idx;
          _state = state;
          _protocole = protocole;
     }
     IOB_IOTMessageRecevedEventArgs(int idx, RelayState state, SendProtole protocole, String message) : IOB_IOTEventArg(message)
     {
          _state = state;
     }
     IOB_IOTMessageRecevedEventArgs(int idx, RelayState state, SendProtole protocole, std::vector<String> messages) : IOB_IOTEventArg(messages)
     {
          _state = state;
     }
     int Idx()
     {
          return _idx;
     }
     RelayState State()
     {
          return _state;
     }
     void State(RelayState state)
     {
          _state = state;
     }
     String StateString()
     {
          return RelayStateConverter::toString(_state);
     }
     SendProtole Protocole()
     {
          return _protocole;
     }
     String SendProtoleString()
     {
          return SendProtoleConverter::toString(_protocole);
     }
};

class IOB_IOTWifiStateChangedEventArgs : public IOB_IOTEventArg
{
private:
     int _state;

public:
     IOB_IOTWifiStateChangedEventArgs() : IOB_IOTEventArg()
     {
          _state = 0;
     }
     IOB_IOTWifiStateChangedEventArgs(int state) : IOB_IOTEventArg()
     {
          _state = state;
     }
     IOB_IOTWifiStateChangedEventArgs(int state, String message) : IOB_IOTEventArg(message)
     {
          _state = state;
     }
     IOB_IOTWifiStateChangedEventArgs(String message) : IOB_IOTEventArg(message)
     {
     }
     IOB_IOTWifiStateChangedEventArgs(std::vector<String> messages) : IOB_IOTEventArg(messages)
     {
     }
     IOB_IOTWifiStateChangedEventArgs(int state, std::vector<String> messages) : IOB_IOTEventArg(messages)
     {
          _state = state;
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
     IOB_IOTMqttStateChangedEventArgs() : IOB_IOTEventArg()
     {
          _state = 0;
     }
     IOB_IOTMqttStateChangedEventArgs(int state) : IOB_IOTEventArg()
     {
          _state = state;
     }
     IOB_IOTMqttStateChangedEventArgs(int state, String message) : IOB_IOTEventArg(message)
     {
          _state = state;
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

typedef IOB_IOTEventHandler<IOB_IOTWifiStateChangedEventArgs> IOB_IOTWifiStateEventHandler;
typedef IOB_IOTEventHandler<IOB_IOTMqttStateChangedEventArgs> IOB_IOTMqttStateEventHandler;
typedef IOB_IOTEventHandler<IOB_IOTMessageSendedEventArgs> IOB_IOTMessageSendedventHandler;
typedef IOB_IOTEventHandler<IOB_IOTMessageRecevedEventArgs> IOB_IOTMessageRecevedEventHandler;

#endif /* IOB_IOTEVENT_H */