#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <synthsoma2/eventrouter.h>

using namespace boost;       


BOOST_AUTO_TEST_SUITE(eventrouter); 
using namespace synthsoma2; 
using namespace somanetwork; 

void ecycle_callback(ecyclecnt_t * tgt, ecyclecnt_t newval)
{
  *tgt = newval ; 
}

BOOST_AUTO_TEST_CASE(startup)
{
  /*
    Test if we can compile 
    
  */
  synthsoma2::EventRouter er; 
  BOOST_CHECK_EQUAL(er.getECycleCnt(), 0); 
  
}

BOOST_AUTO_TEST_CASE(enable)
{
  /*
    Enable sinks
  */
  synthsoma2::EventRouter er; 
  
  er.enableDevice(0); 
  eventlist_t e = er.gatherEvents(0); 
  BOOST_CHECK_EQUAL(e.size(), 0); 

  // now break things
  BOOST_CHECK_THROW(er.enableDevice(MAXEVENT), std::runtime_error); 
  
  BOOST_CHECK_THROW(er.gatherEvents(MAXEVENT), std::runtime_error); 

}

BOOST_AUTO_TEST_CASE(signal)
{
  /* 
     Do we actually signal? 

   */
  
  ecyclecnt_t cnt = 1000; 
  synthsoma2::EventRouter er; 
  er.eventCycleSignal().connect(boost::bind(&ecycle_callback, &cnt, _1)); 
  
  er.eventCycle(); 
  ecyclecnt_t c1 = cnt; 
  er.eventCycle(); 
  er.eventCycle(); 
  ecyclecnt_t c3 = cnt; 
  BOOST_CHECK_EQUAL(c3, c1+2); 

}

BOOST_AUTO_TEST_CASE(onetoone)
{
  /*
    A single source and a single receiver
 */ 
  
  synthsoma2::EventRouter er; 
  const eventsource_t edest = 5; 
  const eventsource_t edest2 = 50; 
  
  er.enableDevice(edest); 
  er.enableDevice(edest2); 
  for (int i = 0; i < 1000; i++) {
    EventTX_t t1; 
    t1.destaddr[edest] = true; 
    if (i % 5 == 0) {
      t1.destaddr[edest2] = true; 
    }
    t1.event.data[0] = 0x1234; 
    t1.event.data[1] = i; 

    er.eventCycle(); 
    
    er.addEvent(0, t1); 
    
    er.processEventCycle(); 
    eventlist_t el = er.gatherEvents(edest); 
    BOOST_CHECK_EQUAL(el.size(), 1); 
    if (el.size() > 1) {
      BOOST_CHECK_EQUAL(el.front().data[0], 0x1234); 
      BOOST_CHECK_EQUAL(el.front().data[1], i);
      
    }
    
    eventlist_t el2 = er.gatherEvents(edest2); 
    if (i % 5 == 0) {
      BOOST_CHECK_EQUAL(el2.size(), 1); 
      if (el2.size() > 1) {
	BOOST_CHECK_EQUAL(el2.front().data[0], 0x1234); 
	BOOST_CHECK_EQUAL(el2.front().data[1], i);
	
      }
    }
    
  }


}


BOOST_AUTO_TEST_SUITE_END(); 
