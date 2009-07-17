#include "simpletspike.h"
#include <boost/foreach.hpp>

namespace synthsoma2 {

  SimpleTSpike::SimpleTSpike(size_t everyNEvents) :
    spikenum_(0), 
    everyN_(everyNEvents), 
    esrc_(0), 
    dsrc_(0), 
    eventToSend_(0), 
    somatime_(0)
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
	  somatime_ = stime; 
	}
    }
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
      ts.time = somatime_; 
      sn::TSpikeWave_t * wv[] = {&ts.x, &ts.y, &ts.a, &ts.b}; 
      for (int i = 0; i < 4; i++) {
	wv[i]->filtid = spikenum_; 
	for (int j = 0; j < 32; j++) {
	  wv[i]->wave[j] = i * 0x10000 + j; 
	}
      }
      
      // FIXME: add some stuff here
      db->newData(dsrc_, ts);       
      spikenum_++; 
    }
    
  }
  

}
