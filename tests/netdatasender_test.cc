#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include <synthsoma2/netdatasender.h>
#include <synthsoma2/netdataserver.h>
#include <somanetwork/network.h>
#include <synthsoma2/types.h>
#include "testutil.h"
#include <somanetwork/logging.h>
#include <somanetwork/tspike.h>
#include <somanetwork/datapacket.h>

using namespace boost;       


BOOST_AUTO_TEST_SUITE(netdatasender)
using namespace synthsoma2; 

BOOST_AUTO_TEST_CASE(create)
{
  // Can we just instantiate
  boost::filesystem::path tgtdir = createTempDir(); 
  pNetDataSender_t nds = NetDataSender::createDomain(tgtdir); 

}


BOOST_AUTO_TEST_CASE(startup)
{
  // Can we just start and end
  boost::filesystem::path tgtdir = createTempDir(); 

  pNetDataSender_t nds = NetDataSender::createDomain(tgtdir); 
  nds->run(); 
  nds->shutdown(); 
  

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
  
  pNetDataSender_t nds = NetDataSender::createDomain(tgtdir); 
  nds->run(); 
  
  sn::pNetworkInterface_t pn = sn::Network::createDomain(tgtdir); 
  pn->enableDataRX(0, sn::TSPIKE); 
  pn->run(); 


  // now send fake data
  for (int i = 0; i < 11; i++) {
    sn::TSpike_t ts; 
    ts.x.wave[0] = i; 
    ts.src = 0; 
    size_t len; 
    sn::pDataPacket_t dp = sn::rawFromTSpikeForTX(ts, 0, &len); 
    
    DataBuffer *  db = new DataBuffer(); 

    memcpy(db->getFrameStartPtr(), &(dp->body[0]), len); 

    db->src = 0; 
    db->typ = sn::TSPIKE; 
    
    db->setFrameLen(len); 
    nds->addDataBuffer(db); 
  }
  char x; 
  

  for (int i = 0; i < 10; i++) {
    read(pn->getDataFifoPipe(), &x, 1); 
    sn::pDataPacket_t newdp = pn->getNewData(); 
    sn::TSpike_t ts = sn::rawToTSpike(newdp); 
    BOOST_CHECK_EQUAL(ts.x.wave[0], i); 
    
  }
  
  pn->shutdown(); 

  nds->shutdown(); 

}

BOOST_AUTO_TEST_SUITE_END(); 
