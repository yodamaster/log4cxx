/*
 * Copyright 2003,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <log4cxx/portability.h>

#if defined(LOG4CXX_HAVE_MS_THREAD)
#include <windows.h>
#endif

#include <log4cxx/helpers/thread.h>
#include <log4cxx/helpers/loglog.h>

using namespace log4cxx::helpers;

IMPLEMENT_LOG4CXX_OBJECT(Runnable)
IMPLEMENT_LOG4CXX_OBJECT(Thread)

struct Thread::Impl
{
	Impl(): thread(0) {};

	/** Thread descriptor */
#ifdef LOG4CXX_HAVE_PTHREAD
	pthread_t thread;
#elif defined(LOG4CXX_HAVE_MS_THREAD)
	void * thread;
#endif

};

#ifdef LOG4CXX_HAVE_PTHREAD
#include <pthread.h>
#include <unistd.h> // usleep
void * threadProc(void * arg)
{
//	LogLog::debug(_T("entering thread proc"));
	Thread * thread = (Thread *)arg;
	thread->run();
	thread->releaseRef();
	pthread_exit(0);
	return 0;
}
#elif defined(LOG4CXX_HAVE_MS_THREAD)
DWORD WINAPI threadProc(void * arg)
{
//	LogLog::debug(_T("entering thread proc"));
	Thread * thread = (Thread *)arg;
	thread->run();
	thread->releaseRef();
	return 0;
}
#else
#include <unistd.h> // usleep
#endif


Thread::Thread(): impl( new Impl )
{
	addRef();
}

Thread::Thread(RunnablePtr runnable) : runnable(runnable), impl( new Impl )
{
	addRef();
}

Thread::~Thread()
{
	// TODO: why don't we use Thread::join ?
	if (impl->thread != 0)
	{
#ifdef LOG4CXX_HAVE_PTHREAD
		::pthread_join(impl->thread, 0);
#elif defined(LOG4CXX_HAVE_MS_THREAD)
		::CloseHandle((HANDLE)impl->thread);
#endif
		LOGLOG_DEBUG(_T("Thread destroyed."));
	}
}

unsigned long Thread::getCurrentThreadId()
{
#ifdef LOG4CXX_HAVE_PTHREAD
	return (unsigned long)::pthread_self();
#elif defined(LOG4CXX_HAVE_MS_THREAD)
	return ::GetCurrentThreadId();
#endif
}

void Thread::start()
{
	parentMDCMap = MDC::getContext();
#ifdef LOG4CXX_HAVE_PTHREAD
//	LogLog::debug(_T("Thread::start"));
	if (::pthread_create(&impl->thread, NULL, threadProc, this) != 0)
	{
		throw ThreadException();
	}
#elif defined(LOG4CXX_HAVE_MS_THREAD)
	unsigned long threadId = 0;
	impl->thread =
		(void *)::CreateThread(NULL, 0, threadProc, this, 0, &threadId);
	if (impl->thread == 0)
	{
		throw ThreadException();
	}
#endif
}

void Thread::run()
{
	MDC::setContext(parentMDCMap);
	if (runnable != 0)
	{
		runnable->run();
	}
}

void Thread::join()
{
	bool bSuccess = true;
#ifdef LOG4CXX_HAVE_PTHREAD
	::pthread_join(impl->thread, 0);
#elif defined(LOG4CXX_HAVE_MS_THREAD)
	if (::WaitForSingleObject((HANDLE)impl->thread, INFINITE) != WAIT_OBJECT_0)
	{
		bSuccess = false;
	}

	::CloseHandle((HANDLE)impl->thread);
#endif

	impl->thread = 0;

	if (!bSuccess)
	{
		throw InterruptedException();
	}

	LOGLOG_DEBUG(_T("Thread ended."));
}

void Thread::sleep(long millis)
{
#ifdef LOG4CXX_HAVE_MS_THREAD
	::Sleep(millis);
#else
	::usleep(1000 * millis);
#endif
}

void Thread::setPriority(int newPriority)
{
	switch(newPriority)
	{
	case MIN_PRIORITY:
		break;
	case NORM_PRIORITY:
		break;
	case MAX_PRIORITY:
		break;
	}
}

#ifdef __GLIBCPP__
#include <bits/atomicity.h>
#else
#if defined(sparc) && defined(__SUNPRO_CC)
extern "C" long sparc_atomic_add_32 (volatile long* p, long val);

static void asm_code()
{
	asm(".align 8"); \
	asm(".global sparc_atomic_add_32"); \
	asm(".type sparc_atomic_add_32, #function"); \

	asm("sparc_atomic_add_32:");
	asm("    membar #Lookaside | #LoadLoad | #LoadStore | #StoreLoad");
	asm("    ld [%o0], %l0");       // l0 = *p;
	asm("    add %l0, %o1, %l2");   // l2 = l0 + val
	asm("    cas [%o0], %l0, %l2"); // if (*p = l0) swap([o0], l2);
	asm("    cmp %l0, %l2");        // does it match
	asm("    bne sparc_atomic_add_32");// if not try again
	asm("    nop");                 // delay slot filler
	asm("    add %l2, %o1, %o0");   // set return code
	asm("    membar #Lookaside | #LoadLoad | #LoadStore | #StoreLoad");
	asm("retl");
	asm("nop");
}
#endif
#endif

long Thread::InterlockedIncrement(volatile long * val)
{
#ifdef __GLIBCPP__
	return __exchange_and_add((volatile _Atomic_word *)val, 1 ) + 1;
#elif defined(__i386__)
	long ret;
	
	__asm__ __volatile__ ("lock; xaddl %0, %1"
			      : "=r" (ret), "=m" (*val)
			      : "0" (1), "m" (*val));

	return ret+1;
#elif defined(sparc) && defined(__SUNPRO_CC)
	sparc_atomic_add_32(val, 1);
	return *val;
#elif defined(LOG4CXX_HAVE_MS_THREAD)
#if LOG4CXX_LOG4CXX_HAVE_OLD_WIN32_INTERLOCKS	// MSDEV 6
	return ::InterlockedIncrement((long *)val);
#else
	return ::InterlockedIncrement(val);
#endif // _MSC_VER
	return *val + 1; // unsafe
#endif
}

long Thread::InterlockedDecrement(volatile long * val)
{
#ifdef __GLIBCPP__
	return __exchange_and_add((volatile _Atomic_word *)val, -1 ) - 1;
#elif defined(__i386__)
	long ret;
	
	__asm__ __volatile__ ("lock; xaddl %0, %1"
			      : "=r" (ret), "=m" (*val)
			      : "0" (-1), "m" (*val));

	return ret-1;
	
#elif defined(sparc) && defined(__SUNPRO_CC)
	sparc_atomic_add_32(val, -1);
	return *val;
#elif defined(LOG4CXX_HAVE_MS_THREAD)
#if LOG4CXX_LOG4CXX_HAVE_OLD_WIN32_INTERLOCKS	// MSDEV 6
	return ::InterlockedDecrement((long *)val);
#else
	return ::InterlockedDecrement(val);
#endif // _MSC_VER
	return *val - 1; // unsafe
#endif
}
