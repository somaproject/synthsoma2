#include <synthsoma2/netdataserver.h>
#include <synthsoma2/netdatasender.h>

namespace synthsoma2
{
  
  
  
  /* ------------------------------------------------------------------------
     NET DATA SERVER 
   ------------------------------------------------------------------------- */

  
  pNetDataServer_t NetDataServer::createDomain(boost::filesystem::path root)
  {
    pNetDataSender_t nds = NetDataSender::createDomain(root); 
    
    return pNetDataServer_t(new NetDataServer(nds)); 
    
  }

  pNetDataServer_t NetDataServer::createINet() 
  {
    // FIXME we want internet, right? 

  }
    


  
  NetDataServer::NetDataServer(pNetDataSender_t & nds):
    pNetDataSender_(nds)    
  {
    
    
  }

  void NetDataServer::sendData(DataBuffer * db)
  {
    pNetDataSender_->addDataBuffer(db); 
  }
  
//   void NetDataServer::dostuff()
//   {
//     // if packet in queue
    
//     // add the relevant sequence number
    
//     // send the actual buffer async

//     // when the async is done, delete the resulting packet
    
//   }

  void NetDataServer::run()
  {
    pNetDataSender_->run(); 
    
  }

  
  void NetDataServer::shutdown()
  {

    pNetDataSender_->shutdown(); 

  }

}
