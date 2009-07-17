#ifndef __SYNTHSOMA2_NETDATASERVER_H__
#define __SYNTHSOMA2_NETDATASERVER_H__

#include <synthsoma2/types.h>

namespace synthsoma2 {
  
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
  
  class IDataSink
  {
  public:

    virtual void sendData(DataBuffer *) = 0; 
    /* Send-data transfers ownership, potentially
       across a thread boundary. 

       sendData must be thread safe. 

       Note that this means that the sink is responsible
       for calling delete on the passed-in pointer
    */
    
       
  }; 
  
  class NetDataServer : public IDataSink, 
		        boost::noncopyable
		       
  {
    /*
      Net Data server: Create a lot of sockets? Or just send
      to a lot of endpoints? 

    */ 
  public:
    pNetDataServer_t createDomain(boost::filesystem::path root); 
    pNetDataServer_t createINet(); 
    
    NetDataServer(); 
    void sendData(DataBuffer *); 
    //void setReTx();  FIXME retx buffer

  private:
    typedef std::list<DataBuffer *> dataqueue_t; 
    dataqueue_t dataqueue_; 
    boost::mutex dataqueuemutex_; 

  }; 
    
}

#endif
