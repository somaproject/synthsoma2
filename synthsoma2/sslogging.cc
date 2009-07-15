#include "sslogging.h"
#include <boost/logging/format.hpp>
#include <boost/logging/format/formatter/tags.hpp>
#include <boost/logging/format/named_write.hpp>
#include <boost/logging/format/formatter/tags.hpp>
#include <boost/logging/format/formatter/high_precision_time.hpp>

  //using namespace boost::logging;
namespace bl = boost::logging; 



BOOST_DEFINE_LOG_FILTER(synthsoma2_log_level, synthsoma2_finder::filter ) // holds the application log level
BOOST_DEFINE_LOG(synthsoma2_l, synthsoma2_finder::logger); 

namespace synthsoma2 {
  void init_logs( boost::logging::level::type level) {
    synthsoma2_log_level()->set_enabled(level); 
    synthsoma2_l()->writer().add_formatter(bl::formatter::high_precision_time("$mm.$ss:$micro ")); 
    synthsoma2_l()->writer().add_formatter(bl::formatter::append_newline()); 
    synthsoma2_l()->writer().add_destination( bl::destination::cout() );
    synthsoma2_l()->mark_as_initialized();

  }

}


