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
#include <synthsoma2/neteventserver.h>

using namespace boost::python;
namespace bp = boost::python; 

synthsoma2::pNetEventServer_t createDomain(std::string s)
{
  return synthsoma2::NetEventServer::createDomain(s); 

}

BOOST_PYTHON_MODULE(pysynthsoma2)
{
  using namespace synthsoma2; 

  /* -----------------------------------------------------------------------
     Utility

     ------------------------------------------------------------------------*/

  class_<sn::EventTX_t>("EventTX"); 

  class_<sn::Event_t>("Event"); 

  class_<runnerstats_t>("runnerstats")
    .def_readonly("eventcycles", &runnerstats_t::eventcycles)
    .def_readonly("eventcyclerate", &runnerstats_t::eventcyclerate)
    .def_readonly("eventsenttotal", &runnerstats_t::eventsenttotal)
    .def_readonly("eventreceivetotal", &runnerstats_t::eventreceivetotal); 
  
  class_<EventBus::devicemap_t>("devicemap")
    .def(map_indexing_suite<EventBus::devicemap_t>());

  
  class_<EventBus, boost::noncopyable, pEventBus_t>("EventBus", 
						    init<EventBus::devicemap_t>()); 
  class_<DataBus, boost::noncopyable, pDataBus_t>("DataBus"); 
  
  class_<SynthSomaRunner, boost::noncopyable>("Runner", 
					      init<pEventBus_t &, pDataBus_t&>())
    .def("run", &SynthSomaRunner::run)
    .def("shutdown", &SynthSomaRunner::shutdown)
    .def("getStats", &SynthSomaRunner::getStats);


  /* -----------------------------------------------------------------------
     DEVICES 

     ------------------------------------------------------------------------*/
  class_<IEventDevice, boost::noncopyable>("EventDevice", no_init); 
  
  // Text Device
  class_<TestDevice, bases<IEventDevice>, pTestDevice_t, 
    boost::noncopyable>("TestDevice")
    .def("sendTXEvents", &TestDevice::pySendTXEvents)
    .def("getEvents", &TestDevice::pyGetReceivedEvents);

   // Timer
   class_<Timer, bases<IEventDevice>, pTimer_t, boost::noncopyable>
   ("Timer"); 

// Network Event Interface
   
   class_<NetEventServer, bases<IEventDevice>, pNetEventServer_t, boost::noncopyable>
   ("NetEventServer", no_init)
     .def("createDomain", &createDomain).staticmethod("createDomain"); 

   
}
