#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <vector>

#include "neteventsender.h"
#include <somanetwork/ports.h>
#include "sslogging.h"
namespace synthsoma2
{
  using namespace boost::asio; 

  pNetEventSender_t NetEventSender::createDomain(boost::filesystem::path basedir)
  {
    pio_service_t ios(new io_service());
    senderproxy::pDatagram_t sp(new senderproxy::LocalDatagram(*ios, basedir / "eventrx")); 

    return pNetEventSender_t(new NetEventSender(ios, sp)); 
    
  }

  pNetEventSender_t NetEventSender::createINet()
  {
    pio_service_t ios(new boost::asio::io_service());
    std::string ip("127.0.0.1"); 
    senderproxy::pDatagram_t  sp(new senderproxy::INetDatagram(*ios, ip, sn::EVENTRXPORT)); 
    
    return pNetEventSender_t(new NetEventSender(ios, sp)); 
    
  }


  NetEventSender::NetEventSender(pio_service_t service, 
				 senderproxy::pDatagram_t  sp) : 
    pReTxBuffer_(NULL), 
    eventCount_(0), 
    ioservice_(service),
    sp_(sp), 
    inpipe_(*service, fifo_.readingPipe), 
    seq_(0), 
    workerthread_(0)
  {
    inpipeArm(); 

  }

  NetEventSender::~NetEventSender()
  {

  }


  void NetEventSender::run()
  {
    if (workerthread_ != 0) {
      throw std::runtime_error("Thread already running"); 
    }
    workerthread_ = new boost::thread(boost::bind(&boost::asio::io_service::run, ioservice_));
  }

  void NetEventSender::setReTXBuffer(eventretxbuffer_t * b) 
  {
    pReTxBuffer_ = b; 
  }

  void NetEventSender::shutdown()
  {

    ioservice_->stop();     

    if (workerthread_ != 0) {
      workerthread_->join();       
    }

  }


  void NetEventSender::inpipeArm()
  {
    inpipe_.async_read_some(boost::asio::buffer(data_, max_length), 
			    boost::bind(&NetEventSender::newEventsOnFifo, this, 
					boost::asio::placeholders::error, 
					boost::asio::placeholders::bytes_transferred)); 
    

  }

  void NetEventSender::addEvents(const eventlist_t & el)
  {
    fifo_.append(el); 
  }

  void NetEventSender::newEventsOnFifo(const boost::system::error_code & error, 
				       size_t bytes_recvd)
  {
    if (!error && bytes_recvd > 0)
      {
	for (int i = 0; i < bytes_recvd; i++) {
	  eventlist_t el = fifo_.pop(); 
	  if (el.size() + eventCount_ > 80) {
	    sendCurrentList(); 
	  }; 
	  
	  pendingEventList_.push_back(el); 
	  eventCount_ += el.size(); 
	  if (pendingEventList_.size() > 9) {
	    sendCurrentList(); 
	  }
	}
	
      } else {
      // FIXME log 
      
    }
    
    inpipeArm(); 
  }
  
  void NetEventSender::sendCurrentList()
  {
    /* 
       Transmit the current event list
    */ 
    std::vector<char> buf = sn::createEventBuffer(seq_, pendingEventList_); 
    
    // now send the actual buffer? 
    sp_->send(buf); 
    // now commit it to the retx buffer; 
    if(pReTxBuffer_) {
      pReTxBuffer_->save(seq_, buf); 
    }

    
    // cleanup
    seq_++; 
    pendingEventList_.clear(); 
    eventCount_ = 0; 
    
  }
}

