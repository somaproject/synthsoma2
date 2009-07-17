#include <synthsoma2/netdataserver.h>

namespace synthsoma2
{
  
  DataBuffer::DataBuffer() :
    src(0), 
    typ(sn::TSPIKE), 
    totallen(0)
  {
    
  }

  char * DataBuffer::getFrameStartPtr()
  {
    return &data_[0] + sizeof(sn::sequence_t); 
  }

  char * DataBuffer::getPtr()
  {
    return &(data_[0]); 
  }

  void DataBuffer::setFrameLen(size_t len)
  {
    totallen = len + sizeof(sn::sequence_t); 
    
  }

  size_t DataBuffer::getLen()
  {
    return totallen; 
  }
  
  
  /* ------------------------------------------------------------------------
     NET DATA SERVER 
   ------------------------------------------------------------------------- */
  NetDataServer::NetDataServer()
  {
    
    
  }

  void NetDataServer::sendData(DataBuffer * db)
  {
    boost::mutex::scoped_lock lock(dataqueuemutex_); 
    dataqueue_.push_back(db); 
  }
  
//   void NetDataServer::dostuff()
//   {
//     // if packet in queue
    
//     // add the relevant sequence number
    
//     // send the actual buffer async

//     // when the async is done, delete the resulting packet
    
//   }


}
