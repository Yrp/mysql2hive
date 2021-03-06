
#if !defined(RESIP_LOCK_HXX)
#define RESIP_LOCK_HXX  

#include "lockable.hpp"

enum LockType
{
   VOCAL_LOCK = 0,
   VOCAL_READLOCK,
   VOCAL_WRITELOCK
};

/**
  @brief A convenience class to lock a Lockable (such as a Mutex) on object 
  creation, and unlock on destruction. (ie, a scoped lock)

  @see Mutex
*/
class Lock
{
   public:
     /**
	  @param	Lockable&	The object to lock
	  @param	LockType	one of VOCAL_LOCK, VOCAL_READLOCK, VOCAL_WRITELOCK
	*/
      Lock(Lockable &, LockType = VOCAL_LOCK);
      virtual ~Lock();

   private:
      Lockable&   myLockable;
};

class ReadLock : public Lock
{
   public:
      ReadLock(Lockable &);
};

class WriteLock : public Lock
{
   public:
      WriteLock(Lockable &);
};


#endif

