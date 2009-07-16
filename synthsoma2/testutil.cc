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
#include "testutil.h"

namespace synthsoma2 { 

bf::path createTempDir()
{
  char tempdir[] = "/tmp/testXXXXXX"; 
  char * ptr = mkdtemp(tempdir); 
  if(ptr == NULL) {
    throw std::runtime_error("unable to create temporary directory"); 
  }

  return bf::path(ptr); 
}

int createBoundDomainSocket(bf::path srcpath)
{
  // Create and bind a temporary domain socket at the indicated path

  int sock = socket(AF_LOCAL, SOCK_DGRAM, 0); 
  
  if (sock < 0) {
    throw std::runtime_error("test infrastructure could not create socket"); 
    
  }
  
  struct sockaddr_un su_me; 
  
  
  bzero((char *) &su_me, sizeof(su_me));
  
  su_me.sun_family = AF_LOCAL;
  
  strcpy(su_me.sun_path, srcpath.string().c_str()); 
  
  int res =  bind(sock, (sockaddr*)&su_me, sizeof(su_me)); 
  
  if (res < 0) {
    throw std::runtime_error("test infrastrucutre error binding socket"); 
  }
  
  return sock; 
  
}

somanetwork::EventTX_t createCanonicalEventTX(somanetwork::eventsource_t src, 
					      somanetwork::eventcmd_t cmd, 
					      size_t index)
{
  /*
    Create canonical event and just slap it in a EventTX

   */

  somanetwork::EventTX_t etx; 
  etx.event = createCanonicalEvent(src, cmd, index); 
  return etx; 

}


somanetwork::Event_t createCanonicalEvent(somanetwork::eventsource_t src,
					  somanetwork::eventcmd_t cmd, 
					  size_t index)
{
  somanetwork::Event_t evt; 
  evt.src = src; 
  evt.cmd = cmd; 
  evt.data[0] = 0x1234; 
  evt.data[1] = index; 
  evt.data[2] = 0x1000 * index; 
  evt.data[3] = src; 

  return evt; 

} 

somanetwork::EventTX_t setdest(const somanetwork::EventTX_t & e, size_t d) 
{
  somanetwork::EventTX_t ed = e; 
  ed.destaddr[d] = true; 
  return ed; 
}

bool equal(const somanetwork::Event_t & e1, const somanetwork::Event_t & e2)
{
  if (e1.cmd != e2.cmd)
    return false; 

  if (e1.src != e2.src)
    return false;
  
  for(int i= 0; i < 5; i++) {
    if (e1.data[i] != e2.data[i]) {
      return false;
    }
  }
  
  return true; 



}

}
