#ifndef __SYNTHSOMA2_TESTUTIL__
#define __SYNTHSOMA2_TESTUTIL__
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <boost/array.hpp>
#include <arpa/inet.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include <somanetwork/datareceiver.h>
#include <somanetwork/event.h>
#include <somanetwork/eventtx.h>
#include <somanetwork/network.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <synthsoma2/types.h>

namespace bf = boost::filesystem; 

namespace synthsoma2 {

bf::path createTempDir(); 
int createBoundDomainSocket(bf::path srcpath); 
somanetwork::EventTX_t createCanonicalEventTX(somanetwork::eventsource_t src, somanetwork::eventcmd_t cmd, 
				 size_t index); 

somanetwork::EventTX_t setdest(const somanetwork::EventTX_t & d, size_t addr) ;

somanetwork::Event_t createCanonicalEvent(somanetwork::eventsource_t src, somanetwork::eventcmd_t cmd, 
			     size_t index); 


bool equal(const somanetwork::Event_t & e1, const somanetwork::Event_t & e2);
}







#endif 
