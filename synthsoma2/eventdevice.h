#ifndef __SYNTHSOMA2_EVENTDEVICE_H__
#define __SYNTHSOMA2_EVENTDEVICE_H__

#include <synthsoma2/types.h>

namespace synthsoma2
{
  /* 
     ABC for the generic EventDevice. Note that 
     these methods are required to be thread-safe. 

  */ 
  class IEventDevice {
  public:
    virtual void ecycle(ecyclecnt_t) = 0; 
    virtual void setDeviceID(sn::eventsource_t id) = 0; 
    virtual eventtxlist_t getTXEvents() = 0; 
    virtual void sendEvents(const eventlist_t &) = 0; 
    virtual void run() = 0; 
    virtual void shutdown() = 0; 

  }; 
  
  typedef boost::shared_ptr<IEventDevice> pEventDevice_t; 
  
}

#endif
