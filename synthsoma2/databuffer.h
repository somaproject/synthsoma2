#ifndef __SYNTHSOMA2_DATABUFFER_H__
#define __SYNTHSOMA2_DATABUFFER_H__

#include <synthsoma2/types.h>

namespace synthsoma2
{

    
  class DataBuffer {
  public:
    static const size_t BUFSIZEMAX = 1300; 
    DataBuffer(); 
    sn::datasource_t src; 
    sn::datatype_t typ; 
    char * getPtr(); 
    char * getFrameStartPtr(); 
    void setFrameLen(size_t); 
    size_t getLen(); 

  private:

    boost::array<char, BUFSIZEMAX> data_; 
    size_t totallen; 
    
  }; 
  


}


#endif 

