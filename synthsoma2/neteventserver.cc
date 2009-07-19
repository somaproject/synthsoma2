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

  pNetEventServer_t NetEventServer::createINet(std::string IP)
  {
    pNetEventServer_t es(new NetEventServer(NetEventSender::createINet(IP))); 
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

  const optEventTX_t NetEventServer::getTXEvent()
  {
    optEventTX_t oetx;  // FIXME 
    return oetx; 
  }
  
  void NetEventServer::sendEvent(const sn::Event_t & e)
  {
    // pNetEventSender_->addEvents(el); // FIXME do something
    eventlist_t el; 
    el.push_back(e); 
    pNetEventSender_->addEvents(el); 

  }

  void NetEventServer::setDeviceID(sn::eventsource_t id)
  {
    // FIXME ? I don't think we really do anything here. 
  }

}
