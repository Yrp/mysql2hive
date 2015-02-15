#ifndef RESIP_ABSTRACTFIFO_HPP
#define RESIP_ABSTRACTFIFO_HPP

#include <deque>

#include "mutex.hpp"
#include "condition.hpp"
#include "lock.hpp"
/** @brief rutil is extend of base library
 *
 * Exclude base library in util, every tools save
 * in this namespace.
 */
/// for statistics gathering
class FifoStatsInterface
{
   public:
      virtual ~FifoStatsInterface() {}
      virtual size_t getCountDepth() const = 0;
      virtual time_t getTimeDepth() const = 0;
};

/**
   @brief The base class from which various templated Fifo classes are derived.

   (aka template hoist)
   AbstractFifo's get operations are all threadsafe; AbstractFifo does not
   define any put operations (these are defined in subclasses).
**/
class AbstractFifo : public FifoStatsInterface
{
   public:
	  /**
	   * Constructor :
	   * @param maxSize (int value to specify a max number of messages to keep)
	   **/
      AbstractFifo(unsigned long maxSize);
      virtual ~AbstractFifo();

      /**
       * @retval bool (Check if the queue of messages is empty ?)
       **/
      bool empty() const;

      /** Get the current size of the fifo. Note you should not use this function
       *  to determine whether a call to getNext() will block or not.
       *  Use messageAvailable() instead.
       */
      unsigned int size() const;

      /** @retval true (is a message is available ?)
       */
      bool messageAvailable() const;

      /// defaults to zero, overridden by TimeLimitFifo<T>
      virtual time_t timeDepth() const;

      /// remove all elements in the queue (or not)
      virtual void clear() {};

      virtual size_t getCountDepth() const;
      virtual time_t getTimeDepth() const;

   protected:
      /** Returns the first message available. It will wait if no
       *  messages are available. If a signal interrupts the wait,
       *  it will retry the wait. Signals can therefore not be caught
       *  via getNext. If you need to detect a signal, use block
       *  prior to calling getNext.
       */
      void* getNext();


      /** Returns the next message available. Will wait up to
       *  ms milliseconds if no information is available. If
       *  the specified time passes or a signal interrupts the
       *  wait, this method returns 0. This interface provides
       *  no mechanism to distinguish between timeout and
       *  interrupt.
       */
      void* getNext(int ms);

      enum {NoSize = 0UL -1};

      std::deque<void*> mFifo;
      unsigned long mSize;
      unsigned long mMaxSize;

      mutable Mutex mMutex;
      Condition mNotEmpty;
      Condition mNotFull;

   private:
      // no value semantics
      AbstractFifo(const AbstractFifo&);
      AbstractFifo& operator=(const AbstractFifo&);
};

#endif

