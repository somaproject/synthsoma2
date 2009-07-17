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
    
  }

  void SimpleTSpike::setDeviceID(sn::eventsource_t id)
  {
    esrc_ = id; 
  }

  eventtxlist_t SimpleTSpike::getTXEvents()
  {

  }

  void SimpleTSpike::sendEvents(const eventlist_t & el)
  {
    
    BOOST_FOREACH(sn::Event_t evt, el) {
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
  
  void SimpleTSpike::setDeviceDataSrc(sn::datasource_t id) 
  {
    dsrc_ = id; 

  }

  void SimpleTSpike::visitSubmitData(IDataBus * db)
  {
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
