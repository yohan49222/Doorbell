class IOB_IOTEventArgs
{
private:
  int _idx;
  int _state;

public:
  IOB_IOTEventArgs(int idx, int state)
  {
    _idx = idx;
    _state = state;
  }
  int Idx() {
    return _idx;
  }
  int State()
  {
    return _state;
  }
};

template<typename T>
class IOB_IOTEvent
{
private:
  std::vector<std::function<void(T&)>> m_EventHandlers;


public:
  void addListener(std::function<void(T&)> handler)
  {
     m_EventHandlers.push_back(handler);
  }

  // void removeListener(std::function<void(T&)> handler)
  //{
  //   // etc etc
  // }

  virtual void fire(T &args)
  {
    for(auto handler : m_EventHandlers)
    {
      handler(args);
    }
  }
};