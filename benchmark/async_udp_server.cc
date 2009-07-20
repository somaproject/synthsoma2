
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

template <class proto>
class MyServer 
{
public:
  MyServer(boost::asio::io_service & ios, typename proto::endpoint ep) : 
    socket_(ios, ep) 
  { 
    

  }

  boost::asio::basic_datagram_socket<proto > socket_;

}; 

int main(int argc, char* argv[])
{
  boost::asio::io_service service; 
  boost::asio::ip::udp::endpoint ep; 
  
  MyServer<boost::asio::ip::udp> server(service, ep); 
  
}
