#if !defined(RESIP_TIMER_HXX)
#define RESIP_TIMER_HXX 

#include <iostream>
#include "compat.hpp"

class Timer;
bool operator<(const Timer& t1, const Timer& t2);
bool operator>(const Timer& t1, const Timer& t2);
std::ostream& operator<<(std::ostream&, const Timer&);

class Timer
{
   public:
      typedef unsigned long Id;
      Timer(unsigned long ms);
      Timer(const Timer& t);
      Timer& operator=(const Timer& t);

      ~Timer();

      /// returns the unique identifier
      Id getId() const { return mId; }
      UInt64 getWhen() {return mWhen;}
      UInt64 getWhen() const {return mWhen;}
      static void setupTimeOffsets(); ///< initialize
      static UInt64 getTimeMicroSec(); ///< get a 64 bit time
      static UInt64 getTimeMs(); ///< in ms
      static UInt64 getTimeSecs(); ///< in secs
      static UInt64 getForever(); ///< infinit time in future

   protected:
   	  UInt64 mWhen; ///< time when the timer "goes off" in MS 
   	  Id mId;
      unsigned long mDuration; ///< duration of time in ms       
      static unsigned long mTimerCount; ///< counter to genreate unique timers ids 
   private:
       /// for TimerQueue only - don't use
      static UInt64 getSystemTime();

      friend bool operator<(const Timer& t1, const Timer& t2);
      friend bool operator>(const Timer& t1, const Timer& t2);
      friend std::ostream& operator<<(std::ostream&, const Timer&);

};

#endif
