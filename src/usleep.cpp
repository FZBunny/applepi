/*****************************************************************************

    File: "usleep.cpp"

    Copyright (C) 2021, Bruce Ward

    This file is part of applepi, an apple2 emulator for Linux,
    especially aimed at the Raspberry Pi operating system.

    applepi is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    applepi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with applepi; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Or perhaps visit: http://www.gnu.org/licenses/licenses.html

*****************************************************************************/





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

