#ifndef IOB_IOTBUTTOBPRESSED_H
#define IOB_IOTBUTTOBPRESSED_H

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

#endif /* IOB_IOTBUTTOBPRESSED_H */