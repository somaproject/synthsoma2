#include "syntheticsoma.h"

namespace synthsoma2
{
  using namespace boost::posix_time;
  
  SynthSomaRunner::SynthSomaRunner(pEventBus_t & eb, pDataBus_t & db):
    pEventBus_(eb), 
    pDataBus_(db), 
    running_(false), 
    timer_(ioservice_, boost::posix_time::microseconds(5000)), 
    pThread_(NULL),
    benchtimer_(ioservice_, boost::posix_time::seconds(1)), 
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
    timer_arm(); 
    bench_timer_arm(); 
    pThread_ = new boost::thread(boost::bind(&boost::asio::io_service::run, &ioservice_));
    
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

  void SynthSomaRunner::worker(const boost::system::error_code&)
  {
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
    
    lastinvoke_ = newlastinvoke; 

    if (running_) {
      timer_arm(); 
    }

    boost::mutex::scoped_lock lock(runnerstatsmutex_); 
    runnerstats_.eventcycles += eventcnt; 
    runnerstats_.eventsenttotal = pEventBus_->getTotalCounts().first; 
    runnerstats_.eventreceivetotal = pEventBus_->getTotalCounts().second; 

  }
  
  void SynthSomaRunner::timer_arm()
  {
    timer_.expires_from_now( boost::posix_time::microseconds(5000));

    timer_.async_wait(boost::bind(&SynthSomaRunner::worker, this, 
				  boost::asio::placeholders::error)); 
    
  }
  

  void SynthSomaRunner::bench_timer_arm()
  {
    if (!running_)
      return; 
    benchtimer_.expires_from_now( boost::posix_time::seconds(1));

    benchtimer_.async_wait(boost::bind(&SynthSomaRunner::benchmark, this, 
				       boost::asio::placeholders::error)); 
    
  }
  
  void SynthSomaRunner::benchmark(const boost::system::error_code&)
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

    bench_timer_arm(); 

  }
  runnerstats_t SynthSomaRunner::getStats()
  {
    boost::mutex::scoped_lock lock(runnerstatsmutex_); 
    return runnerstats_; 

  }
}
