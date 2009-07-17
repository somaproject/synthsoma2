#include <synthsoma2/netdatasender.h>
#include <synthsoma2/netdataserver.h>

namespace synthsoma2
{
  pNetDataSender_t NetDataSender::createDomain(boost::filesystem::path root)
  {
    pio_service_t ios(new boost::asio::io_service);

    senderproxy::pDataDatagram_t dg(new senderproxy::LocalDataDatagram(*ios, root)); 
    return pNetDataSender_t(new NetDataSender(ios, dg)); 
  }

  
  pNetDataSender_t NetDataSender::createINet()
  {


  }


  NetDataSender::NetDataSender(pio_service_t ios, senderproxy::pDataDatagram_t  dg) :
    ioservice_(ios), 
    pSenderProxy_(dg), 
    inpipe_(*ios, fifo_.readingPipe), 
    workerthread_(NULL)
  {
    inpipeArm(); 
    
  }

  NetDataSender::~NetDataSender()
  {

  }
  
  void NetDataSender::run()
  {
    if (workerthread_ != 0) {
      throw std::runtime_error("Thread already running"); 
    }
    workerthread_ = new boost::thread(boost::bind(&boost::asio::io_service::run, ioservice_));
    
  }
  
  void NetDataSender::shutdown()
  {
    ioservice_->stop();     

    if (workerthread_ != 0) {
      workerthread_->join();       
    }

  }

  void NetDataSender::addDataBuffer(DataBuffer * d)
  {
    fifo_.append(d); 

  }


  void NetDataSender::newDataOnFifo(const boost::system::error_code & error, 
				      size_t bytes_recvd)
  {
    // do the real work
       if (!error && bytes_recvd > 0)
      {
	for (int i = 0; i < bytes_recvd; i++) {
	  DataBuffer * db = fifo_.pop(); 
	  sn::sequence_t seq = seqproto_.process(db); 
	  pSenderProxy_->send(db); 
	  delete db; 
	}
	
      } else {
      // FIXME log 
      
    }
    
    inpipeArm(); 
  }

  void NetDataSender::inpipeArm()
  {
    inpipe_.async_read_some(boost::asio::buffer(data_, max_length), 
			    boost::bind(&NetDataSender::newDataOnFifo, this, 
					boost::asio::placeholders::error, 
					boost::asio::placeholders::bytes_transferred)); 
    



  }



}
