#include "simpletspike.h"
#include <boost/foreach.hpp>
#include <somanetwork/tspike.h>

namespace synthsoma2 {

  SimpleTSpike::SimpleTSpike() : 
    esrc_(0), 
    dsrc_(0), 
    eventToSend_(0), 
    somatime_(0), 
    tspikepos_(0), 
    cycletimeset_(false)
  {
    
  }

  void SimpleTSpike::addTSpike(const sn::TSpike_t & ts)
  {
    tspikes_.push_back(ts); 
    maxtime_ = tspikes_[tspikes_.size() -1].time; 
  }
  void SimpleTSpike::ecycle(ecyclecnt_t cnt)
  {
    // this is JUST FOR TIMING
    boost::lock_guard<boost::mutex> lock(ecyclemutex_);
 
  }

  void SimpleTSpike::setDeviceID(sn::eventsource_t id)
  {
    esrc_ = id; 
  }


  void SimpleTSpike::settime(somatime_t t)
  {
    /* This only exists so we can easily test
       the module without having to generate real events */ 

    somatime_ = t; 
  }

  void SimpleTSpike::run() 
  {

  }

  void SimpleTSpike::shutdown()
  {


  }
  
  const optEventTX_t SimpleTSpike::getTXEvent()
  {
    // this is JUST FOR TIMING
    boost::lock_guard<boost::mutex> lock(ecyclemutex_);
    optEventTX_t ot; 
    return ot; 

  }

  void SimpleTSpike::sendEvent(const sn::Event_t & evt)
  {
    boost::lock_guard<boost::mutex> lock(ecyclemutex_);

    if (evt.src == 0x00 && evt.cmd == 0x10 )
      {
	// this is the time
	somatime_t stime = 0; 
	stime = evt.data[0]; 
	stime = stime << 16; 
	stime |= evt.data[1]; 
	stime = stime << 16; 
	stime |= evt.data[2]; 
	settime(stime); 
      }
  }

  
  void SimpleTSpike::setDeviceDataSrc(sn::datasource_t id) 
  {
    dsrc_ = id; 

  }

  void SimpleTSpike::visitSubmitData(IDataBus * db)
  {
    boost::lock_guard<boost::mutex> lock(ecyclemutex_);

    if (!tspikes_.empty()) { 
      if(!cycletimeset_){
	cyclestarttime_ = somatime_; 
	cycletimeset_ = true; 
      } else {
	// normal operation
	
	while (somatime_ - cyclestarttime_  > tspikes_[tspikepos_].time) {
	  // send the tspike
	  sn::TSpike_t ts = tspikes_[tspikepos_]; 
	  ts.time = cyclestarttime_ + ts.time; 
	  db->newData(dsrc_, ts);       
	  
	  // now consider incrementing the tspikepos_, cyclestarttime; 
	  tspikepos_++;
	  if (tspikepos_ == tspikes_.size()) {
	    tspikepos_ = 0; 
	    cyclestarttime_ += tspikes_[tspikes_.size() -1].time; 
	  }
	}
	
      }
      
    }
  }

}
