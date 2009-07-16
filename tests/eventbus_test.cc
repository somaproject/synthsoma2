#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <synthsoma2/eventbus.h>
#include <synthsoma2/devices/timer.h>
#include <synthsoma2/devices/testdevice.h>
#include "testutil.h"
using namespace boost;       


BOOST_AUTO_TEST_SUITE(eventbus); 
using namespace synthsoma2; 
using namespace somanetwork; 


BOOST_AUTO_TEST_CASE(startup)
{
  /*
    Create the single debug device, have it send data to 
    a deviceid with no connected device, and then one to itself

   */
  EventBus::devicemap_t dm; 
  pTestDevice_t td(new TestDevice); 
  const int MYID = 3; 
  dm[MYID] = td; 
  
  EventBus eb(dm); 
  eb.run(); 
  
  for(int i = 0; i < 100; i++) {
    eb.eventcycle();     
  }

  td->debugSendTXEvents(setdest(createCanonicalEventTX(MYID, 0x20, 0), 10));
  
  for(int i = 0; i < 100; i++) {
    eb.eventcycle();     
    BOOST_CHECK_EQUAL(td->debugGetReceivedEvents().size(), 0); 
  }

  // Now try and send an event that we will receive
  td->debugSendTXEvents(setdest(createCanonicalEventTX(MYID, 0x20, 0), MYID));

  eventlist_t el; 
  
  for(int i = 0; i < 100; i++) {
    eb.eventcycle();     
    el = td->debugGetReceivedEvents(); 
    if (!el.empty()) 
      break; 
  }
  
  BOOST_CHECK_EQUAL(el.size(), 1); 
  BOOST_ASSERT(equal(el.front(), createCanonicalEvent(MYID, 0x20, 0))); 
  
  
  eb.shutdown(); 
  
}

BOOST_AUTO_TEST_CASE(multiple)
{
  
  /*
    Send events from one device to another
   */
  EventBus::devicemap_t dm; 
  pTestDevice_t td1(new TestDevice); 
  pTestDevice_t td2(new TestDevice); 
  const int ID1 = 3; 
  const int ID2 = 10; 
  dm[ID1] = td1; 
  dm[ID2] = td2; 
  
  EventBus eb(dm); 
  eb.run(); 
  
  for(int i = 0; i < 100; i++) {
    eb.eventcycle();     
  }

  // send the events and make sure we get them on the next cycle

  
  for(int i = 0; i < 100; i++) {
    td1->debugSendTXEvents(setdest(createCanonicalEventTX(ID1, 0x20, i), ID2));
    td2->debugSendTXEvents(setdest(createCanonicalEventTX(ID2, 0x21, i), ID1));
    eb.eventcycle();     


    eventlist_t el1 = td1->debugGetReceivedEvents(); 
    BOOST_CHECK_EQUAL(el1.size(), 1); 
    BOOST_ASSERT(equal(el1.front(), createCanonicalEvent(ID2, 0x21, i))); 
    
    eventlist_t el2 = td2->debugGetReceivedEvents(); 
    BOOST_CHECK_EQUAL(el2.size(), 1); 
    BOOST_ASSERT(equal(el2.front(), createCanonicalEvent(ID1, 0x20, i))); 
  }
  
  eb.shutdown(); 

}


BOOST_AUTO_TEST_SUITE_END(); 
