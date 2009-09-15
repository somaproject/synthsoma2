#include "synthsoma2/devices/audiodevice.h"
#include <boost/foreach.hpp>
#include <algorithm>

namespace synthsoma2
{

AudioDevice::AudioDevice() :
  cnt_(0), 
  enabled_(false),
  enabledChan_(0),
  time_(0),
  signaltime_(0.0)
{
  rates_.push_back(80.0); 
  rates_.push_back(220.0); 
  rates_.push_back(440.0); 
  rates_.push_back(1000); 

}

AudioDevice::~AudioDevice()
{
  
}


void AudioDevice::setDeviceID(sn::eventsource_t id) 
{
  src_ = id; 
}

void AudioDevice::ecycle(ecyclecnt_t cnt)
{
  time_ += ECYCLETIME; 
  
  while (time_ > (4*FS)) { 
    // send an event, if that's what we're supposed to do
    if (enabled_) { 
      sn::EventTX_t etx; 
      // broadcast
      for(int i = 0; i < somanetwork::ADDRBITS; i++) {
	etx.destaddr[i] = true; 
      }

      etx.event.src = src_; 
      etx.event.cmd = AUDIO_BCAST_CMD; 
      
      etx.event.data[0] = 1;
      float rate = rates_[enabledChan_]; 
      
      for (int i = 0; i < 4; i++) {
	etx.event.data[i+1] = int(sin(rate * signaltime_) * (1<<14)); // not full-scale
	signaltime_ += FS; 	
	
      }
      outboundevents_.push_back(etx); 
    }
    time_ -= (4*FS); 

  }
}

void AudioDevice::run() 
{
  
  
}

void AudioDevice::shutdown() 
{

  
}


const optEventTX_t AudioDevice::getTXEvent() {
  
  optEventTX_t oute ;
  if (!outboundevents_.empty()) { 
    oute = boost::make_optional(outboundevents_.front()); 
      outboundevents_.pop_front(); 
  }

  return oute; 
  
}

void AudioDevice::sendEvent(const sn::Event_t & et) 
{
  if (et.cmd == AUDIO_CONTROL_CMD) { 
    // something changing the state? 
    if (et.data[1]  == 1) { 
      if (et.data[2] == 1) { 
	enabled_ = true; 
      } else { 
	enabled_ = false; 
      }
      enabledChan_ = et.data[3]; 
  
      // send a status change update

      sn::EventTX_t etx; 
      // broadcast
      for(int i = 0; i < somanetwork::ADDRBITS; i++) {
	etx.destaddr[i] = true; 
      }

      etx.event.src = src_; 
      etx.event.cmd = AUDIO_BCAST_CMD; 
      
      etx.event.data[0] = 0;
      etx.event.data[1] = enabled_; 
      etx.event.data[2] = enabledChan_; 
      etx.event.data[3] = 32000; 
      outboundevents_.push_back(etx); 


    }
    
  }
  

}


}
