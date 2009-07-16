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


  eventtxlist_t TestDevice::getTXEvents() {
    // FIXME: we have got to do something about this double-copying. 

    boost::mutex::scoped_lock lock(outmutex_); 
    eventtxlist_t oute = outboundevents_; 
    outboundevents_.clear(); 
    return oute; 

  }

  void TestDevice::sendEvents(const eventlist_t & el) 
  {
    boost::mutex::scoped_lock lock(inmutex_); 
    
    inboundevents_.insert(inboundevents_.end(), el.begin(), el.end());
    
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

  
  boost::python::list TestDevice::pyGetReceivedEvents()
  {
    boost::python::list l; 
    eventlist_t eventlist = TestDevice::debugGetReceivedEvents(); 
    
    BOOST_FOREACH(sn::Event_t e,  eventlist) {
      l.append(e); 
    }
    
    return l; 
  }

  void TestDevice::pySendTXEvents(const sn::EventTX_t & e)
  {
    debugSendTXEvents(e); 
  }
    



}
