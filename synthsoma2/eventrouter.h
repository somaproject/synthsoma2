#ifndef __SYNTHSOMA2_EVENTROUTER_H__
#define __SYNTHSOMA2_EVENTROUTER_H__

#include <synthsoma2/types.h>

#include <bitset>
#include <boost/signal.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
namespace synthsoma2
{
  
  class EventRouter : public boost::noncopyable
  {
  public:
    typedef boost::signal<void (ecyclecnt_t)> ecyclesig_t; 

    EventRouter(); 
    ~EventRouter(); 
    
    
    void enableDevice(sn::eventsource_t src); 
    /*
      Enable this device to receive events; otherwise we just
      discard the packet. This is used to prevent events just 
      building up for devices that will never acquire them. 
      
    */
    ecyclecnt_t getECycleCnt() {
      return ecyclecnt_; 
    }

    // Event cycle timing
    void eventCycle(); 

    ecyclesig_t & eventCycleSignal() {
      return ecyclesig_; 
    }

    void processEventCycle(); 
    /*
      Survey all the buffers, and take the head event, and push
      it into the output buffer
      
    */

    void addEvent(sn::eventsource_t src, sn::EventTX_t et); 
    void addEvents(sn::eventsource_t src, const  eventtxlist_t & et); 
    const eventlist_t gatherEvents(sn::eventsource_t src); 
    
    typedef boost::array<size_t, MAXEVENT> routestats_t; 

    std::pair<size_t, size_t> getTotalCounts(); // NOT THREAD SAFE

  private:
    
    std::bitset<MAXEVENT> eventsEnabled_; 
    ecyclecnt_t ecyclecnt_; 

    bool cycleHasBeenProcessed_; 
    
    ecyclesig_t ecyclesig_;
    
    typedef std::list<sn::EventTX_t> inputbuffer_t; 
    static const size_t INPUTBUFFER_WARN = 1000;
    boost::ptr_vector<inputbuffer_t> inputbuffers_; 
    
    typedef std::list<sn::Event_t> outputbuffer_t; 
    static const size_t OUTPUTBUFFER_WARN = 1000;
    boost::ptr_vector<outputbuffer_t> outputbuffers_; 

    routestats_t eventSendCount_; 
    routestats_t eventReceiveCount_; 

    size_t eventSendCountTotal_; 
    size_t eventReceiveCountTotal_; 
    
  }; 

  typedef boost::shared_ptr<EventRouter> pEventRouter_t; 

}

#endif
