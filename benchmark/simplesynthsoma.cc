#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <synthsoma2/databus.h>
#include <synthsoma2/eventbus.h>
#include <synthsoma2/syntheticsoma.h>
#include <synthsoma2/neteventserver.h>
#include <synthsoma2/devices/timer.h>
#include <synthsoma2/devices/testdevice.h>
#include <boost/program_options.hpp>
#include <synthsoma2/sslogging.h>
#include <synthsoma2/testutil.h>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{

  po::options_description desc("Allowed options");
  
  desc.add_options()
    ("help", "produce help message")
    ("duration", po::value<int>()->default_value(10), "duration to run (sec)")
    ("enable-timer", "Enable EventBus Timer")
    ("enable-network", "Enable the Event and Data Network interfaces");
		   
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }



  using namespace synthsoma2; 

  synthsoma2::init_logs(warning); 


  EventBus::devicemap_t dm; 
  
  if(vm.count("enable-timer")) {
    dm[0] = pEventDevice_t(new Timer); 
  }


  if(vm.count("enable-network")) {
    boost::filesystem::path fp =  createTempDir(); 
    dm[4] = NetEventServer::createDomain(fp); 
  }


  pEventBus_t eb(new EventBus(dm)); 
  
  DataBus::devicemap_t ddm; 
  pDataBus_t db(new DataBus(ddm)); 
  
  SynthSomaRunner ssr(eb, db); 
  ssr.run(); 
  
  for (int i = 0; i < vm["duration"].as<int>(); i++)
    {
      sleep(1); 
      runnerstats_t s = ssr.getStats(); 
      std::cout << s.eventcycles << " " << s.eventcyclerate 
		<< " " << s.eventsenttotal 
		<< " " << s.eventreceivetotal << std::endl; 
    }
  
  ssr.shutdown(); 
  

  
}
