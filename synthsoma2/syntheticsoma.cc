#include "syntheticsoma.h"

namespace synthsoma2
{
  using namespace boost::posix_time;

  SynthSomaRunner::SynthSomaRunner(pEventBus_t eb, pDataBus_t db):
    pEventBus_(eb), 
    pDataBus_(db), 
    running_(false), 
    timer_(ioservice_, boost::posix_time::microseconds(5000)), 
    pThread_(NULL)
  {


  }
  
  void SynthSomaRunner::run() 
  {
    setup(); 
    running_ = true; 
    // start independent run thread
    lastinvoke_ = microsec_clock::local_time(); 
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
    while (consumed < worktime) {
      // do an event cycle


      consumed += microseconds(20); 
    }
    
    newlastinvoke += (consumed - worktime); 
    
    lastinvoke_ = newlastinvoke; 

    if (running_) {
      timer_arm(); 
    }

  }
  
  void SynthSomaRunner::timer_arm()
  {
    timer_.async_wait(boost::bind(&SynthSomaRunner::worker, this, 
				  boost::asio::placeholders::error)); 

  }


}
