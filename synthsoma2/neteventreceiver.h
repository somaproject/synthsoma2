#ifndef __SYNTHSOMA2_NETEVENTRECEIVER_H__
#define __SYNTHSOMA2_NETEVENTRECEIVER_H__

#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


#include <somanetwork/tspipefifo.h>
#include <somanetwork/eventtx.h>


#include <synthsoma2/types.h>

#include <synthsoma2/tsretxbuffer.h>

namespace synthsoma2
{
 
  class INetEventReceiver
  {
  public:
    virtual void run() = 0; 
    virtual void shutdown() = 0; 
    virtual optEventTX_t getInboundEvent() = 0; 

  }; 

  typedef boost::shared_ptr<INetEventReceiver> pNetEventReceiver_t; 

  template<class proto>
  class NetEventReceiver : boost::noncopyable, public INetEventReceiver
  {
    /* NetEventReceiver runs in an independent thread
       and takes in events and makes them available for per-ecycle
       polling. 
              
    */ 

  public:
    

    NetEventReceiver(pio_service_t ios, typename proto::endpoint ep) :
      ioservice_(ios), 
      socket_(*ios, ep), 
      data_(1550), 
      workerthread_(NULL)
    {
      
      // Things to do upon receiving a packet
      
      
      inPacketArm(); 

    }
    
    

    void run() {

      if (workerthread_ != 0) {
	throw std::runtime_error("Thread already running"); 
      }
      
      workerthread_ = new boost::thread(boost::bind(&boost::asio::io_service::run, ioservice_));
      
    }
    
    void shutdown() {

      ioservice_->stop();     
      
      if (workerthread_ != 0) {
	workerthread_->join();       
      }

    }

    optEventTX_t getInboundEvent() {
      optEventTX_t oe; 

      boost::mutex::scoped_lock lock(queuemutex_); 
      if(!outqueue_.empty()) {
	oe = outqueue_.front(); 
	outqueue_.pop_front(); 

      }
      return oe; 

    }

    void inPacketArm() {
      socket_.async_receive_from(boost::asio::buffer(data_), remote_endpoint_, 
				 boost::bind(&NetEventReceiver::newDatagram, 
					     this, boost::asio::placeholders::error, 
					     boost::asio::placeholders::bytes_transferred)); 
      
      
    }


    void newDatagram(const boost::system::error_code & error, 
		     size_t bytes_recvd)
    {
      sn::EventTXList_t etl; 
      sn::eventtxnonce_t nonce =  sn::getEventListFromBuffer(data_, &etl); 

      // put the data in the output buffer
      // now resend! 
      std::vector<char> outbuf(4); 
      sn::eventtxnonce_t nnonce = htons(nonce); 
      memcpy(&outbuf[0], &nnonce, sizeof(nnonce)); 
      outbuf[2] = 0; 
      outbuf[3] = 1;

      SS2L_(debug) << "NetEventReceiver: received nonce=" 
		  << nonce << " with " << etl.size() << "events" ; 

      socket_.send_to(boost::asio::buffer(outbuf, 4), 
		      remote_endpoint_); 
      
      // re-arm
      inPacketArm(); 
      
      boost::mutex::scoped_lock lock(queuemutex_); 
      copy(etl.begin(), etl.end(), std::back_insert_iterator<std::list<sn::EventTX_t> >(outqueue_)); 


    }
    // testing ? 
    
  private:
    pio_service_t ioservice_; 
    
    boost::asio::basic_datagram_socket<proto > socket_;
    typename proto::endpoint remote_endpoint_; 
    boost::thread * workerthread_;  
    boost::mutex queuemutex_; 
    std::list<sn::EventTX_t> outqueue_; 

    std::vector<char> data_; 
    

  };
  
  
  class NetEventReceiverFactory { 
  public:
    static pNetEventReceiver_t createDomain(boost::filesystem::path basedir);
    static pNetEventReceiver_t createINet();
  };
  


   

}

#endif
