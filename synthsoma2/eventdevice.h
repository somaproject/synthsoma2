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
    virtual eventtxlist_t getTXEvents() = 0; 
    virtual void sendEvents(const eventlist_t &) = 0; 
  }; 
  
}

#endif
