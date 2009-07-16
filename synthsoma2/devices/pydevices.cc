#include <boost/python.hpp>
#include <boost/python/class.hpp>
#include <boost/python/converter/shared_ptr_from_python.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/module.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

#include <synthsoma2/types.h>
#include <synthsoma2/devices/testdevice.h>


using namespace boost::python;

BOOST_PYTHON_MODULE(devices)
{

  using namespace synthsoma2; 
  class_<sn::EventTX_t>("EventTX"); 

  class_<sn::Event_t>("Event"); 
  
//   class_<TestDevice, bases<IEventDevice>, pTestDevice_t, 
//     boost::noncopyable>
//   ("TestDevice")
//   .def("sendTXEvents", &TestDevice::debugSendTXEvents)
//   .def("getEvents()", &TestDevice::debugGetReceivedEvents);

}

