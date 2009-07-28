#include "eventbus.h"
#include <boost/foreach.hpp>

namespace synthsoma2
{
  EventBus::EventBus(devicemap_t dev) :
    devices_(dev),
    eventcnt_(0), 
    eventsFromDevices_(0), 
    eventsToDevices_(0)
  {
    
//     router_->eventCycleSignal().connect(boost::bind(&EventBus::event_cycle_callback, 
// 						   this, _1)); 
    
    
  }
  
  EventBus::~EventBus()
  {


  }
  
  void EventBus::run()
  {
    /*
      Perform all of the necessary setup, and then kick off the
      devices
      
    */
    
    // first set the device ID for everyone
    BOOST_FOREACH(devicepair_t p, devices_)
      {
	p.second->setDeviceID(p.first); 
      }
    
    // then start the devices
    BOOST_FOREACH(devicepair_t p, devices_)
      {
	p.second->run(); 
      }

  }
  
  void EventBus::shutdown()
  {
    BOOST_FOREACH(devicepair_t p, devices_)
      {
	p.second->shutdown(); 
      }

    
    
  }

  void EventBus::eventcycle()
  {
    //router_->eventCycle(); 

    BOOST_FOREACH(devicepair_t p, devices_)
      {
	p.second->ecycle(eventcnt_); 
      }
    
    // collect all pending events
    BOOST_FOREACH(devicepair_t p, devices_) {
      const optEventTX_t etx = p.second->getTXEvent();
      if (etx) {
	eventsFromDevices_++; 
	//router_->addEvents(p.first, p.second->getTXEvents()); 
	for (int i = 0; i < sn::ADDRBITS; i++) {
	  if(etx->destaddr[i]) {
	    devicemap_t::iterator dmi = devices_.find(i); 
	    if (dmi != devices_.end()) {
	      eventsToDevices_++; 
	      dmi->second->sendEvent((*etx).event); 
	    }
	  }
	}
      }
    }
    
    eventcnt_++; 
  }
  
  
  void EventBus::event_cycle_callback(ecyclecnt_t cnt)
  {
    /*

      Guarantee that we service them in 'order'

     */

  }

  std::pair<size_t, size_t> EventBus::getTotalCounts() 
  {
    return std::make_pair(eventsFromDevices_, eventsToDevices_); 
    
  }

}
