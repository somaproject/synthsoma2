#include "dspboard.h"
#include <boost/foreach.hpp>
#include <somanetwork/tspike.h>
#include <somadspio/mockdspboard.h>

namespace synthsoma2 {

  DSPBoard::DSPBoard() : 
    workerThread_(NULL), 
    esrc_(0), 
    dsrc_(0), 
    running_(false), 
    dspboard_(NULL), 
    sampleBuffer_(1), 
    sampleiter_(sampleBuffer_.begin())
  {
  }

  void DSPBoard::ecycle(ecyclecnt_t cnt)
  {
    {
      boost::lock_guard<boost::mutex> lock(ecyclemutex_);
      newevent_=true;
    }
    ecyclecond_.notify_one();

    
  }

  void DSPBoard::setDeviceID(sn::eventsource_t id)
  {
    esrc_ = id; 
  }


  void DSPBoard::run() 
  {
    if(dspboard_) {
      delete dspboard_; 
    }
    dspboard_ = new somadspio::mock::MockDSPBoard (dsrc_, esrc_); 
    dspboard_->setEventTXCallback(boost::bind(&DSPBoard::sendevent_cb, this, _1)); 
    samplepos_ = 0.0; 
    // now launch runner thread
    running_ = true; 
    workerThread_ = new boost::thread(boost::bind(&DSPBoard::workerThread, this)); 
    
  }

  void DSPBoard::setSampleBuffer(const std::vector<samples_t > & data)
  {
    if (running_) {
      throw std::runtime_error("Cannot set sample buffer when device is running"); 
    }

    sampleBuffer_.clear(); 
    sampleBuffer_.rset_capacity(data.size()); 
    for (int i = 0; i < data.size(); i++) {
      sampleBuffer_.push_back(data[i]); 
    }
    sampleiter_ = sampleBuffer_.begin(); 

  }


  void DSPBoard::shutdown()
  {

    if (running_) {
      running_ = false; 
      if (workerThread_) {
	ecyclecond_.notify_one();

	workerThread_->join(); 
	delete workerThread_; 
      }

    }

    if(dspboard_) {
      delete dspboard_; 
    }

  }
  
  void DSPBoard::workerThread()
  {
    // wait for an ecycle, then on 16/25 of them, add a new sample buffer
    // 
    while(running_) {
      {
	boost::unique_lock<boost::mutex> lock(ecyclemutex_); 
	while (!newevent_ and running_) {
	  ecyclecond_.wait(lock); 	
	}
      }
      newevent_ = false; 
      
      samplepos_ += 32000. / 50000. ; 
      
      while (samplepos_ > 1.0) {
	//
	boost::array<int16_t, 10> samples;

  	for(int i = 0; i < 10; i++) {
	  // 	  // FIXME do something about the gain
   	  samples[i] = 0; 
  	}

 	dspboard_->addSamples(samples); 
	
	if (sampleiter_ == sampleBuffer_.end()) {
	  sampleiter_ = sampleBuffer_.begin(); 
	} else {
	  sampleiter_++; 
	}
	samplepos_ -= 1.0; 
	dspboard_->runloop(); 
      }

//       // etc. etc. 
      dspboard_->runloop(); 
      
    }


  }


  const optEventTX_t DSPBoard::getTXEvent()
  {
    boost::mutex::scoped_lock lock(eventoutmutex_);

    optEventTX_t ote; 
    return ote; 

//     if (outevents_.empty()) {
//       return ote; 

//     } else {
//       optEventTX_t ote = outevents_.front(); 
//       outevents_.pop_front(); 
//       return ote; 
//     }

  }

  void DSPBoard::sendEvent(const sn::Event_t & evt)
  {
//     sn::EventTX_t etx; 
//     etx.event = evt; 
//     etx.destaddr[esrc_] = true; 
//     // FIXME: mutex-protect? 
//     sn::EventTXList_t etxl; 
//     etxl.push_back(etx); 
//     dspboard_->sendEvents(etxl); 
  }

  
  void DSPBoard::setDeviceDataSrc(sn::datasource_t id) 
  {
    dsrc_ = id; 

  }

  void DSPBoard::visitSubmitData(IDataBus * db)
  {
    
  }
  
  void DSPBoard::sendevent_cb(somanetwork::EventTX_t eb)
  {
    boost::mutex::scoped_lock lock(eventoutmutex_);
    outevents_.push_back(eb);     

  }

}
