#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <synthsoma2/tsretxbuffer.h>

using namespace boost;       


BOOST_AUTO_TEST_SUITE(eventrouter); 
using namespace synthsoma2; 
using namespace somanetwork; 

BOOST_AUTO_TEST_CASE(startup)
{
  int N = 100; 
  
  TSReTxBuffer<uint16_t, int> buffer(N, 0); 
  
  for (int i = 0; i < 10; i++) {
    buffer.save(i, 10 + i); 
  }
  
  for (int i = 0; i < 10; i++) {
    BOOST_CHECK_EQUAL(buffer.lookup(i), 10 + i); 
  }
  
  buffer.save(101, 1000); 
  BOOST_CHECK_EQUAL(buffer.lookup(101), 1000); 
  BOOST_CHECK_EQUAL(buffer.lookup(1), 1000); 
  
}


BOOST_AUTO_TEST_SUITE_END(); 
