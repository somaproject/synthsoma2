#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include <synthsoma2/neteventsender.h>
#include <somanetwork/network.h>
#include <synthsoma2/types.h>
#include "testutil.h"
#include <somanetwork/logging.h>


using namespace boost;       


BOOST_AUTO_TEST_SUITE(neteventsender)
using namespace synthsoma2; 

BOOST_AUTO_TEST_CASE(create)
{
  // Can we just instantiate
  boost::filesystem::path tgtdir = boost::filesystem::path("/tmp/test1assa"); 
  boost::filesystem::remove_all(tgtdir); 
  boost::filesystem::create_directory(tgtdir); 

  pNetEventSender_t nes = NetEventSender::createDomain(tgtdir); 

}


BOOST_AUTO_TEST_CASE(startup)
{
  // Can we just start and end
  boost::filesystem::path tgtdir = boost::filesystem::path("/tmp/test1assa"); 
  boost::filesystem::remove_all(tgtdir); 
  boost::filesystem::create_directory(tgtdir); 

  pNetEventSender_t nes = NetEventSender::createDomain(tgtdir); 
  nes->run(); 
  nes->shutdown(); 
  

}

BOOST_AUTO_TEST_CASE(startup_send_few_events)
{
  // Can we just start and end
  boost::filesystem::path tgtdir = createTempDir(); 
  boost::filesystem::remove_all(tgtdir); 
  boost::filesystem::create_directory(tgtdir); 

    somanetwork::init_logs(debug); 

  // create other sockets
  createBoundDomainSocket(tgtdir / "dataretx"); 
  createBoundDomainSocket(tgtdir / "eventretx"); 
  createBoundDomainSocket(tgtdir / "eventtx"); 

  pNetEventSender_t nes = NetEventSender::createDomain(tgtdir); 
  nes->run(); 
  
  sn::pNetworkInterface_t pn = sn::Network::createDomain(tgtdir); 
  
  pn->run(); 

  // now send fake events
  for (int i = 0; i < 11; i++) {
    eventlist_t el; 
    sn::Event_t e; 
    e.cmd = 10; 
    e.src = 20; 
    e.data[0] = 0x1234; 
    e.data[1] = i; 
    el.push_back(e); 
    nes->addEvents(el); 
  }
  char x; 
  
  read(pn->getEventFifoPipe(), &x, 1); 
  sn::pEventPacket_t ep = pn->getNewEvents(); 
  sn::EventList_t::iterator i; 
  int pos = 0; 
  for (i = ep->events->begin(); i != ep->events->end(); i++) {
    BOOST_CHECK_EQUAL(i->cmd, 10); 
    BOOST_CHECK_EQUAL(i->src, 20); 
    BOOST_CHECK_EQUAL(i->data[0], 0x1234); 
    BOOST_CHECK_EQUAL(i->data[1], pos); 

    pos++; 
  }
  
  pn->shutdown(); 

  nes->shutdown(); 

}

BOOST_AUTO_TEST_SUITE_END(); 
