#include <synthsoma2/eventrouter.h>
#include <iterator> 

namespace synthsoma2
{
  EventRouter::EventRouter() :
    ecyclecnt_(0), 
    cycleHasBeenProcessed_(false)
  {
    for (int i = 0; i < MAXEVENT; i++) {
      inputbuffers_.push_back(new inputbuffer_t); 
      outputbuffers_.push_back(new outputbuffer_t); 
      eventSendCount_[i] = 0; 
      eventReceiveCount_[i] = 0; 
    }
  }

  EventRouter::~EventRouter()
  {


  }

  void EventRouter::enableDevice(sn::eventsource_t src)
  {


    if (src > ( MAXEVENT -1)) {
      throw std::runtime_error("Cannot enable event source > MAXEVENT"); 
    }
    SS2L_(info) << "EventRouter: Enabled device " << (int)src; 

    eventsEnabled_[src] = true; 

  }

  const eventlist_t EventRouter::gatherEvents(sn::eventsource_t src)
  {
    if (src >= MAXEVENT) {
      throw std::runtime_error("Cannot obtain events for non-enabled device"); 

    }

    if (!eventsEnabled_[src]) {
      throw std::runtime_error("Device is not enabled, cannot gather events"); 
    }

    eventlist_t s; 
    copy(outputbuffers_[src].begin(), outputbuffers_[src].end(), 
	 std::back_insert_iterator<eventlist_t >(s)); 
    outputbuffers_[src].clear(); 
    return s; 
  }

  void EventRouter::eventCycle()
  {
    ecyclecnt_++; 
    ecyclesig_(ecyclecnt_); 
    cycleHasBeenProcessed_ = false; 
    
  }
  
  std::pair<size_t, size_t> EventRouter::getTotalCounts()
  {

    return std::make_pair(eventSendCountTotal_, eventReceiveCountTotal_); 

  }

  void EventRouter::processEventCycle() {
    if (cycleHasBeenProcessed_) {
      throw std::runtime_error("Cycle has been processed already"); 
    }

    size_t sentcnt = 0; 
    size_t receivecnt = 0; 

    // For each event source
    for (int i = 0; i < MAXEVENT; i++) {
      if (!inputbuffers_[i].empty()) {
	eventSendCount_[i] += 1; 
	sentcnt++; 
	// if there are pending events, then get them
	// and transfer to output queue
	
	sn::EventTX_t e = inputbuffers_[i].front(); 
	inputbuffers_[i].pop_front(); 
	
	for (int tgti = 0; tgti < sn::ADDRBITS; tgti++) {
	  if (e.destaddr[tgti]) {
	    if (eventsEnabled_[tgti]) {
	      eventReceiveCount_[tgti] += 1; 
	      receivecnt++; 
	      outputbuffers_[tgti].push_back(e.event); 
	    }
	  }
	  
	}
	
      }
      
    }

    eventSendCountTotal_ += sentcnt; 
    eventReceiveCountTotal_ += receivecnt; 

  }
  
  void EventRouter::addEvent(sn::eventsource_t src, sn::EventTX_t et)
  {
    
    if (src >= MAXEVENT) {
      throw std::runtime_error("Cannot obtain events for src >= MAXEVCENT"); 

    }
    
    if (inputbuffers_[src].size() > INPUTBUFFER_WARN) {
      SS2L_(warning) << "EventRouter: Input buffer " 
		  << src << " near overflow"; 
      
    }
    
    inputbuffers_[src].push_back(et);     
    
  }

  void EventRouter::addEvents(sn::eventsource_t src, const eventtxlist_t & et)
  {
    
    if (src >= MAXEVENT) {
      throw std::runtime_error("Cannot obtain events for src >= MAXEVCENT"); 

    }
    
    if (inputbuffers_[src].size() + et.size() > INPUTBUFFER_WARN) {
      SS2L_(warning) << "EventRouter: Input buffer " 
		  << src << " near overflow"; 
      
    }
    for (eventtxlist_t::const_iterator eti = et.begin(); eti != et.end(); eti++)
      {
	inputbuffers_[src].push_back(*eti);     
      }
    
  }
 
}
