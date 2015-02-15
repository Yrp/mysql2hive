
#if !defined(RESIP_FIFO_HXX)
#define RESIP_FIFO_HXX

#include <cassert>
#include "abstractfifo.hpp"
#include "timer.hpp"
/**
   @brief A templated, threadsafe message-queue class.
*/
template < class Msg >
class Fifo : public AbstractFifo
{
   public:
//      Fifo();
      Fifo(unsigned long maxSize=0UL-1);
      virtual ~Fifo();

      /// Add a message to the fifo.
      void add(Msg* msg);
      void add(Msg* msg, int ms);

      /** Returns the first message available. It will wait if no
       *  messages are available. If a signal interrupts the wait,
       *  it will retry the wait. Signals can therefore not be caught
       *  via getNext. If you need to detect a signal, use block
       *  prior to calling getNext.
       */
      Msg* getNext();


      /** Returns the next message available. Will wait up to
       *  ms milliseconds if no information is available. If
       *  the specified time passes or a signal interrupts the
       *  wait, this method returns 0. This interface provides
       *  no mechanism to distinguish between timeout and
       *  interrupt.
       */
      Msg* getNext(int ms);

      /// delete all elements in the queue
      virtual void clear();

   private:
      Fifo(const Fifo& rhs);
      Fifo& operator=(const Fifo& rhs);
};


//template <class Msg>
//Fifo<Msg>::Fifo() :
//   AbstractFifo(0UL-1)
//{
//}

template <class Msg>
Fifo<Msg>::Fifo(unsigned long maxSize) :
   AbstractFifo(maxSize)
{
}

template <class Msg>
Fifo<Msg>::~Fifo()
{
   clear();
}

template <class Msg>
void
Fifo<Msg>::clear()
{
   Lock lock(mMutex); (void)lock;
   while ( ! mFifo.empty() )
   {
      Msg* msg = static_cast<Msg*>(mFifo.front());
      mFifo.pop_front();
      delete msg;
   }
   assert(mFifo.empty());
   mSize = 0UL -1;
}

template <class Msg>
void
Fifo<Msg>::add(Msg* msg)
{
   Lock lock(mMutex); (void)lock;

   while (mSize >= mMaxSize)
   {
      mNotFull.wait(mMutex);
   }
   mFifo.push_back(msg);
   mSize++;
   mNotEmpty.signal();
}

template <class Msg>
void
Fifo<Msg>::add(Msg* msg, int ms)
{
   if(ms == 0)
   {
      return add(msg);
   }

   const UInt64 begin(Timer::getTimeMs());
   const UInt64 end(begin + (unsigned int)(ms)); // !kh! ms should've been unsigned :(
   Lock lock(mMutex); (void)lock;

   while (mSize >= mMaxSize)
   {
      const UInt64 now(Timer::getTimeMs());
      if(now >= end)
      {
          return ;
      }

      unsigned int timeout((unsigned int)(end - now));
      bool signaled = mNotFull.wait(mMutex, timeout);
      if (!signaled)
      {
         return;
      }
   }
   mFifo.push_back(msg);
   mSize++;
   mNotEmpty.signal();
}

template <class Msg>
Msg*
Fifo<Msg> ::getNext()
{
   return static_cast<Msg*>(AbstractFifo::getNext());
}

template <class Msg>
Msg*
Fifo<Msg> ::getNext(int ms)
{
   return static_cast<Msg*>(AbstractFifo::getNext(ms));
}

#endif

