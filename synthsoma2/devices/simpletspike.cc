#include "simpletspike.h"

namespace synthsoma2 {

  SimpleTSpike::SimpleTSpike(size_t everyNEvents) :
    spikenum_(0), 
    everyN_(everyNEvents), 
    eventToSend_(0)
  {

  }
    
  void SimpleTSpike::ecycle(ecyclecnt_t cnt)
  {
    /*     */
    if ((cnt % everyN_) == 0) {
      boost::mutex::scoped_lock lock(mutex_);
      eventToSend_++; 
    }

  }

  void SimpleTSpike::setDeviceID(sn::eventsource_t id)
  {
    esrc_ = id; 
  }

  eventtxlist_t SimpleTSpike::getTXEvents()
  {

  }

  void SimpleTSpike::sendEvents(const eventlist_t &)
  {
    // FIXME get the time stamp event 
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

  void SimpleTSpike::visitSubmitData(DataBus * db)
  {
    bool tosend = false; 
    {
      boost::mutex::scoped_lock lock(mutex_);
      if (eventToSend_ > 0) {
	tosend = true; 
	eventToSend_--;
      }
    }

    if(tosend) {
      sn::TSpike_t ts; 
      ts.src = dsrc_; 
      spikenum_++; 
      ts.x.filtid = spikenum_; 
      ts.y.filtid = spikenum_; 
      ts.a.filtid = spikenum_; 
      ts.b.filtid = spikenum_; 

      // FIXME: add some stuff here
      db->newData(dsrc_, ts);       
      spikenum_++; 
    }
    
  }
  

}
