#include <synthsoma2/databuffer.h>

namespace synthsoma2 {
  
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



}
