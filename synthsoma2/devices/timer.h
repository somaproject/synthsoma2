#ifndef __SYNTHSOMA2_TIMER_H__
#define __SYNTHSOMA2_TIMER_H__

#include <synthsoma2/types.h>
#include <synthsoma2/eventdevice.h> 

namespace synthsoma2
{

  class Timer : public IEventDevice, 
		boost::noncopyable
  {
    typedef uint64_t timestamp_t; 
  public:
    Timer(); 
    ~Timer(); 
    
    void setTimeStamp(timestamp_t ts); 
    
    // Event Device Interface
    void ecycle(ecyclecnt_t);
    void setDeviceID(sn::eventsource_t id); 
 
    eventtxlist_t getTXEvents(); 
    void sendEvents(const eventlist_t &); 
    void run(); 
    void shutdown(); 

  private:
    timestamp_t ts_; 
    eventtxlist_t outboundevents_; 
    sn::eventsource_t src_; 

    const static sn::eventcmd_t TIMECMD = 0x10; 

  }; 
  typedef boost::shared_ptr<Timer> pTimer_t; 
  
}

#endif 

