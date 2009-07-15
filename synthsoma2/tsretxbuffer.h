#ifndef __SYNTHSOMA2_TSRETXBUFFER_H__
#define __SYNTHSOMA2_TSRETXBUFFER_H__

#include <synthsoma2/types.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>


namespace synthsoma2
{
  /*
    Thread-safe Retransmission Buffer

    Items are stored by a sequenceid_t, which is some
    sort of unsigned integer
    
    We allocate the ENTIRE BUFFER at start-up, so don't
    request too big of a buffer!
    
  */
  template<class seqid_t, class data_t>
  class TSReTxBuffer
  {
  public: 
    TSReTxBuffer(size_t size, data_t initval) :
      size_(size), 
      buffer_(size)
    {
      for (size_t i = 0; i < size; i++) {
	buffer_[i] = initval; 
      }
      
    }
    
    void save(seqid_t id, const data_t & data)
    {
      size_t bufpos = (size_t)id % size_; 

      boost::mutex::scoped_lock lock(mutex_); 

      buffer_[bufpos] = data; 
    }

    data_t  lookup(seqid_t id)
    {
      size_t bufpos = (size_t)id % size_; 
      
      boost::mutex::scoped_lock lock(mutex_); 
      return buffer_[bufpos]; 
    }
    
  private:
    size_t size_; 
    std::vector<data_t> buffer_; 
    boost::mutex mutex_; 

  }; 

}


#endif // 
