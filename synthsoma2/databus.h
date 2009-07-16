#ifndef __SYNTHSOMA2_DATABUS_H__
#define __SYNTHSOMA2_DATABUS_H__

#include <synthsoma2/types.h>

namespace synthsoma2
{
  class DataBus
  {
  public:
    DataBus(); 
    void run(); 
    void shutdown(); 

  }; 

  typedef boost::shared_ptr<DataBus> pDataBus_t; 


}
#endif
