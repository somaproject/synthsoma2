#include "asioutil.h"
#include <boost/format.hpp>
#include <synthsoma2/netdataserver.h>

namespace synthsoma2 
{
  namespace senderproxy {

    LocalDatagram::LocalDatagram(io_service & service, 
				 boost::filesystem::path sockname) :
      socket_(service), 
      endpoint_(sockname.string())
    {

      socket_.open();

      boost::asio::socket_base::non_blocking_io command(true);
      socket_.io_control(command);

    }
    
    LocalDatagram::~LocalDatagram()
    {

    }


    void LocalDatagram::send(const std::vector<char> & b)
    {
      try { 
	socket_.send_to(buffer(&b[0], b.size()), endpoint_); 
      } catch (std::exception & e) {
	// Really? we have to do this to avoid WAS_WOULDBLOCK? 
	// IRC Confirms
      }
    }


    INetDatagram::INetDatagram(io_service & service, 
			       std::string ip, uint16_t destport) : 
      socket_(service, ip::udp::endpoint(ip::udp::v4(), 0)), 
      endpoint_(ip::address_v4::from_string(ip), destport)
    {


      // set as broadcast 
      boost::asio::socket_base::broadcast option(true);
      socket_.set_option(option);
    }

    INetDatagram::~INetDatagram()
    {


    }

    void INetDatagram::send(const std::vector<char> & b)
    {
      socket_.send_to(buffer(&b[0], b.size()), endpoint_); 
    }


    /*-------------------------------------------------------------------
      Data Datagram Proxies
      ------------------------------------------------------------------*/

    
    LocalDataDatagram::LocalDataDatagram(io_service & service, 
					 boost::filesystem::path sockdir) :
      socket_(service)
    {
      /* 
	 Create all possible endpoints

       */ 
      boost::filesystem::path database = sockdir / "data";
      for(sn::datasource_t src = 0; src < 64; src++) {
	std::string tspike = (database / "tspike" / 
			      boost::str(boost::format("%d") % (int)src)).string(); 
	
	endpointmap_[std::make_pair(src, sn::TSPIKE)] 
	  = local::datagram_protocol::endpoint(tspike); 
	
	std::string raw = (database / "raw" / 
			   boost::str(boost::format("%d") % (int)src)).string(); 
	
	endpointmap_[std::make_pair(src, sn::RAW)] 
	  = local::datagram_protocol::endpoint(raw); 
	
	std::string wave = (database / "wave" / 
			    boost::str(boost::format("%d") % (int)src)).string(); 
	
	endpointmap_[std::make_pair(src, sn::WAVE)] 
	  = local::datagram_protocol::endpoint(wave); 
	
      }


      socket_.open();

      boost::asio::socket_base::non_blocking_io command(true);
      socket_.io_control(command);
 
     
    }
    
    LocalDataDatagram::~LocalDataDatagram()
    {

    }
    
    void LocalDataDatagram::send(DataBuffer * db) {

      endpointmap_t::iterator iep = endpointmap_.find(std::make_pair(db->src, db->typ)); 
      
      if (iep != endpointmap_.end()) {

 	try { 
	  socket_.send_to(buffer(db->getPtr(), db->getLen()), 
			   iep->second); 
	} catch (std::exception & e) {
	  // Really? we have to do this to avoid WAS_WOULDBLOCK? 
	  // but don't we want this thread to be able to block if it has to? 
	  // FIXME
	  // IRC Confirms
	}
	
      }
    }
    
  }

}
