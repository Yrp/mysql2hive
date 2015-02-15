
#if defined( WIN32 )
#  include <windows.h>
#  include <winbase.h>
#else
#  include <sys/time.h>
#  include <unistd.h>
#endif

#include <cassert>

#include "timer.hpp"
 
unsigned long
Timer::mTimerCount = 1L;

Timer::Timer(unsigned long tms) :
    mWhen(tms + getTimeMs()),
    mId(++mTimerCount),
    mDuration(tms)
{
}

Timer::Timer(const Timer& t):
    mWhen(t.mWhen),
    mId(t.mTimerCount),
    mDuration(t.mDuration)
{
}

Timer&
Timer::operator=(const Timer& other)
{
    if (this != &other)
    {
        mWhen = other.mWhen;
        mId = other.mTimerCount;
        mDuration = other.mDuration;
    }
    return *this;
}

Timer::~Timer() 
{}

UInt64
Timer::getSystemTime()
{
    assert( sizeof(UInt64) == 64/8 );
    UInt64 time=0;
#if defined(WIN32)  
    FILETIME ft;
#ifdef UNDER_CE
    SYSTEMTIME st;
    GetSystemTime( &st);
    SystemTimeToFileTime(&st,&ft);
#else
    GetSystemTimeAsFileTime( &ft);
#endif

    ULARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    time = li.QuadPart/10;
#else
    struct timeval now;
    gettimeofday( &now , NULL );
    //assert( now );
    time = now.tv_sec;
    time = time*1000000;
    time += now.tv_usec;
#endif
    return time;
}


void 
Timer::setupTimeOffsets()
{
}


UInt64 
Timer::getTimeMicroSec()
{
    return getSystemTime();
}


UInt64 
Timer::getTimeMs()
{
    return getSystemTime()/1000LL;
}

UInt64 
Timer::getTimeSecs()
{
    return getSystemTime()/1000000LL;
}


UInt64 
Timer::getForever()
{
    assert( sizeof(UInt64) == 8 );
#if defined(WIN32) && !defined(__GNUC__)
    return 18446744073709551615ui64;
#else
    return 18446744073709551615ULL;
#endif
}


bool 
operator<(const Timer& t1, const Timer& t2)
{
    //std::cerr << "operator(<" << t1.mWhen << ", " << t2.mWhen << ") = " << (t1.mWhen < t2.mWhen) << std::endl;
    return t1.mWhen < t2.mWhen;
}

bool 
operator>(const Timer& t1, const Timer& t2)
{
    return t1.mWhen > t2.mWhen;
}

std::ostream& 
operator<<(std::ostream& str, const Timer& t)
{
   UInt64 now = Timer::getTimeMs();

   str << "Timer[id=" << t.mId << " when=" << t.mWhen << " rel=";
   if (t.mWhen < now)
   {
      str << "past";
   }
   else
   {
      str << (t.mWhen - now);
   }
   str << "]";
   return str;
}
