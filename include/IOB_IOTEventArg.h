#ifndef IOB_IOTEVENTARG_H
#define IOB_IOTEVENTARG_H

class IOB_IOTEventArg
{
private:
     std::vector<String> _messageList;
     bool _handled = false;

public:
     IOB_IOTEventArg(){}
     IOB_IOTEventArg(String message)
     {
          _messageList.push_back(message);
     }
     IOB_IOTEventArg(std::vector<String> messages)
     {
          for (String m : messages)
          {
               _messageList.push_back(m);
          }
     }
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
     void Handled(bool handled)
     {
          _handled = handled;
     }
     bool Handled()
     {
          return _handled;
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

#endif /* IOB_IOTEVENTARG_H */
