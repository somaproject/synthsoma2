#ifndef __SYNTHSOMA2_ASIOUTIL_H__
#define __SYNTHSOMA2_ASIOUTIL_H__

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <synthsoma2/types.h>
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
      virtual ~Datagram() {}; 
    }; 

    typedef boost::shared_ptr<Datagram> pDatagram_t;
    


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

    
    class DataDatagram
    {
    public:
      /* Ugh, a slight conflation of concepts here */ 
      virtual void send(DataBuffer * db) = 0; 
      virtual ~DataDatagram() {};

    }; 

    typedef boost::shared_ptr<DataDatagram> pDataDatagram_t; 



    class LocalDataDatagram : public DataDatagram
    {
    public:
      LocalDataDatagram(io_service & service, boost::filesystem::path sockdir);
      ~LocalDataDatagram(); 
      void send(DataBuffer * db); 
      
    private:
      typedef std::map<std::pair<sn::datasource_t, sn::datatype_t>, local::datagram_protocol::endpoint> endpointmap_t; 
      endpointmap_t endpointmap_; 
      local::datagram_protocol::socket socket_; 
    }; 
    
    
    class INetDataDatagram : public DataDatagram
    {
    public:
      INetDataDatagram(io_service & service, std::string ip);
      ~INetDataDatagram(); 
      void send(DataBuffer * db); 
      
    private:
      typedef std::map<std::pair<sn::datasource_t, sn::datatype_t>, ip::udp::endpoint> endpointmap_t; 
      endpointmap_t endpointmap_; 
      ip::udp::socket socket_; 
    }; 
    
    
  }

}

#endif
