#ifndef __SYNTHSOMA2_EVENTBUS_H__
#define __SYNTHSOMA2_EVENTBUS_H__

#include <synthsoma2/types.h>
#include <synthsoma2/eventdevice.h>
#include <map>

namespace synthsoma2
{

  typedef uint64_t eventcnt_t; 

  class EventBus : public boost::noncopyable
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
    
//     EventRouter::ecyclesig_t &  eventCycleSignal() {
//       return router_->eventCycleSignal(); 
//     }
    
  private:
    

    devicemap_t devices_; 
    eventcnt_t eventcnt_; 
    
    size_t eventsFromDevices_; 
    size_t eventsToDevices_; 
//     pEventRouter_t router_; 

    
  }; 


}

#endif 
