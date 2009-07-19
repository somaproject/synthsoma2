#include "eventbus.h"
#include <boost/foreach.hpp>

namespace synthsoma2
{
  EventBus::EventBus(devicemap_t dev) :
    devices_(dev), 
    router_(new EventRouter())
  {
    
    router_->eventCycleSignal().connect(boost::bind(&EventBus::event_cycle_callback, 
						   this, _1)); 
    
    
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
	router_->enableDevice(p.first); 
      }
    
    // then start the devices
    BOOST_FOREACH(devicepair_t p, devices_)
      {
	p.second->run(); 
      }

  }
  
  void EventBus::shutdown()
  {
    
    
  }

  void EventBus::eventcycle()
  {
    router_->eventCycle(); 

    // collect all pending events
    BOOST_FOREACH(devicepair_t p, devices_) {
      router_->addEvents(p.first, p.second->getTXEvents()); 
    }
    
    router_->processEventCycle(); 
    
    // now push the events back out
    BOOST_FOREACH(devicepair_t p, devices_) {
      p.second->sendEvents(router_->gatherEvents(p.first)); 
    }
    

  }
  
  
  void EventBus::event_cycle_callback(ecyclecnt_t cnt)
  {
    /*

      Guarantee that we service them in 'order'

     */

    BOOST_FOREACH(devicepair_t p, devices_)
      {
	p.second->ecycle(cnt); 
      }
  }

  std::pair<size_t, size_t> EventBus::getTotalCounts() 
  {
    return router_->getTotalCounts(); 
    
  }

}
