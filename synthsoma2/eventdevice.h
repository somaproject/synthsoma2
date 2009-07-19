#ifndef __SYNTHSOMA2_EVENTDEVICE_H__
#define __SYNTHSOMA2_EVENTDEVICE_H__

#include <synthsoma2/types.h>

namespace synthsoma2
{
  
  typedef boost::optional<sn::EventTX_t> optEventTX_t; 
  /* Boost:optional is used to model the possible-return of a 
     value from the getEventTX() function, without the overhead of a list. 
     
     Things to note: 
     1. directly testing the optional returns true if it is initialized
     2. *dereferencing it obtains the internal const ref
     3. Creating an uninitailized boost::optional does not invoke the constructor
     on the contained type
  */ 
  

  
  /* 
     ABC for the generic EventDevice. Note that 
     these methods are required to be thread-safe. 
     
  */ 
  class IEventDevice {
  public:
    virtual void ecycle(ecyclecnt_t) = 0; 
    virtual void setDeviceID(sn::eventsource_t id) = 0; 

    virtual const optEventTX_t getTXEvent() = 0; 
    /* Get a single TX event, may return an empty optional  */ 

    virtual void sendEvent(const sn::Event_t &) = 0; 
    // Will replace sendEvents

    virtual void run() = 0; 
    virtual void shutdown() = 0; 

  }; 
  
  typedef boost::shared_ptr<IEventDevice> pEventDevice_t; 
  
}

#endif
