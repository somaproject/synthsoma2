#include <synthsoma2/datasequenceproto.h>
#include <synthsoma2/netdataserver.h>

namespace synthsoma2 
{

  DataSequenceProto::DataSequenceProto()
  {
    std::vector<uint32_t> vals; 

    for(int i = 0; i < 63; i++) {
      vals.push_back(0); 
    }
    
    sequencemap_[sn::TSPIKE] = vals; 
    sequencemap_[sn::WAVE] = vals; 
    sequencemap_[sn::RAW] = vals; 


  }


  sn::sequence_t DataSequenceProto::getNextSeqNum(sn::datasource_t src,
						  sn::datatype_t typ)
  {
    if(sequencemap_.find(typ) == sequencemap_.end())
      {
	throw std::runtime_error("unknown data type"); 
      }
    sn::sequence_t s = sequencemap_[typ][src]; 
    sequencemap_[typ][src] = s + 1; 

    return s; 
  }

  sn::sequence_t DataSequenceProto::process(DataBuffer * db)
  {
    
    // get the next sequence number
    sn::sequence_t news = getNextSeqNum(db->src, db->typ); 
    
    // now copy
    uint32_t nnews = htonl(news); 
    memcpy(db->getPtr(), &nnews, sizeof(nnews));

    return news; 

  }


}
