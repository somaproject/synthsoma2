#include <synthsoma2/databus.h>
#include <synthsoma2/datadevice.h>
#include <boost/foreach.hpp>

namespace synthsoma2 { 
  DataBus::DataBus(devicemap_t dm) :
    devices_(dm), 
    datasink_(NULL), 
    running_(false), 
    newevent_(false), 
    pthread_(NULL)
  {
    
    
  }

  void DataBus::run()
  {
    running_ = true; 
    pthread_ = new boost::thread(&DataBus::ecycle_triggered_thread, this); 

  }

  void DataBus::shutdown()
  {
    running_ = false; 
    
    if(pthread_) 
      pthread_->join(); 
    
  }

  void DataBus::setDataSink(IDataSink * s)
  {
    if(running_)
      throw std::runtime_error("Running, can't set data sink"); 
  
    if(datasink_)
      throw std::runtime_error("Data sink already set"); 

    datasink_ = s;
    
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
      
      while(!newevent_) 
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

  void DataBus::newData(sn::datasource_t src, const sn::TSpike_t &)
  {
    // FIXME, at the moment we just create empty packets
    DataBuffer * db = new DataBuffer(); 
    db->src = src; 
    db->typ = sn::TSPIKE; 
    
    // FIXME here's where the magic happens
    if(datasink_) {
      datasink_->sendData(db);
    }
    
  }

  void DataBus::newData(sn::datasource_t src, const sn::Wave_t &)
  {
    DataBuffer * db = new DataBuffer(); 
    // FIXME
  }

  void DataBus::newData(sn::datasource_t src, const sn::Raw_t &) 
  {
    DataBuffer * db = new DataBuffer(); 
    // FIXME
  }
  

  

}

