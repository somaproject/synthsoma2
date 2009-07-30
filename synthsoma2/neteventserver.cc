#include "neteventserver.h"


namespace synthsoma2
{
  
  NetEventServer::NetEventServer(pNetEventSender_t esend, 
				 pNetEventReceiver_t erecv) :
    pNetEventSender_(esend),
    pNetEventReceiver_(erecv)
  {
    

  }


  NetEventServer::~NetEventServer()
  {
    

  }

  pNetEventServer_t NetEventServer::createDomain(boost::filesystem::path root)
  {
    
    pNetEventServer_t es(new NetEventServer(NetEventSender::createDomain(root), 
					    NetEventReceiverFactory::createDomain(root))); 
    
    return es; 
  }

  pNetEventServer_t NetEventServer::createINet(std::string IP)
  {
    pNetEventServer_t es(new NetEventServer(NetEventSender::createINet(IP), 
					    NetEventReceiverFactory::createINet())); 
    return es; 
  }
  
  void NetEventServer::run()
  {
    pNetEventSender_->run(); 
    pNetEventReceiver_->run(); 
    
  }
  
  void NetEventServer::shutdown()
  {
    pNetEventSender_->shutdown(); 
    pNetEventReceiver_->shutdown(); 
    
  }
  
  void NetEventServer::ecycle(ecyclecnt_t cnt)
  {


  }

  const optEventTX_t NetEventServer::getTXEvent()
  {
    optEventTX_t oetx = pNetEventReceiver_->getInboundEvent(); 

    return oetx; 
  }
  
  void NetEventServer::sendEvent(const sn::Event_t & e)
  {
    eventlist_t el; 
    el.push_back(e); 
    pNetEventSender_->addEvents(el); 

  }

  void NetEventServer::setDeviceID(sn::eventsource_t id)
  {
    // FIXME ? I don't think we really do anything here. 
  }

}
