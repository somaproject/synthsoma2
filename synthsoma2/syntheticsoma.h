#ifndef __SYNTHSOMA2_SYNTHSOMA2_H__
#define __SYNTHSOMA2_SYNTHSOMA2_H__

#include <synthsoma2/types.h>
#include <synthsoma2/eventbus.h>
#include <synthsoma2/databus.h>
#include "boost/date_time/posix_time/posix_time.hpp"


namespace synthsoma2
{
  class SynthSomaRunner
  {
    /*
      The primary core of synth soma, which takes an Event bus
      and a Data Bus and runs them, keeping track of overflow, etc. 
      
      
    */ 
  public:
    SynthSomaRunner(pEventBus_t, pDataBus_t); 
    void run();     
    void shutdown(); 
    
    void worker(const boost::system::error_code&); 
    
  private:
    bool running_; 
    void setup();
    pEventBus_t pEventBus_; 
    pDataBus_t pDataBus_; 
    
    boost::asio::io_service ioservice_;

    boost::asio::deadline_timer timer_; 
    boost::thread * pThread_; 
    void timer_arm(); 
    
    boost::posix_time::ptime lastinvoke_; 

  }; 






}



#endif 
