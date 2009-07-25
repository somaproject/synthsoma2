#include <boost/python.hpp>
#include <boost/python/class.hpp>
#include <boost/python/converter/shared_ptr_from_python.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/module.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>


#include <synthsoma2/syntheticsoma.h>
#include <synthsoma2/eventbus.h>
#include <synthsoma2/devices/testdevice.h>
#include <synthsoma2/devices/timer.h>
#include <synthsoma2/devices/simpletspike.h>
#include <synthsoma2/neteventserver.h>
#include <synthsoma2/netdataserver.h>

using namespace boost::python;
namespace bp = boost::python; 

synthsoma2::pNetEventServer_t eventCreateDomain(std::string s)
{
  return synthsoma2::NetEventServer::createDomain(s); 

}

synthsoma2::pNetDataServer_t dataCreateDomain(std::string s)
{
  return synthsoma2::NetDataServer::createDomain(s); 

}

bp::list ts_get_wave(somanetwork::TSpikeWave_t  * ts)
{
  bp::list newlist; 
  for (int i = 0; i < 32; i++) {
    newlist.append(ts->wave[i]); 
  }
  return newlist;  

}

void ts_set_wave(somanetwork::TSpikeWave_t  * ts, bp::list data)
{
  for (int i = 0; i < 32; i++) {
    ts->wave[i] = bp::extract<int32_t>(data[i]); 
  }

}

boost::python::list TestDevice_pyGetReceivedEvents(synthsoma2::TestDevice *td)
{
  boost::python::list l; 
  synthsoma2::eventlist_t eventlist = td->debugGetReceivedEvents(); 
  
  BOOST_FOREACH(somanetwork::Event_t e,  eventlist) {
    l.append(e); 
  }
  
  return l; 
}

void TestDevice_pySendTXEvents(synthsoma2::TestDevice * td, 
			       const somanetwork::EventTX_t & e)
{
  td->debugSendTXEvents(e); 
}

    


BOOST_PYTHON_MODULE(pysynthsoma2)
{
  using namespace synthsoma2; 

  /* -----------------------------------------------------------------------
     Utility
     ------------------------------------------------------------------------*/

  class_<sn::EventTX_t>("EventTX"); 


  class_<sn::Event_t>("Event")
    .def_readwrite("src", &sn::Event_t::src)
    .def_readwrite("cmd", &sn::Event_t::cmd)
    .def_readwrite("data", &sn::Event_t::data); 
    
  class_<sn::TSpikeWave_t>("TSpikeWave")
    .def_readwrite("valid", &sn::TSpikeWave_t::valid)
    .def_readwrite("filtid", &sn::TSpikeWave_t::filtid)
    .def_readwrite("threshold", &sn::TSpikeWave_t::threshold)
    // TOTALLY SUBOPTMIAL (FIXME): for the time being we need to assign
    // this property as an -entire- wave, i.e. ts.wave = range(32)
    .add_property("wave", &ts_get_wave, &ts_set_wave); 


  class_<sn::TSpike_t>("TSpike")
    .def_readwrite("src", &sn::TSpike_t::src)
    .def_readwrite("time", &sn::TSpike_t::time)
    .def_readwrite("x", &sn::TSpike_t::x)
    .def_readwrite("y", &sn::TSpike_t::y)
    .def_readwrite("a", &sn::TSpike_t::a)
    .def_readwrite("b", &sn::TSpike_t::b); 

  class_<runnerstats_t>("runnerstats")
    .def_readonly("eventcycles", &runnerstats_t::eventcycles)
    .def_readonly("eventcyclerate", &runnerstats_t::eventcyclerate)
    .def_readonly("eventsenttotal", &runnerstats_t::eventsenttotal)
    .def_readonly("eventreceivetotal", &runnerstats_t::eventreceivetotal)
    .def_readonly("datapackets", &runnerstats_t::datapackets)
    .def_readonly("datapacketrate", &runnerstats_t::datapacketrate); 


  /* -----------------------------------------------------------------------
     Core Busses
     ------------------------------------------------------------------------*/

  
  class_<EventBus::devicemap_t>("EventDeviceMap")
    .def(map_indexing_suite<EventBus::devicemap_t>());

  class_<DataBus::devicemap_t>("DataDeviceMap")
    .def(map_indexing_suite<DataBus::devicemap_t>());

  
  
  class_<EventBus, boost::noncopyable, pEventBus_t>("EventBus", 
						    init<EventBus::devicemap_t>()); 
  class_<DataBus, boost::noncopyable, pDataBus_t>("DataBus",
						  init<DataBus::devicemap_t>())
    .def("setDataSink", &DataBus::setDataSink); 
  
  class_<IDataSink, boost::noncopyable, pDataSink_t>("DataSink", 
						     no_init); 
  
  class_<NetDataServer, boost::noncopyable, pNetDataServer_t, bases<IDataSink> >
    ("NetDataServer", no_init)
    .def("createDomain", &dataCreateDomain).staticmethod("createDomain") 
    .def("createINet", &NetDataServer::createINet).staticmethod("createINet"); 
  
  
  /* -----------------------------------------------------------------------
     Utility
     ------------------------------------------------------------------------*/

  class_<SynthSomaRunner, boost::noncopyable>("Runner", 
					      init<pEventBus_t &, pDataBus_t&>())
    .def("run", &SynthSomaRunner::run)
    .def("shutdown", &SynthSomaRunner::shutdown)
    .def("getStats", &SynthSomaRunner::getStats);


  /* -----------------------------------------------------------------------
     DEVICES 
     ------------------------------------------------------------------------*/
  class_<IEventDevice, boost::noncopyable>("EventDevice", no_init); 
  class_<IDataDevice, boost::noncopyable>("DataDevice", no_init); 
  
  // Text Device
  class_<TestDevice, bases<IEventDevice>, pTestDevice_t, 
    boost::noncopyable>("TestDevice")
    .def("sendTXEvents", &TestDevice_pySendTXEvents)
    .def("getEvents", &TestDevice_pyGetReceivedEvents);

   // Timer
   class_<Timer, bases<IEventDevice>, pTimer_t, boost::noncopyable>
   ("Timer"); 

// Network Event Interface
   
   class_<NetEventServer, bases<IEventDevice>, pNetEventServer_t, boost::noncopyable>
   ("NetEventServer", no_init)
     .def("createDomain", &eventCreateDomain).staticmethod("createDomain")
     .def("createINet", &NetEventServer::createINet).staticmethod("createINet"); 

   class_<SimpleTSpike, bases<IEventDevice, IDataDevice>, 
    pSimpleTSpike_t, boost::noncopyable>("SimpleTSpike")
    .def("addTSpike", &SimpleTSpike::addTSpike); 


}
