#ifndef __SYNTHSOMA2_NETEVENTSERVER_H__
#define __SYNTHSOMA2_NETEVENTSERVER_H__

#include <synthsoma2/types.h>
#include <synthsoma2/eventdevice.h>
#include <synthsoma2/neteventsender.h>

namespace synthsoma2
{
  /*
    This is the multithreaded synthetic event server. 
    The interface to the world is the standard thread-safe 
    
    THis is basically an aggregator for:
    NetEventSender
    NetEventReTX
    NetEventReceiver
    
     
   */ 
  class NetEventServer : public IEventDevice, 
			 boost::noncopyable
  {
  public:
    // factory methods
    static pNetEventServer_t createINet(std::string ip); 
    static pNetEventServer_t createDomain(boost::filesystem::path root); 
    
    // run()
    void run(); 
    void shutdown(); 
    void setDeviceID(sn::eventsource_t id); 

    // 
    void ecycle(ecyclecnt_t); 

    const optEventTX_t getTXEvent(); 
    void sendEvent(const sn::Event_t &); 
    // Assume each sendEvents invocation constitutes a single
    // ecycle's worth of events? 
    
    ~NetEventServer();
  private:
    NetEventServer(pNetEventSender_t); 
    pNetEventSender_t pNetEventSender_; 
    
    
  }; 
  
  typedef boost::shared_ptr<NetEventServer> pNetEventServer_t; 
  
}

#endif // __SYNTHSOMA2_NETEVENTSERVER_H__
