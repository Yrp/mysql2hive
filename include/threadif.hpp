#if !defined(RESIP_THREADIF_HXX)
#define RESIP_THREADIF_HXX

#include <pthread.h>
#include "mutex.hpp"
#include "condition.hpp"


class ThreadIf
{
public:
	typedef pthread_t Id;
	
	ThreadIf();
	virtual ~ThreadIf();
	
	/// runs the code in thread() .  Returns immediately
	virtual void run();
	/// joins to the thread running thread()
	void join();
	/** guarantees resources consumed by thread are released when thread terminates
	 * after this join can no-longer be used
	 */
	void detach();
	
	/// request the thread running thread() to return, by setting  mShutdown
	virtual void shutdown();

	/** waits for waitMs, or stops waiting and returns true if shutdown was
	 * called
	 */
	virtual bool waitForShutdown(int ms) const;
	
	/// returns true if the thread has been asked to shutdown or not running
	bool isShutdown() const;
	
	static Id selfId();
	/** thread is a virtual method.  Users should derive and define
	thread() such that it returns when isShutdown() is true.
	*/
	virtual void thread() = 0;
	
private:
	/// Suppress copying
	ThreadIf(const ThreadIf &);
	const ThreadIf & operator=(const ThreadIf &);
	
protected:
	Id mId;
	
	bool mShutdown;
	mutable Mutex mShutdownMutex;
	mutable Condition mShutdownCondition;
};


#endif

