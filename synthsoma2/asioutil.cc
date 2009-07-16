#include "asioutil.h"

namespace synthsoma2 
{
  namespace senderproxy {

    LocalDatagram::LocalDatagram(io_service & service, 
				 boost::filesystem::path sockname) :
      socket_(service), 
      endpoint_(sockname.string())
    {
      socket_.open();
    }
    
    LocalDatagram::~LocalDatagram()
    {

    }


    void LocalDatagram::send(const std::vector<char> & b)
    {
      socket_.send_to(buffer(&b[0], b.size()), endpoint_); 
    }


    INetDatagram::INetDatagram(io_service & service, 
			       std::string ip, uint16_t destport) : 
      socket_(service), 
      endpoint_(ip::address_v4::from_string(ip), destport)
    {
      /// set up the endpoint
      //socket_.open(); 
    }

    INetDatagram::~INetDatagram()
    {


    }

    void INetDatagram::send(const std::vector<char> & b)
    {
      socket_.send_to(buffer(&b[0], b.size()), endpoint_); 
    }


  }

}
