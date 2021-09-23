



#include <QtCore/qglobal.h>

#ifdef __WXMSW__

#include <chrono>
#include <thread>
#include <windows.h>


quint64 microseconds (void)
{
    LARGE_INTEGER performanceCount ;
    QueryPerformanceCounter (&performanceCount) ;
    return performanceCount.QuadPart ;
}

//  This usleep stolen from "latsa", https://gist.github.com/ngryman/6482577

void usleep(unsigned int usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * (__int64)usec);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}


void quickSleep (quint64 uSecs)
{
// printf ("*** quickSleep not implemented for Windows yet ***\n") ;
}


#else      //   Must be Linux...

#include <time.h>

quint64 microseconds (void)   // This function is good for 584,000+ years...
{
    struct timespec ts ;
    clock_gettime (CLOCK_REALTIME, &ts) ;
    quint64 uSecs = ts.tv_sec*1e6 + ts.tv_nsec/1000 ;
    return uSecs ;
}


// Sleep for 'uSecs' microseconds, while ignoring signals.

void quickSleep (quint64 uSecs)
{
    struct timespec tReq, tRem ;
    tReq.tv_sec = 0 ;
    tReq.tv_nsec = uSecs * 1000 ;

    while (true) {  
        int ret = nanosleep (&tReq, &tRem) ;  // (nanosleep returns -1 & puts remaining time in tRem if interrupted)
        if (ret == 0) break ;
        tReq = tRem ;
    }
}


#endif

