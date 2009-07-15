#ifndef __SYNTHSOMA2_TYPES_H__
#define __SYNTHSOMA2_TYPES_H__

#include <somanetwork/event.h>
#include <somanetwork/eventtx.h>
#include <boost/utility.hpp> 
#include <synthsoma2/sslogging.h>

namespace synthsoma2
{
  namespace sn = somanetwork;   
  
  const int MAXEVENT = 80; 
  
  typedef uint64_t ecyclecnt_t; 
}

#endif // 
