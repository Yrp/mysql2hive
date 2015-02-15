
#include "lock.hpp"

Lock::Lock(Lockable & lockable, LockType lockType)
   : myLockable(lockable)
{
   switch ( lockType )
    {
       case VOCAL_READLOCK:
       {
          myLockable.readlock();
          break;
       }
	    
       case VOCAL_WRITELOCK:
       {
          myLockable.writelock();
          break;
       }
       
       default:
       {
          myLockable.lock();
          break;
       }
    }
}

Lock::~Lock()
{
    myLockable.unlock();
}

ReadLock::ReadLock(Lockable & lockable)
   : Lock(lockable, VOCAL_READLOCK)
{
}

WriteLock::WriteLock(Lockable & lockable)
   : Lock(lockable, VOCAL_WRITELOCK)
{
}


