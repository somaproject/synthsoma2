#include "synthsoma2/devices/testdevice.h"
#include <boost/foreach.hpp>
#include <algorithm>

namespace synthsoma2
{

  TestDevice::TestDevice()
  {

  }
  
  TestDevice::~TestDevice()
  {

  }


  void TestDevice::setDeviceID(sn::eventsource_t id) 
  {
    src_ = id; 
  }

  void TestDevice::ecycle(ecyclecnt_t cnt)
  {
    
  }
  
  void TestDevice::run() 
  {


  }

  void TestDevice::shutdown() 
  {


  }


  const optEventTX_t TestDevice::getTXEvent() {

    optEventTX_t oute ;
    boost::mutex::scoped_lock lock(outmutex_); 
    if (!outboundevents_.empty()) { 
      oute = boost::make_optional(outboundevents_.front()); 
      outboundevents_.pop_front(); 
    }

    return oute; 

  }

  void TestDevice::sendEvent(const sn::Event_t & etx) 
  {
    boost::mutex::scoped_lock lock(inmutex_); 
    
    inboundevents_.push_back(etx); 
    
  }

  void TestDevice::debugSendTXEvents(const sn::EventTX_t & e) {
    boost::mutex::scoped_lock lock(outmutex_); 
    outboundevents_.push_back(e); 

  }

  eventlist_t TestDevice::debugGetReceivedEvents() 
  {
    boost::mutex::scoped_lock lock(inmutex_); 
    eventlist_t el = inboundevents_; 
    inboundevents_.clear(); 
    return el; 

  }




}
