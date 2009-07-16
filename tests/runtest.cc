#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <synthsoma2/neteventsender.h>

using namespace boost;       

using namespace synthsoma2; 

int main()
{

  // Can we just start and end
  boost::filesystem::path tgtdir = boost::filesystem::path("/tmp/test1assa"); 
  boost::filesystem::remove_all(tgtdir); 
  boost::filesystem::create_directory(tgtdir); 

  pNetEventSender_t nes = NetEventSender::createDomain(tgtdir); 
  
}

