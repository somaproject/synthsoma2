#ifndef __SYNTHSOMA2_DATADEVICE_H__
#define __SYNTHSOMA2_DATADEVICE_H__

#include <somanetwork/datapacket.h>
#include <synthsoma2/types.h>
#include <synthsoma2/databus.h>


namespace synthsoma2
{

  class IDataDevice {
    
    /*
      Note that in general a data device will also 
      be an IEventDevice, so that's where the Run() methods should
      come from
      
     */ 
  public:
    virtual void setDeviceDataSrc(sn::datasource_t id) = 0; 
    // thread-safe functions! 
    virtual void visitSubmitData(DataBus * db) = 0;  // visitor-like callback. 

  }; 





}


#endif // __SYNTHSOMA2_DATADEVICE_H__
