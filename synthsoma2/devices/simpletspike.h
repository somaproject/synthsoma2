#ifndef __SYNTHSOMA2_SIMPLETSPIKE_H__
#define __SYNTHSOMA2_SIMPLETSPIKE_H__

#include <synthsoma2/eventdevice.h>
#include <synthsoma2/datadevice.h>


namespace synthsoma2
{

  class SimpleTSpike : public IEventDevice, public IDataDevice, boost::noncopyable
  {
    /* A stupid data source that generates tspikes
       at a given rate. */ 

    
  public:
    SimpleTSpike(size_t everyNEvents); 
    
    void ecycle(ecyclecnt_t); 
    void setDeviceID(sn::eventsource_t id); 
    eventtxlist_t getTXEvents(); 
    void sendEvents(const eventlist_t &); 
    void run(); 
    void shutdown(); 

    // Data interface
    void setDeviceDataSrc(sn::datasource_t id) ; 
    void visitSubmitData(DataBus * db);  // visitor-like callback. 

  private:
    size_t spikenum_; 
    size_t everyN_; 
    
    sn::eventsource_t esrc_; 
    sn::datasource_t dsrc_; 
    boost::mutex mutex_;
    int eventToSend_; 

    somatime_t somatime_; 

  }; 

  typedef boost::shared_ptr<SimpleTSpike> pSimpleTSpike_t; 


}

#endif 
