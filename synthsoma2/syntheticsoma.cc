#include "syntheticsoma.h"

namespace synthsoma2
{
using namespace boost::posix_time;

SynthSomaRunner::SynthSomaRunner(pEventBus_t & eb, pDataBus_t & db):
  pEventBus_(eb), 
  pDataBus_(db), 
  running_(false), 
  pThread_(NULL),
  bencheventcnt_(0)
{
  
  
}

void SynthSomaRunner::run() 
{
  setup(); 
  running_ = true; 
  
  // start independent run thread
  lastinvoke_ = microsec_clock::local_time(); 
  lastbench_ = microsec_clock::local_time(); 
  //     timer_arm(); 
  //     bench_timer_arm(); 
  pThread_ = new boost::thread(boost::bind(&SynthSomaRunner::worker, this)); 
  
}

void SynthSomaRunner::shutdown()
{
  
  running_ = false; 
  if (pThread_) {
    pThread_->join(); 
  }
  
  pEventBus_->shutdown(); 
  
  pDataBus_->shutdown(); 
  
}

void SynthSomaRunner::setup()
{
  
  pEventBus_->run(); 
  pDataBus_->run(); 
  
}

void SynthSomaRunner::worker() 
{
  while(running_) {
    ptime newlastinvoke = microsec_clock::local_time(); 
    time_duration worktime = newlastinvoke - lastinvoke_; 
    
    // now extract off 20-usec counts and send events
    time_duration consumed = seconds(0); 
    size_t eventcnt = 0; 
    while (consumed < worktime) {
      // do an event cycle
      pEventBus_->eventcycle(); 
      pDataBus_->ecycle(0);  // FIXME add correct count? 
      bencheventcnt_ += 1; 
      eventcnt++; 
      
      consumed += microseconds(20); 
    }
    
    newlastinvoke = lastinvoke_ + consumed; 
    
    time_duration actual_duration =  microsec_clock::local_time() - newlastinvoke; 
    
    lastinvoke_ = newlastinvoke; 
    
    boost::mutex::scoped_lock lock(runnerstatsmutex_); 
    runnerstats_.eventcycles += eventcnt; 
    runnerstats_.eventsenttotal = pEventBus_->getTotalCounts().first; 
    runnerstats_.eventreceivetotal = pEventBus_->getTotalCounts().second; 
    runnerstats_.meanecycledur_us =  float(actual_duration.total_microseconds()) / eventcnt; 
  }
}

void SynthSomaRunner::benchmark()
{
  ptime newlastbench = microsec_clock::local_time(); 
  time_duration worktime = newlastbench - lastbench_; 
  boost::mutex::scoped_lock lock(runnerstatsmutex_); 
  //std::cout << " bench event cnt = " <<  bencheventcnt_ << std::endl; 
  runnerstats_.eventcyclerate = float( bencheventcnt_) / worktime.total_seconds();
  bencheventcnt_ = 0; 
  
  DataBusStats dbs = pDataBus_->getStats(); 
  
  runnerstats_.datapacketrate = 
    (dbs.submittedPackets - runnerstats_.datapackets) / worktime.total_seconds();
  runnerstats_.datapackets = dbs.submittedPackets; 
  
  lastbench_ = newlastbench; 
  
}

runnerstats_t SynthSomaRunner::getStats()
{
  boost::mutex::scoped_lock lock(runnerstatsmutex_); 
  return runnerstats_; 
  
}
}
