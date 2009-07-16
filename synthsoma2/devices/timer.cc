#include "synthsoma2/devices/timer.h"

namespace synthsoma2
{

  Timer::Timer() :
    ts_(0)
  {

  }
  
  Timer::~Timer()
  {

  }

  void Timer::run()
  {

  }

  void Timer::shutdown()
  {

  }

  void Timer::setTimeStamp(timestamp_t ts) 
  {
    ts_ = ts; 
  }

  void Timer::setDeviceID(sn::eventsource_t id) 
  {
    src_ = id; 
  }

  void Timer::ecycle(ecyclecnt_t cnt)
  {
    // * append a new event
    sn::EventTX_t oute; 
    // broadcast
    for (int i = 0; i < sn::ADDRBITS; i++) {
      oute.destaddr[i] = true;
    }
    
    oute.event.src = src_; 
    oute.event.cmd = TIMECMD; 
    oute.event.data[0] = (ts_ >> 48) & 0xFFFF; 
    oute.event.data[1] = (ts_ >> 32) & 0xFFFF; 
    oute.event.data[2] = (ts_ >> 16) & 0xFFFF; 
    oute.event.data[3] = (ts_ >> 0) & 0xFFFF; 
    outboundevents_.push_back(oute); 
    
  }
  

  eventtxlist_t Timer::getTXEvents() {
    // FIXME: we have got to do something about this double-copying. 
    
    eventtxlist_t oute = outboundevents_; 
    outboundevents_.clear(); 
    return oute; 
  }

  void Timer::sendEvents(const eventlist_t &) 
  {
    // no op 
  }

  



}
