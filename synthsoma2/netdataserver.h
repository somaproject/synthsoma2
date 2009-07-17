#ifndef __SYNTHSOMA2_NETDATASERVER_H__
#define __SYNTHSOMA2_NETDATASERVER_H__

#include <synthsoma2/types.h>
#include <synthsoma2/databuffer.h>

namespace synthsoma2 {

  class IDataSink
  {
  public:
    virtual void run() = 0; 
    virtual void shutdown() = 0; 

    virtual void sendData(DataBuffer *) = 0; 
    /* Send-data transfers ownership, potentially
       across a thread boundary. 

       sendData must be thread safe. 

       Note that this means that the sink is responsible
       for calling delete on the passed-in pointer
    */
    
       
  }; 
  
  typedef boost::shared_ptr<IDataSink> pDataSink_t ; 

  class NetDataServer : public IDataSink, 
		        boost::noncopyable
		       
  {
    /*
      Net Data server: Create a lot of sockets? Or just send
      to a lot of endpoints? 

    */ 
  public:
    static pNetDataServer_t createDomain(boost::filesystem::path root); 
    static pNetDataServer_t createINet(std::string destip); 
    
    NetDataServer(pNetDataSender_t & nds); 
    void sendData(DataBuffer *); 
    //void setReTx();  FIXME retx buffer

    void run(); 
    void shutdown(); 


  private:
    typedef std::list<DataBuffer *> dataqueue_t; 
    dataqueue_t dataqueue_; 

    pNetDataSender_t pNetDataSender_; 

  }; 
  
  typedef boost::shared_ptr<NetDataServer> pNetDataServer_t; 
}

#endif
