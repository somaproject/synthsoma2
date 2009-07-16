#ifndef __SYNTHSOMA2_TYPES_H__
#define __SYNTHSOMA2_TYPES_H__

#include <somanetwork/event.h>
#include <somanetwork/eventtx.h>
#include <boost/utility.hpp> 
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include <synthsoma2/sslogging.h>

namespace synthsoma2
{
  namespace sn = somanetwork;   
  
  const int MAXEVENT = 80; 
  
  typedef uint64_t ecyclecnt_t; 
  typedef std::list<sn::EventTX_t> eventtxlist_t; 
  typedef sn::EventList_t eventlist_t; 

  class NetEventSender; 

  typedef boost::shared_ptr<boost::asio::io_service> pio_service_t; 


  class NetEventServer;
  typedef boost::shared_ptr<NetEventServer> pNetEventServer_t; 

  class EventBus; 
  typedef boost::shared_ptr<EventBus> pEventBus_t; 
  
  class DataBus;
  typedef boost::shared_ptr<DataBus> pDataBus_t; 

}

#endif // 
