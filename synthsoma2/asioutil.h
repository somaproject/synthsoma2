#ifndef __SYNTHSOMA2_ASIOUTIL_H__
#define __SYNTHSOMA2_ASIOUTIL_H__

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

/*
  Wrappers so we can write AF_INET/AF_LOCAL independent code


 */

namespace synthsoma2 { 

  namespace senderproxy { 
    using namespace boost::asio; 
    
    class Datagram
    {
    public:
      virtual void send(const std::vector<char> & ) = 0; 
      virtual ~Datagram() = 0; 
    }; 
    
    class LocalDatagram : public Datagram
    {
    public:
      LocalDatagram(io_service & service, 
		    boost::filesystem::path sockname); 
      ~LocalDatagram(); 
      
      void send(const std::vector<char> & buffers); 
      
    private:
      local::datagram_protocol::socket socket_; 
      local::datagram_protocol::endpoint endpoint_; 
    };

    class INetDatagram : public Datagram
    {
    public:
      INetDatagram(io_service & service, 
		   std::string ip, uint16_t destport); 
      ~INetDatagram(); 

      void send(const std::vector<char> & ); 
      
    private:
      ip::udp::socket socket_; 
      ip::udp::endpoint endpoint_; 
    };


  }
  
}

#endif
