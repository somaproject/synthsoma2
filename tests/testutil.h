#ifndef __TESTUTIL__
#define __TESTUTIL__
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <boost/array.hpp>
#include <arpa/inet.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include <somanetwork/datareceiver.h>
#include <somanetwork/network.h>

#include <sys/un.h>
#include <sys/socket.h>

namespace bf = boost::filesystem; 

bf::path createTempDir(); 
int createBoundDomainSocket(bf::path srcpath); 



#endif 
