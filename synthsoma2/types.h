#ifndef __SYNTHSOMA2_TYPES_H__
#define __SYNTHSOMA2_TYPES_H__

#include <somanetwork/event.h>
#include <somanetwork/eventtx.h>
#include <somanetwork/datapacket.h>
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

  typedef boost::optional<sn::EventTX_t> optEventTX_t; 

  typedef uint64_t somatime_t; 
  
  class NetEventSender; 

  typedef boost::shared_ptr<boost::asio::io_service> pio_service_t; 


  class NetEventServer;
  typedef boost::shared_ptr<NetEventServer> pNetEventServer_t; 

  class EventBus; 
  typedef boost::shared_ptr<EventBus> pEventBus_t; 
  
  class DataBus;
  typedef boost::shared_ptr<DataBus> pDataBus_t; 

  class NetDataServer;
  typedef boost::shared_ptr<NetDataServer> pNetDataServer_t; 

  class IDataDevice; 
  typedef boost::shared_ptr<IDataDevice>  pDataDevice_t; 

  class DataBuffer; 

  class NetDataSender;
  typedef boost::shared_ptr<NetDataSender> pNetDataSender_t; 

  class INetEventReceiver; 
  typedef boost::shared_ptr<INetEventReceiver> pNetEventReceiver_t; 
  
  template<typename T>
  class NetEventReceiver; 

}

#endif // 
