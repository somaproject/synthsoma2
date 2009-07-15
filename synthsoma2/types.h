#ifndef __SYNTHSOMA2_TYPES_H__
#define __SYNTHSOMA2_TYPES_H__

#include <somanetwork/event.h>
#include <somanetwork/eventtx.h>
#include <boost/utility.hpp> 
#include <boost/shared_ptr.hpp>

#include <synthsoma2/sslogging.h>

namespace synthsoma2
{
  namespace sn = somanetwork;   
  
  const int MAXEVENT = 80; 
  
  typedef uint64_t ecyclecnt_t; 
  typedef std::list<sn::EventTX_t> eventtxlist_t; 
  typedef sn::EventList_t eventlist_t; 

  class NetEventSender; 
}

#endif // 
