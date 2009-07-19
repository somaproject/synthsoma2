#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <boost/timer.hpp>
#include <synthsoma2/databus.h>
#include <synthsoma2/eventbus.h>
#include <synthsoma2/syntheticsoma.h>
#include <synthsoma2/neteventserver.h>
#include <synthsoma2/devices/timer.h>
#include <synthsoma2/devices/testdevice.h>
#include <boost/program_options.hpp>
#include <synthsoma2/sslogging.h>
#include <synthsoma2/testutil.h>
#include <synthsoma2/devices/timer.h>
#include <synthsoma2/devices/simpletspike.h>

/*
  Here we are going to measure the performance of our event bus
  and the associated overhead. We're using a series of dummy 
  event-capturing sinks, and the timer as our source. 
  
  50e3 event cycles / sec gives us only 50k ticks per ecycle (on a single
  core) to do what we want. 
  
  HARD REQUIREMENT: we need to be able to have 64 sinks and have them all not choke
  on the timer events. 

 */ 

namespace po = boost::program_options;
using namespace synthsoma2;

int main(int argc, char* argv[])
{

  EventBus::devicemap_t dm; 
  
  int TIMERN = 4; 
  for (int i = 0; i < TIMERN; i++) {
    pTimer_t timer(new Timer()); 
    dm[i] = timer; 
  }


  int TSPIKEN = 64; 
  std::list<pSimpleTSpike_t> sts; 
  for (int ti = 0; ti < TSPIKEN; ti++)
    {
      pSimpleTSpike_t ts(new SimpleTSpike); 
      dm[ti+8] = ts; 
      sts.push_back(ts); 
    }
  
  
  pEventBus_t eventbus(new EventBus(dm)); 
  std::cout << "eventBus created" << std::endl; 
  int ITERS = 100000;
	
  eventbus->run(); 
  boost::timer t; 
  for(int i = 0; i < ITERS; i++){
    eventbus->eventcycle(); 

  }
  double elapsed = t.elapsed(); 
  
  std::cout << ITERS/elapsed/1000. << " kecycles/sec" << std::endl;
  std::cout << ITERS/elapsed/50000. * 100 << "% of what's necessary" 
	    << std::endl;
  
  eventbus->shutdown(); 
  
}
