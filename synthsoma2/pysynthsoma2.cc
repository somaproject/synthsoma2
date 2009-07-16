#include <boost/python.hpp>
#include <boost/python/class.hpp>
#include <boost/python/converter/shared_ptr_from_python.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/module.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

#include <synthsoma2/syntheticsoma.h>

using namespace boost::python;

BOOST_PYTHON_MODULE(pysynthsoma2)
{
  using namespace synthsoma2; 

  class_<runnerstats_t>("runnerstats")
    .def_readonly("eventcycles", &runnerstats_t::eventcycles)
    .def_readonly("eventcyclerate", &runnerstats_t::eventcyclerate)
    .def_readonly("eventsenttotal", &runnerstats_t::eventsenttotal)
    .def_readonly("eventreceivetotal", &runnerstats_t::eventreceivetotal); 
  


}
