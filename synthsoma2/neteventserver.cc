#include "neteventserver.h"


namespace synthsoma2
{
  
  NetEventServer::NetEventServer(pNetEventSender_t es) :
    pNetEventSender_(es)
  {
    

  }


  NetEventServer::~NetEventServer()
  {
    

  }

  pNetEventServer_t NetEventServer::createDomain(boost::filesystem::path root)
  {
    pNetEventServer_t es(new NetEventServer(NetEventSender::createDomain(root))); 
    return es; 
  }

  void NetEventServer::run()
  {
    pNetEventSender_->run(); 
    
  }
  
  void NetEventServer::shutdown()
  {
    pNetEventSender_->shutdown(); 
    
  }
  
  void NetEventServer::ecycle(ecyclecnt_t cnt)
  {


  }

  eventtxlist_t NetEventServer::getTXEvents() {
    
    eventtxlist_t t; // FIXME empty
    return t; 
    
  }
  
  void NetEventServer::sendEvents(const eventlist_t & el)
  {
    pNetEventSender_->addEvents(el); 

  }

  void NetEventServer::setDeviceID(sn::eventsource_t id)
  {
    // FIXME ? I don't think we really do anything here. 
  }

}
