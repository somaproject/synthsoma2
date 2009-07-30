#include "dspboard.h"
#include <boost/foreach.hpp>
#include <somanetwork/tspike.h>
#include <somadspio/mockdspboard.h>
#include <algorithm>


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
    dspboard_->setDataTXCallback(boost::bind(&DSPBoard::senddata_cb, this, _1)); 
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


      // aggregate in and send the events  to the mock DSP board
      sn::EventTXList_t etxl; 
      {
	
	boost::mutex::scoped_lock lock(eventinmutex_);
	for(std::list<somanetwork::Event_t>::iterator ei = inevents_.begin(); 
	    ei != inevents_.end(); ++ei) {
	  sn::EventTX_t etx; 
	  etx.destaddr[esrc_] = true; 
	  etx.event = *ei; 
	  etxl.push_back(etx); 
	}
	inevents_.clear(); 
      }

      dspboard_->sendEvents(etxl); 


      newevent_ = false; 
      
      samplepos_ += 32000. / 50000. ; 
      
      while (samplepos_ > 1.0) {
	//
	boost::array<int16_t, 10> samples;

	samples_t insamples = *sampleiter_; 
  	for(int i = 0; i < 10; i++) {
	  double voltsperbit = dspboard_->getSignalScale(i); 
	  double val = round(insamples[i] /  voltsperbit); 
	  int ival = int(val); 
	  ival = std::max(ival, -32678); 
	  ival = std::min(ival, 32767); 

	  samples[i] = ival; 
  	}

 	dspboard_->addSamples(samples); 
	
	sampleiter_++; 	
	if (sampleiter_ == sampleBuffer_.end()) {
	  sampleiter_ = sampleBuffer_.begin(); 
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

    if (outevents_.empty()) {
      optEventTX_t ote; 
      return ote; 

    } else {
      optEventTX_t ote = outevents_.front(); 
      outevents_.pop_front(); 
      return ote; 
    }

  }

  void DSPBoard::sendEvent(const sn::Event_t & evt)
  {
    /*
      There's a minor impedance mismatch here, in that the DSPboard
      code wants a complete Event Cycle's worth of events at a time. 
      So we batch up all the inbound events into a buffer, and
      then send them at once. 

     */ 
    if (evt.src != 0) {
      SS2L_(warning) << "DSPBoard " << (int)dsrc_ << " :" 
		  << "received inbound event " << evt; 
    }

    boost::mutex::scoped_lock lock(eventinmutex_);
    inevents_.push_back(evt); 

  }


  void DSPBoard::setDeviceDataSrc(sn::datasource_t id) 
  {
    dsrc_ = id; 

  }

  void DSPBoard::visitSubmitData(IDataBus * db)
  {

    boost::mutex::scoped_lock lock(outdatamutex_); 
    if(!outdata_.empty()) {
      // do we do any conversion here? 
      unsigned char * pfront = outdata_.front(); 
      // now we decode it? 
      uint16_t npktlen; 
      memcpy(&npktlen, pfront, sizeof(npktlen)); 
      uint16_t hpktlen; 
      hpktlen = ntohs(npktlen); 

      unsigned char typ = pfront[2]; 
      unsigned char src = pfront[3]; 
      sn::pDataPacket_t dp(new sn::DataPacket_t); 
      dp->missing = false; 
      dp->seq = 0; 
      dp->src = src; 
      dp->typ = sn::charToDatatype(typ); 
      memcpy(&(dp->body), &pfront[2], hpktlen); 
      
      if (typ == 0)  {
	sn::TSpike_t ts =  rawToTSpike(dp);
	db->newData(src, ts); 

      } else if (typ == 1) {
	sn::Wave_t w = rawToWave(dp); 
	db->newData(src, w); 

      } else if (typ == 2) {
	sn::Raw_t r = rawToRaw(dp); 
	db->newData(src, r); 
      }

      outdata_.pop_front(); 
      delete pfront; 
      
    }

  }
  
  void DSPBoard::sendevent_cb(somanetwork::EventTX_t eb)
  {
    boost::mutex::scoped_lock lock(eventoutmutex_);
    outevents_.push_back(eb);     

  }

  void DSPBoard::senddata_cb(unsigned char * data)
  {
    boost::mutex::scoped_lock lock(outdatamutex_); 
    outdata_.push_back(data); 

  }
  

}
