#ifndef __SYNTHSOMA2_NETEVENTSENDER_H__
#define __SYNTHSOMA2_NETEVENTSENDER_H__

#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


#include <somanetwork/tspipefifo.h>

#include <synthsoma2/types.h>
#include <synthsoma2/tsretxbuffer.h>
#include <synthsoma2/asioutil.h>

namespace synthsoma2
{
  

  typedef TSReTxBuffer<sn::eventseq_t, std::vector<char> > eventretxbuffer_t; 

  typedef boost::shared_ptr<NetEventSender> pNetEventSender_t; 

  class NetEventSender {
    /* 
       NetEventSender handles event transmission and 
       packet assembly.        
       
       it runs in its own thread, and its functions
       are thread safe. It calls methods onthe passed-in
       eventretxbuffer, but that should be thread safe anyway. 
  
    */
    
  public:
    // creation
    static pNetEventSender_t createDomain(boost::filesystem::path basedir); 
    static pNetEventSender_t createINet(std::string ip); 
    
    void setReTXBuffer(eventretxbuffer_t * b); 

    // running
    void run(); 
    void shutdown(); 

    // Data manipulation
    void addEvents(const eventlist_t & el); 
    
    ~NetEventSender(); 
    
  private:
    NetEventSender(pio_service_t service, 
		   senderproxy::pDatagram_t ); 

    void newEventsOnFifo(const boost::system::error_code & error, 
		 size_t bytes_received);
    void inpipeArm(); 
    void sendCurrentList(); 

    eventretxbuffer_t * pReTxBuffer_; 
    
    std::list<sn::EventList_t> pendingEventList_; 
    size_t eventCount_; 
    sn::TSPipeFifo<eventlist_t> fifo_; 

    pio_service_t  ioservice_; 
    senderproxy::pDatagram_t sp_; 
    boost::asio::posix::stream_descriptor inpipe_ ;

    enum { max_length = 1024 };
    char data_[max_length];
    
    sn::eventseq_t seq_; 
    boost::thread * workerthread_;  
  }; 
  
  
  
}

#endif // 
