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
