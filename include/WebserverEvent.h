class WebserverEventArgs
{
private:
  int _idx;
  int _state;

public:
  WebserverEventArgs(int idx, int state)
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
class WebserverEvent
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