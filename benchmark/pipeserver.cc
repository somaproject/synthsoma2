#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <somanetwork/tspipefifo.h>

using namespace boost::asio;

typedef somanetwork::TSPipeFifo<int> fifo_t; 


class server
{
public:

  void status(const boost::system::error_code& e)
  {
    std::cout << "Appending to packet" << std::endl; 
    fifo_.append(pktcount_); 
    timer_.expires_from_now( boost::posix_time::seconds(2)); 
    timer_.async_wait(boost::bind(&server::status, this,  boost::asio::placeholders::error)); 
    
  }

  server(boost::asio::io_service& io_service, fifo_t & fifo)
    : io_service_(io_service),
      inpipe_(io_service, fifo.readingPipe), 
      pktcount_(0), 
      timer_(io_service, boost::posix_time::seconds(2)), 
      fifo_(fifo)
  {
    inpipe_.async_read_some(boost::asio::buffer(data_, max_length), 
			    boost::bind(&server::handle_receive_from, this,
					boost::asio::placeholders::error, 
					boost::asio::placeholders::bytes_transferred));
    timer_.async_wait(boost::bind(&server::status, this,  boost::asio::placeholders::error)); 
    
  }

  void handle_receive_from(const boost::system::error_code& error,
      size_t bytes_recvd)
  {
    if (!error && bytes_recvd > 0)
    {
      pktcount_++; 
      for (int i = 0; i < bytes_recvd; i++) {
	std::cout << "Got some !" <<       fifo_.pop() << std::endl; 
      }
      inpipe_.async_read_some(
			      boost::asio::buffer(data_, max_length), 
			      boost::bind(&server::handle_receive_from, this,
					  boost::asio::placeholders::error,
					  boost::asio::placeholders::bytes_transferred));
    }
    else
      {
      }
  }
  
private:
  boost::asio::io_service& io_service_;
  posix::stream_descriptor inpipe_;
  enum { max_length = 1024 };
  char data_[max_length];
  int pktcount_; 
  boost::asio::deadline_timer timer_; 
  fifo_t & fifo_; 
};

int main(int argc, char* argv[])
{
  fifo_t fifo; 

  boost::asio::io_service io_service;
  
  using namespace std; // For atoi.
  server s(io_service, fifo);

  io_service.run();

  return 0;
}
