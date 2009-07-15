#ifndef __SYNTHSOMA2_NETEVENTSERVER_H__
#define __SYNTHSOMA2_NETEVENTSERVER_H__

#include <synthsoma2/types.h>
#include <synthsoma2/eventdevice.h>

namespace synthsoma2
{
  /*
    This is the multithreaded synthetic event server. 
    The interface to the world is the standard thread-safe 
    
    The idea here is that we run three-ish threads
   */ 
  class NetEventServer : public IEventDevice, 
			 boost::noncopyable
  {
  public:
    // factory methods
    pNetEventServer_t createINet(); 
    pNetEventServer_t createDomain(); 
    
    // run()
    void run(); 
    void shutdown(); 

    // 
    void ecycle(ecyclecnt_t); 
    eventtxlist_t getTXEvents(); 
    void sendEvents(const eventlist_t &); 
    // Assume each sendEvents invocation constitutes a single
    // ecycle's worth of events? 
    
    ~NetEventServer();
  private:
    NetEventServer(); 

  }; 
  
  typedef boost::shared_ptr<NetEventServer> pNetEventServer_t; 
  
}

#endif // __SYNTHSOMA2_NETEVENTSERVER_H__
