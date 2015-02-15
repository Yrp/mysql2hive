
#include <cassert>
#include "abstractfifo.hpp"
#include "timer.hpp"


AbstractFifo::AbstractFifo(unsigned long maxSize)
   : mSize(0),
     mMaxSize(maxSize)
{}

AbstractFifo::~AbstractFifo()
{}


void*
AbstractFifo ::getNext()
{
   Lock lock(mMutex); (void)lock;

   // Wait util there are messages available.
   while (mFifo.empty())
   {
      mNotEmpty.wait(mMutex);
   }

   // Return the first message on the fifo.
   //
   void* firstMessage = mFifo.front();
   mFifo.pop_front();
   assert(mSize != 0);
   mSize--;
   mNotFull.signal();

   return firstMessage;
}

void*
AbstractFifo::getNext(int ms)
{
   if(ms == 0)
   {
      return getNext();
   }

   const UInt64 begin(Timer::getTimeMs());
   const UInt64 end(begin + (unsigned int)(ms)); // !kh! ms should've been unsigned :(
   Lock lock(mMutex); (void)lock;

   // Wait until there are messages available
   while (mFifo.empty())
   {
      const UInt64 now(Timer::getTimeMs());
      if(now >= end)
      {
          return 0;
      }

      unsigned int timeout((unsigned int)(end - now));

      // bail if total wait time exceeds limit
      bool signaled = mNotEmpty.wait(mMutex, timeout);
      if (!signaled)
      {
         return 0;
      }
   }

   // Return the first message on the fifo.
   //
   void* firstMessage = mFifo.front();
   mFifo.pop_front();
   assert(mSize != 0);
   mSize--;
//   if (mSize <= 1)
//   {
      mNotFull.signal();
//   }
   return firstMessage;
}

bool
AbstractFifo::empty() const
{
   Lock lock(mMutex); (void)lock;
   return mSize == 0;
}


unsigned int
AbstractFifo::size() const
{
   Lock lock(mMutex); (void)lock;
   return mSize;
}

time_t
AbstractFifo::timeDepth() const
{
   return 0;
}

bool
AbstractFifo::messageAvailable() const
{
   Lock lock(mMutex); (void)lock;
   assert(mSize != NoSize);
   return !mFifo.empty();
}

size_t
AbstractFifo::getCountDepth() const
{
   return mSize;
}

time_t
AbstractFifo::getTimeDepth() const
{
   return 0;
}
