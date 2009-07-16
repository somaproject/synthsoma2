#ifndef __SYNTHSOMA2_EVENTBUS_H__
#define __SYNTHSOMA2_EVENTBUS_H__

#include <synthsoma2/types.h>
#include <synthsoma2/eventdevice.h>
#include <synthsoma2/eventrouter.h>
#include <map>

namespace synthsoma2
{
  class EventBus
  {
    /*
      Largely handles managing the devices, and serves as a higher-order
      wrapper around the EventRouter

      
      
    */
  public:
    typedef std::map<sn::eventsource_t, pEventDevice_t> devicemap_t;     
    typedef std::pair<sn::eventsource_t, pEventDevice_t> devicepair_t;     

    EventBus(devicemap_t devices); 
    ~EventBus(); 

    void run(); 
    
    void shutdown(); 
    void event_cycle_callback(ecyclecnt_t cnt);

    void eventcycle(); 
    std::pair<size_t, size_t> getTotalCounts(); 

  private:
    

    devicemap_t devices_; 
    
    pEventRouter_t router_; 

    
  }; 


}

#endif 
