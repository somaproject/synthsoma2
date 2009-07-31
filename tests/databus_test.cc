#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <somanetwork/datapacket.h>
#include <synthsoma2/databus.h>
#include <synthsoma2/devices/timer.h>
#include <synthsoma2/datadevice.h>
#include <synthsoma2/netdataserver.h>
#include <time.h>


#include "testutil.h"
using namespace boost;       


BOOST_AUTO_TEST_SUITE(databus); 
using namespace synthsoma2; 
using namespace somanetwork; 



class TestDataDevice : public IDataDevice
{
public:
  
  void setDeviceDataSrc(sn::datasource_t id) {
    dsrc = id; 

  }

  // thread-safe functions! 
  void visitSubmitData(IDataBus * db) {
    if(!whattosend_.empty()) {
      if (whattosend_.front() == 0) {
	// send tspike

	if (tspikes_.empty()) {
	  throw std::runtime_error("tspikes buffer empty"); 
	}
	
	db->newData(dsrc, tspikes_.front()); 
	tspikes_.pop_front();

      } else if (whattosend_.front() == 1) {
	// send wave

	if (waves_.empty()) {
	  throw std::runtime_error("waves buffer empty"); 
	}

	db->newData(dsrc, waves_.front()); 
	waves_.pop_front();


      } else if (whattosend_.front() == 2) {
	// send raw

	if (raws_.empty()) {
	  throw std::runtime_error("raws buffer empty"); 
	}

	db->newData(dsrc, raws_.front()); 
	raws_.pop_front(); 
	
      }
      
      whattosend_.pop_front(); 
    }
    
  }
  
  sn::datasource_t dsrc; 

  std::list<int> whattosend_; 
  
  std::list<sn::TSpike_t> tspikes_; 
  std::list<sn::Wave_t> waves_; 
  std::list<sn::Raw_t> raws_; 

};

typedef boost::shared_ptr<TestDataDevice> pTestDataDevice_t; 


class TestDataSink: public IDataSink
{
  public:
  void run(){

  }

  void shutdown()
  {

  }

  void sendData(DataBuffer * db) 
  {
    databuffers.push_back(db); 

  }
  
  std::list<DataBuffer *> databuffers; 
  
}; 

typedef boost::shared_ptr<TestDataSink> pTestDataSink_t; 
  

BOOST_AUTO_TEST_CASE(startup)
{
  /*
    Create the single debug device, have it send data to 
    a deviceid with no connected device, and then one to itself

   */
  DataBus::devicemap_t dm; 
  
  pTestDataDevice_t td(new TestDataDevice); 

  pTestDataSink_t sink(new TestDataSink); 
  

  const int MYID = 3; 
  dm[MYID] = td; 
  
  DataBus db(dm); 

  db.setDataSink(sink); 

  // add a single tspike
  sn::TSpike_t ts; 
  ts.src = MYID; 
  ts.time = 0x12345678; 
  
  td->tspikes_.push_back(ts); 
  td->whattosend_.push_back(0); 
  
  db.run(); 
  
  for(int i = 0; i < 100; i++) {
    db.ecycle(i);     
    timespec ts; 
    ts.tv_sec = 0; 
    ts.tv_nsec = 1000000; 
    nanosleep(&ts, &ts); 
  }

  BOOST_CHECK_EQUAL(sink->databuffers.size(), 1); 
  BOOST_CHECK_EQUAL(sink->databuffers.front()->src, MYID); 
  BOOST_CHECK_EQUAL(sink->databuffers.front()->typ, sn::TSPIKE); 
  

  db.shutdown(); 

}


BOOST_AUTO_TEST_CASE(data_types)
{
  /*
    Create the test device, and then send the TSpike, Wave, and Raw in
    order. 

   */
  DataBus::devicemap_t dm; 
  
  pTestDataDevice_t td(new TestDataDevice); 

  pTestDataSink_t sink(new TestDataSink); 
  

  const int MYID = 3; 
  dm[MYID] = td; 
  
  DataBus db(dm); 

  db.setDataSink(sink); 

  // add a single tspike
  sn::TSpike_t ts; 
  ts.src = MYID; 
  ts.time = 0x12345678; 
  
  td->tspikes_.push_back(ts); 

  td->whattosend_.push_back(0); 

  sn::Wave_t wave; 
  wave.src = MYID; 
  wave.time = 0x11223344; 
  td->waves_.push_back(wave); 
  td->whattosend_.push_back(1); 

  sn::Raw_t raw; 
  raw.src = MYID; 
  raw.time = 0xAABBCCDD; 
  td->raws_.push_back(raw); 
  td->whattosend_.push_back(2); 

  
  db.run(); 
  
  for(int i = 0; i < 100; i++) {
    timespec ts; 
    ts.tv_sec = 0; 
    ts.tv_nsec = 1000000; 
    nanosleep(&ts, &ts); 
    db.ecycle(i);     
  }

  BOOST_CHECK_EQUAL(sink->databuffers.size(), 3); 


  BOOST_CHECK_EQUAL(sink->databuffers.front()->src, MYID); 
  BOOST_CHECK_EQUAL(sink->databuffers.front()->typ, sn::TSPIKE); 
  // decode buffer

  DataBuffer * databuffer = sink->databuffers.front(); 
  sink->databuffers.pop_front(); 

  pDataPacket_t dp(new DataPacket_t); 
  dp->src = databuffer->src; 
  dp->typ = databuffer->typ; 
  dp->missing = false; 
  memcpy(&(dp->body[0]), databuffer->getFrameStartPtr(), 
	 databuffer->getLen() - 4); 
  
  TSpike_t recovered_tspike = rawToTSpike(dp); 
  BOOST_CHECK_EQUAL(recovered_tspike.time, ts.time); 

  // Check Wave

  if (sink->databuffers.size()  > 0) {
    BOOST_CHECK_EQUAL(sink->databuffers.front()->typ, sn::WAVE); 
    databuffer = sink->databuffers.front(); 
    sink->databuffers.pop_front(); 
    
    pDataPacket_t dp1(new DataPacket_t); 
    dp1->src = databuffer->src; 
    dp1->typ = databuffer->typ; 
    dp1->missing = false; 
    memcpy(&(dp1->body[0]), databuffer->getFrameStartPtr(), 
	   databuffer->getLen() - 4); 
    
    Wave_t recovered_wave = rawToWave(dp1); 
    BOOST_CHECK_EQUAL(recovered_wave.time, wave.time); 
  }



  if (sink->databuffers.size()  > 0) {
    BOOST_CHECK_EQUAL(sink->databuffers.front()->typ, sn::RAW); 
    databuffer = sink->databuffers.front(); 
    sink->databuffers.pop_front(); 
    
    pDataPacket_t dp1(new DataPacket_t); 
    dp1->src = databuffer->src; 
    dp1->typ = databuffer->typ; 
    dp1->missing = false; 
    memcpy(&(dp1->body[0]), databuffer->getFrameStartPtr(), 
	   databuffer->getLen() - 4); 
    
    Raw_t recovered_raw = rawToRaw(dp1); 
    BOOST_CHECK_EQUAL(recovered_raw.time, raw.time); 
  }



  db.shutdown(); 

}



BOOST_AUTO_TEST_SUITE_END(); 
