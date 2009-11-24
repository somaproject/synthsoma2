#ifndef __SYNTHSOMA2_AUDIODEVICE_H__
#define __SYNTHSOMA2_AUDIODEVICE_H__

#include <synthsoma2/types.h>
#include <synthsoma2/eventdevice.h> 
#include <boost/thread/mutex.hpp>

namespace synthsoma2
{
/* 
   Simple device implementing the audio monitor protocol. 
   Very simple sending of 32 kHz events every N events. 

 */ 
  class AudioDevice : public IEventDevice, 
		boost::noncopyable
  {
    typedef uint64_t timestamp_t; 
  public:
    AudioDevice(); 
    ~AudioDevice(); 
    
    // Event Device Interface
    void ecycle(ecyclecnt_t);
    void setDeviceID(sn::eventsource_t id); 

    void ignoreSource(sn::eventsource_t src); 

 
    const optEventTX_t getTXEvent(); 
    void sendEvent(const sn::Event_t &); 


    void run(); 
    void shutdown(); 

    static const int AUDIO_BCAST_CMD = 0x18; 
    static const int AUDIO_CONTROL_CMD = 0x30; 

  private:
    eventtxlist_t outboundevents_; 
    eventlist_t inboundevents_; 
    sn::eventsource_t src_; 

    // audio-monitor-specific state
    ecyclecnt_t cnt_; 
    bool enabled_; 
    int enabledChan_; 
    static const float FS = 1/32000.; 
    static const float ECYCLETIME = 1/50000.; 
    float time_;
    float signaltime_; 
    
    std::vector<double> rates_;
    int volume_; 
    
  }; 

  typedef boost::shared_ptr<AudioDevice> pAudioDevice_t; 

}

#endif 

