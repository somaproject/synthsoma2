#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include <somanetwork/network.h>
#include <synthsoma2/types.h>
#include "testutil.h"
#include <somanetwork/logging.h>
#include <somanetwork/tspike.h>
#include <somanetwork/datapacket.h>
#include <synthsoma2/neteventreceiver.h>

using namespace boost;       


BOOST_AUTO_TEST_SUITE(neteventreceiver)
using namespace synthsoma2; 

BOOST_AUTO_TEST_CASE(create)
{
  // Can we just instantiate
  boost::filesystem::path tgtdir = createTempDir(); 

  pNetEventReceiver_t recv = NetEventReceiverFactory::createDomain(tgtdir); 
  
  recv->run(); 

  // Dummy sockets
  createBoundDomainSocket(tgtdir / "dataretx"); 
  createBoundDomainSocket(tgtdir / "eventretx"); 

  // now we conjure up a libsomanetwork to send test packets
  sn::pNetworkInterface_t pn = sn::Network::createDomain(tgtdir); 
  
  pn->run(); 

  int N = 100; 
  for (int i = 0; i < N; i++) {
    sn::EventTXList_t etxl; 
    sn::EventTX_t canonical = createCanonicalEventTX(0, 1,  i); 
    etxl.push_back(canonical); 
    pn->sendEvents(etxl); 
  }
  sleep(1); 
  // now did we get the events;
  for (int i = 0; i < N; i++) {
    optEventTX_t oetx = recv->getInboundEvent(); 
    
    equal(oetx->event, createCanonicalEvent(0,1,  i)); 
    
  }

  recv->shutdown(); 
  pn->shutdown(); 


}


BOOST_AUTO_TEST_SUITE_END(); 

