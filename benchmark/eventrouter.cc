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

/*
  Here we are going to measure the performance of our event router
  and the associated overhead. We're using a series of dummy 
  event-capturing sinks, and the timer as our source. 
  
  50e3 event cycles / sec gives us only 50k ticks per ecycle (on a single
  core) to do what we want. 
  
  HARD REQUIREMENT: we need to be able to have 64 sinks and have them all not choke
  on the timer events. 

 */ 

namespace po = boost::program_options;


int main(int argc, char* argv[])
{


  
}
