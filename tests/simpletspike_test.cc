#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <synthsoma2/devices/simpletspike.h>
#include <synthsoma2/databus.h>

using namespace boost;       


BOOST_AUTO_TEST_SUITE(simpletspike); 

using namespace synthsoma2; 
using namespace somanetwork; 


class DummyDataBus: public IDataBus
{
public:
  typedef std::list<std::pair<sn::datasource_t, sn::TSpike_t> > tspikelist_t; 

  DummyDataBus()  {

  } 

  ~DummyDataBus()  {

  } 

  tspikelist_t tspikelist_; 

  void newData(sn::datasource_t src, const sn::TSpike_t & ts)
  {
    
    tspikelist_.push_back(std::make_pair(src, ts)); 
  }

  void newData(sn::datasource_t src, const sn::Wave_t &)
  {


  }

  void newData(sn::datasource_t src, const sn::Raw_t &)
  {


  }

}; 



BOOST_AUTO_TEST_CASE(empty)
{
  
  DummyDataBus ddb; 
  
  SimpleTSpike sts; 
  sts.run();
  
  int N = 10000000; 
  for (int i = 0; i < N; i++) 
    {
      
      sts.settime(i); 
      sts.visitSubmitData(&ddb) ; 
    }
  BOOST_CHECK_EQUAL(ddb.tspikelist_.size(), 0); 
}


BOOST_AUTO_TEST_CASE(single_spike)
{
  using namespace synthsoma2; 
  using namespace somanetwork; 

  TSpike_t ts; 
  ts.src = 0; 
  ts.time = 9; 
  
  SimpleTSpike sts; 
  sts.addTSpike(ts); 
  DummyDataBus ddb; 

  sts.run();

  int N = 100; 
  for (int i = 0; i < N; i++) 
    {
      
      sts.settime(i); 
      sts.visitSubmitData(&ddb) ; 
    }
  
  BOOST_CHECK_EQUAL(ddb.tspikelist_.size(), N / 10); 


}



BOOST_AUTO_TEST_SUITE_END(); 
