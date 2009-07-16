#ifndef __SYNTHSOMA2_TESTDEVICE_H__
#define __SYNTHSOMA2_TESTDEVICE_H__

#include <synthsoma2/types.h>
#include <synthsoma2/eventdevice.h> 
#include <boost/python.hpp>

#include <boost/thread/mutex.hpp>

namespace synthsoma2
{

  class TestDevice : public IEventDevice, 
		boost::noncopyable
  {
    typedef uint64_t timestamp_t; 
  public:
    TestDevice(); 
    ~TestDevice(); 
    
    // Event Device Interface
    void ecycle(ecyclecnt_t);
    void setDeviceID(sn::eventsource_t id); 
 
    eventtxlist_t getTXEvents(); 
    void sendEvents(const eventlist_t &); 
    void run(); 
    void shutdown(); 

    // debug interface, simple mutex access. 
    void debugSendTXEvents(const sn::EventTX_t & ); 
    eventlist_t debugGetReceivedEvents(); 

    boost::python::list pyGetReceivedEvents(); 
    void pySendTXEvents(const sn::EventTX_t & ); 
    
  private:
    eventtxlist_t outboundevents_; 
    eventlist_t inboundevents_; 
    sn::eventsource_t src_; 

    boost::mutex inmutex_; 
    boost::mutex outmutex_; 

  }; 

  typedef boost::shared_ptr<TestDevice> pTestDevice_t; 

}

#endif 

