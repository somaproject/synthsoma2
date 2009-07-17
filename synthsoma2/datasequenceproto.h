#ifndef __SYNTHSOMA2_DATASEQUENCEPROTO_H__
#define __SYNTHSOMA2_DATASEQUENCEPROTO_H__

#include <synthsoma2/types.h>
#include <map>

namespace synthsoma2
{
  
  class DataSequenceProto
  {
    /* 
       The last step of data packet manipulation, 
       where we add the sequence id
     */ 
    
  public:
    DataSequenceProto(); 
    sn::sequence_t process(DataBuffer * db); 
    
  private:
    typedef std::map<sn::datatype_t, std::vector<sn::sequence_t> > seqmap_t; 
    seqmap_t sequencemap_; 

    sn::sequence_t getNextSeqNum(sn::datasource_t, sn::datatype_t); 
    
    
  };


}

#endif // __SYNTHSOMA2_DATASEQUENCEPROTO_H__
