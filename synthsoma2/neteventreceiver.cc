#include <synthsoma2/neteventreceiver.h>
#include <somanetwork/ports.h> 
namespace synthsoma2
{
  
  pNetEventReceiver_t 
  NetEventReceiverFactory::createDomain(boost::filesystem::path basedir)
  {
    pio_service_t ios(new boost::asio::io_service);
    
    // construct the endpoint
    std::string sockname = (basedir / "eventtx").string(); 
    boost::asio::local::datagram_protocol::endpoint ep(sockname); 
    SS2L_(debug) << "NetEventReceiver: domain interface created " << sockname;  

    return pNetEventReceiver_t(new NetEventReceiver<boost::asio::local::datagram_protocol>(ios, ep));

    
      
  }

  pNetEventReceiver_t 
  NetEventReceiverFactory::createINet() 
  {
    pio_service_t ios(new boost::asio::io_service);
    
    // construct the endpoint
    boost::asio::ip::udp::endpoint ep(boost::asio::ip::address_v4(), sn::EVENTTXPORT); 
    SS2L_(debug) << "NetEventReceiver: ip interface created port=" 
		 << sn::EVENTTXPORT; 

    return pNetEventReceiver_t(new NetEventReceiver<boost::asio::ip::udp>(ios, ep));

    
      
  }

}
