#ifndef __SYNTHSOMA2_DATABUS_H__
#define __SYNTHSOMA2_DATABUS_H__

#include <boost/thread/condition_variable.hpp>
#include <synthsoma2/types.h>
#include <synthsoma2/netdataserver.h>
#include <somanetwork/tspike.h>
#include <somanetwork/wave.h>
#include <somanetwork/raw.h>

namespace synthsoma2
{

  class IDataBus
  {
  public:
    // visitor callbacks
    virtual void newData(sn::datasource_t src, const sn::TSpike_t &) =0;  
    virtual void newData(sn::datasource_t src, const sn::Wave_t &) = 0; 
    virtual void newData(sn::datasource_t src, const sn::Raw_t &) = 0; 
  };

    class DataBus : public IDataBus, boost::noncopyable
  {
    /*
      The DataBus is simpler than the EventBus, and as a result 
      we don't need quite as much logic as the event bus. 

      That's a lie, because the data bus does the bulk of its
      work in its own thread. It polls the data devices in its
      own thread, and transferrs data to them in its own thread. 
      
      And it is a slave to the ecycle signal, which triggers
      a condition variable in the other thread. 

      Note that all of the data types have a src field, and
      that combined with the data type itself should be enough
      to pass them off. 

      Inputs: 
         src, TSpike_t, 
	 src, Wave_t, 
	 src, Raw_t

      we have a list of pDataSource_t objects, and we 
      go round-robin and invoke, visitor-pattern-style, their
      visitSubmitData(DataBus*) method, where they can then call
      one of the type-safe new data methods: 
         DataBus::newData(src, TSpike_t); 
	 DataBus::newData(src, Wave_t); 
	 DataBus::newData(src, Raw_t); 
	 

     // Performance

     data is the one place I really care about performance, because
     we're potentially sending a -ton- of large packets. So the
     architecture should be as zero-copy as possible. to do this, we
     mate the data bus more closely with the data network.
     
     
     */

  public:

    typedef std::map<sn::datasource_t, pDataDevice_t> devicemap_t;     
    typedef std::pair<sn::datasource_t, pDataDevice_t> devicepair_t;     

    DataBus(devicemap_t); 
    void ecycle(ecyclecnt_t cnt); // thread-safe
    
    void run(); 
    void shutdown(); 
    void setDataSink(pDataSink_t ds); 

    // visitor callbacks
    void newData(sn::datasource_t src, const sn::TSpike_t &); 
    void newData(sn::datasource_t src, const sn::Wave_t &); 
    void newData(sn::datasource_t src, const sn::Raw_t &); 

  private:
    devicemap_t devices_; 
    pDataSink_t datasink_; 

    bool running_; 

    void ecycle_triggered_thread(); 

    // mutex
    bool newevent_; 
    boost::condition_variable cond_;
    boost::mutex mutex_;

    boost::thread * pthread_;     
  }; 

  typedef boost::shared_ptr<DataBus> pDataBus_t; 


}
#endif
