#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <synthsoma2/datasequenceproto.h>
#include <synthsoma2/netdataserver.h>
using namespace boost;       


BOOST_AUTO_TEST_SUITE(datasequenceproto); 
using namespace synthsoma2; 
using namespace somanetwork; 


BOOST_AUTO_TEST_CASE(startup)
{
  DataSequenceProto dsp; 

  for (int i = 0; i < 10; i++) {
    for (int k = 0; k < 5; k++) {
      DataBuffer * db = new DataBuffer; 
      db->src = k;
      db->typ = TSPIKE; 
      
      sequence_t s = dsp.process(db); 
      
      sequence_t snet, shost; 
      memcpy(&snet, db->getPtr(), sizeof(snet)); 
      shost = htonl(snet); 
      BOOST_CHECK_EQUAL(shost, s) ; 
      BOOST_CHECK_EQUAL(shost, i) ; 
    }
  }
  
}




BOOST_AUTO_TEST_SUITE_END(); 
