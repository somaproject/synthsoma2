#ifndef __SYNTHSOMA2_LOGGING_H__
#define __SYNTHSOMA2_LOGGING_H__

#include <boost/logging/format_fwd.hpp>
#include <boost/logging/format/named_write.hpp>

using namespace boost::logging::scenario::usage;
typedef use<
        //  the filter is always accurate (but slow)
        filter_::change::always_accurate, 
        //  filter does not use levels
        filter_::level::use_levels, 
        // the logger is initialized once, when only one thread is running
        logger_::change::set_once_when_one_thread, 
        // the logger favors speed (on a dedicated thread)
        logger_::favor::speed> synthsoma2_finder;



#define SS2L_(lvl) BOOST_LOG_USE_LOG_IF_LEVEL(synthsoma2_l(), synthsoma2_log_level(), lvl )

BOOST_DECLARE_LOG_FILTER(synthsoma2_log_level, synthsoma2_finder::filter ) // holds the application log level
BOOST_DECLARE_LOG(synthsoma2_l, synthsoma2_finder::logger)

namespace synthsoma2 
{
  
  using namespace boost::logging::level; 

  void init_logs( boost::logging::level::type level); 



}

#endif 
