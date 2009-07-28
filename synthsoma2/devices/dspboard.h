#ifndef __SYNTHSOMA2_DSPBOARD_H__
#define __SYNTHSOMA2_DSPBOARD_H__

#include <synthsoma2/eventdevice.h>
#include <synthsoma2/datadevice.h>
#include <somadspio/mockdspboard.h>
#include <boost/circular_buffer.hpp>

#include <vector>

namespace synthsoma2
{

  class DSPBoard : public IEventDevice, public IDataDevice, boost::noncopyable
  {
    /* A simple interface to the DSPboard firmware that takes a
       vector of 32-bit float samples, representing "real nanovolt" samples. 

       We scale those by the the gain values of the acqserial
       interface on the dspboard, and at ~32kHz those are placed into
       the mockdspboard buffer.
       
    */       

    
  public:
    DSPBoard(); 
    typedef boost::array<double, 10> samples_t; 
    // user interface
    void setSampleBuffer(const std::vector<samples_t > & data); 

    //    void addTSpike(const sn::TSpike_t & ts); 
    void ecycle(ecyclecnt_t); 
    void setDeviceID(sn::eventsource_t id); 

    const optEventTX_t getTXEvent(); 
    void sendEvent(const sn::Event_t &); 

    void run(); 
    void shutdown(); 
    
    // Data interface
    void setDeviceDataSrc(sn::datasource_t id) ; 
    void visitSubmitData(IDataBus * db);  // visitor-like callback. 

  private:
    boost::thread * workerThread_;

    sn::eventsource_t esrc_; 
    sn::datasource_t dsrc_; 
    boost::mutex mutex_;

    bool running_; 
    somadspio::mock::MockDSPBoard * dspboard_; 

    void sendevent_cb(somanetwork::EventTX_t evt); 
    boost::mutex eventoutmutex_; 
    std::list<somanetwork::EventTX_t> outevents_; 

    void workerThread(); 
    
    double samplepos_; 
    boost::circular_buffer<samples_t> sampleBuffer_; 
    boost::circular_buffer<samples_t>::iterator sampleiter_; 

    bool newevent_; 
    boost::condition_variable ecyclecond_;
    boost::mutex ecyclemutex_;

  }; 

  typedef boost::shared_ptr<DSPBoard> pDSPBoard_t; 


}

#endif 
