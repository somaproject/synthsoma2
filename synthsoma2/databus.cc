#include <synthsoma2/databus.h>
#include <synthsoma2/datadevice.h>
#include <boost/foreach.hpp>

namespace synthsoma2 { 
  DataBus::DataBus(devicemap_t dm) :
    devices_(dm), 
    running_(false), 
    newevent_(false), 
    pthread_(NULL)
  {
    
    
  }

  void DataBus::run()
  {

    BOOST_FOREACH(devicepair_t p, devices_)
      {
	p.second->setDeviceDataSrc(p.first); 
      }
    
    // then start the devices

    running_ = true; 
    if(datasink_) {
      datasink_->run(); 
    }
    pthread_ = new boost::thread(&DataBus::ecycle_triggered_thread, this); 

  }

  void DataBus::shutdown()
  {
    running_ = false; 
    
    if(pthread_) {
      cond_.notify_one();

      pthread_->join(); 
    }
    if(datasink_) 
      datasink_->shutdown(); 
    
  }

  void DataBus::setDataSink(pDataSink_t ds)
  {
    if(running_)
      throw std::runtime_error("Running, can't set data sink"); 
  
    if(datasink_)
      throw std::runtime_error("Data sink already set"); 

    datasink_ = ds;
    
  }
  
  void DataBus::ecycle(ecyclecnt_t cnt)
  {
    {
      boost::lock_guard<boost::mutex> lock(mutex_);
      newevent_=true;
    }
    cond_.notify_one();
  }
  
  void DataBus::ecycle_triggered_thread()
  {
    while(running_) {
      // wait for condition variable (OR TIMEOUT?)
      boost::unique_lock<boost::mutex> lock(mutex_); 
      
      while(!newevent_ and running_) 
	{
	  cond_.wait(lock); 
	}
      
      // poll the relevant subset of devices to get their data
      BOOST_FOREACH(devicepair_t p, devices_) {
	p.second->visitSubmitData(this); 
	
      }
      
      newevent_ = false; 
    }
    
  }

  void DataBus::newData(sn::datasource_t src, const sn::TSpike_t &ts)
  {

    if(datasink_) {

      
      DataBuffer * db = new DataBuffer(); 
      
      db->src = src; 
      db->typ = sn::TSPIKE; 
      size_t len; 
      
      
      sn::pDataPacket_t dp = sn::rawFromTSpikeForTX(ts, 0, &len); 
      
      memcpy(db->getFrameStartPtr(), &(dp->body[0]), len); 

      db->setFrameLen(len); 

      datasink_->sendData(db);
      SS2L_(info) << "DataBus: sending TSpike from src = " 
		  << (int)src ; 
    }
    
    stats_.submittedPackets++; 

  }

  void DataBus::newData(sn::datasource_t src, const sn::Wave_t & w)
  {
    if(datasink_) {
      
      DataBuffer * db = new DataBuffer(); 

      db->src = src; 
      db->typ = sn::WAVE; 
      size_t len; 
      
      
      sn::pDataPacket_t dp = sn::rawFromWaveForTX(w,  &len); 
      
      memcpy(db->getFrameStartPtr(), &(dp->body[0]), len); 

      db->setFrameLen(len); 

      datasink_->sendData(db);

      SS2L_(info) << "DataBus: sending WAVE from src = " 
		  << (int)src ; 

    }

    stats_.submittedPackets++; 
  }

  void DataBus::newData(sn::datasource_t src, const sn::Raw_t & r) 
  {

    if(datasink_) {
      DataBuffer * db = new DataBuffer(); 

      db->src = src; 
      db->typ = sn::RAW; 
      size_t len; 
      
      
      sn::pDataPacket_t dp = sn::rawFromRawForTX(r, 0,  &len); 
      
      memcpy(db->getFrameStartPtr(), &(dp->body[0]), len); 

      db->setFrameLen(len); 

      datasink_->sendData(db);


      SS2L_(info) << "DataBus: sending RAW from src = " 
		  << (int)src ; 

    }
    stats_.submittedPackets++; 
  }
  
  DataBusStats DataBus::getStats()
  {
    // FIXME this should be protected by a mutex
    return stats_; 
  }
  

}

