#ifndef __SYNTHSOMA2_SIMPLETSPIKE_H__
#define __SYNTHSOMA2_SIMPLETSPIKE_H__

#include <synthsoma2/eventdevice.h>
#include <synthsoma2/datadevice.h>
#include <vector>

namespace synthsoma2
{

  class SimpleTSpike : public IEventDevice, public IDataDevice, boost::noncopyable
  {
    /* An example device that takes in a list of tspikes where
       the times are a list of times relative to zero. In a sense,
       they describe a little timeline of spikes that will be played
       back, over and over. 
       
       For example, if there was only one tspike, with time 50000, 
       then we'd send that same spike over and over every second. 

       If there were two spikes with times 50000 and 100000, respectively, 
       then we'd send those two over and over. 
       
    */       

    
  public:
    SimpleTSpike(); 
    void addTSpike(const sn::TSpike_t & ts); 
    void ecycle(ecyclecnt_t); 
    void setDeviceID(sn::eventsource_t id); 

    const optEventTX_t getTXEvent(); 
    void sendEvent(const sn::Event_t &); 

    void run(); 
    void shutdown(); 
    void settime(somatime_t); 
    
    // Data interface
    void setDeviceDataSrc(sn::datasource_t id) ; 
    void visitSubmitData(IDataBus * db);  // visitor-like callback. 

  private:
    size_t spikenum_; 
    std::vector<sn::TSpike_t> tspikes_; 
    
    sn::eventsource_t esrc_; 
    sn::datasource_t dsrc_; 
    boost::mutex mutex_;
    int eventToSend_; 

    somatime_t somatime_; 
    somatime_t maxtime_; 
    bool cycletimeset_; 
    somatime_t cyclestarttime_; 

    size_t tspikepos_; 

  }; 

  typedef boost::shared_ptr<SimpleTSpike> pSimpleTSpike_t; 


}

#endif 
