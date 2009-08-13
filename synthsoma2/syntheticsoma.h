#ifndef __SYNTHSOMA2_SYNTHSOMA2_H__
#define __SYNTHSOMA2_SYNTHSOMA2_H__

#include <synthsoma2/types.h>
#include <synthsoma2/eventbus.h>
#include <synthsoma2/databus.h>
#include "boost/date_time/posix_time/posix_time.hpp"


namespace synthsoma2
{

  struct runnerstats_t
  {
    size_t eventcycles; 
    float eventcyclerate; 
    size_t eventsenttotal;
    size_t eventreceivetotal; 
    size_t datapackets; 
    size_t datapacketrate; 
    double meanecycledur_us; 
    runnerstats_t() :
      eventcycles(0),
      eventcyclerate(0.),
      eventsenttotal(0), 
      eventreceivetotal(0), 
      datapackets(0), 
      datapacketrate(0),
      meanecycledur_us(0.0)
    {
    }
    
  };
  
  class SynthSomaRunner
  {
    /*
      The primary core of synth soma, which takes an Event bus
      and a Data Bus and runs them, keeping track of overflow, etc. 
      
      
    */ 
  public:
    SynthSomaRunner(pEventBus_t &, pDataBus_t &); 
    void run();     
    void shutdown(); 
    
    void worker(); 
    
    runnerstats_t getStats(); 
    
  private:
    bool running_; 
    void setup();
    pEventBus_t pEventBus_; 
    pDataBus_t pDataBus_; 
    
    boost::thread * pThread_; 

    void benchmark(); 
    
    boost::posix_time::ptime lastinvoke_; 

    boost::posix_time::ptime lastbench_; 
    size_t bencheventcnt_; 

    runnerstats_t runnerstats_; 
    boost::mutex runnerstatsmutex_; 
    
    
  }; 






}



#endif 
