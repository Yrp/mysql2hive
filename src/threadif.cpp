
#include <cassert>
#include <iostream>
#include "threadif.hpp"
#include "lock.hpp"

extern "C"
{
static void* threadWrapper( void* threadParm )
{
	assert( threadParm );
	ThreadIf* t = static_cast < ThreadIf* > ( threadParm );
	
	assert( t );
	t->thread();
	
	return 0;
}
}

ThreadIf::ThreadIf() : mId(0) , mShutdown(false), mShutdownMutex()
{}

ThreadIf::~ThreadIf()
{
	join();
}

void ThreadIf::run()
{
	assert(mId == 0);
	// spawn the thread
	if ( int retval = pthread_create( &mId, 0, threadWrapper, this) )
	{
		std::cerr << "Failed to spawn thread: " << retval << std::endl;
		assert(0); 
	}
}

void ThreadIf::join()
{
	if (mId == 0)
	{
		return;
	}
	
	void* stat;
	if (mId != pthread_self())
	{
		int r = pthread_join( mId , &stat );
		if ( r != 0 )
		{
			std::cerr << "Internal error: pthread_join() returned " << r << std::endl;
			assert(0);
		}
	}
	
	mId = 0;
}

void ThreadIf::detach()
{
	pthread_detach(mId);
	mId = 0;
}

ThreadIf::Id ThreadIf::selfId()
{
	return pthread_self();
}

void ThreadIf::shutdown()
{
	Lock lock(mShutdownMutex);
	if (!mShutdown)
	{
		mShutdown = true;
		mShutdownCondition.signal();
	}
}

bool ThreadIf::waitForShutdown(int ms) const
{
	Lock lock(mShutdownMutex);
	if(!mShutdown)
	{
		mShutdownCondition.wait(mShutdownMutex, ms);
	}
	return mShutdown;
}

bool ThreadIf::isShutdown() const
{
	Lock lock(mShutdownMutex);
	(void)lock;
	return ( mShutdown );
}
