#ifndef __SYNTHSOMA2_NETDATASENDER_H__
#define __SYNTHSOMA2_NETDATASENDER_H__

#include <synthsoma2/types.h>
#include <synthsoma2/datasequenceproto.h>
#include <somanetwork/tspipefifo.h>
#include <synthsoma2/asioutil.h>

namespace synthsoma2
{
  class NetDataSender
  {
    /* Class actually responsible for taking in DataBuffer *
       packets and pushing them out the network interface. 
       
    */ 
    
    
    // ignore retransmission for the time being FIXME

  public:
    static pNetDataSender_t createDomain(boost::filesystem::path root); 
    static pNetDataSender_t createINet(std::string destip); 
    
    void run(); 
    void shutdown(); 
    
    void addDataBuffer(DataBuffer *); 

    ~NetDataSender(); 

  private:
    NetDataSender(pio_service_t,  senderproxy::pDataDatagram_t); 



    void newDataOnFifo(const boost::system::error_code & error, 
		       size_t bytes_received);
    void inpipeArm(); 

    pio_service_t  ioservice_; 

    DataSequenceProto seqproto_; 

    senderproxy::pDataDatagram_t pSenderProxy_; 
    sn::TSPipeFifo<DataBuffer*> fifo_; 
    boost::asio::posix::stream_descriptor inpipe_; 
    
    enum { max_length = 1024 };
    char data_[max_length];
    
    boost::thread * workerthread_;  

  };


}


#endif
