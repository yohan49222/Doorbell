class IOB_IOTEventArg
{
private:
  std::vector<String> _messageList;

public:
  void AddMessage(String message)
  {
    _messageList.push_back(message);
  }
  void AddMessages(std::vector<String> messages)
  {
    for (String m : messages)
    {
      _messageList.push_back(m);
    }
  }
  virtual std::vector<String> MessageList()
  {
    return _messageList;
  }
};

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

template <typename T>
class IOB_IOTEventHandler
{
private:
  std::function<void(T &)> m_EventHandlers = 0L;

public:
  IOB_IOTEventHandler()
  {
    m_EventHandlers = 0L;
  }
  void setHandler(std::function<void(T &)> handler)
  {
    m_EventHandlers = handler;
  }
  virtual void fire(T &args)
  {
    if (m_EventHandlers)
    {
      m_EventHandlers(args);
    }
  }
};
