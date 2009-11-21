#include<windows.h>
#include "time.h"

bool hrtimer = false;

__int64 my_time(void)
{
    static __int64 freq = (-1);
    __int64 usec_time;

    if( freq == (-1) ){
//                freq = ( QueryPerformanceFrequency( (LARGE_INTEGER *)&freq ) &&
//                        (freq == (1193182) || freq == (3579545) ) )
//                        ? freq : 0;
                /*freq = */
                if(!QueryPerformanceFrequency( (LARGE_INTEGER *)&freq )) freq=0;
    }

    if( freq != 0 ){
            /* Microsecond resolution */
            QueryPerformanceCounter( (LARGE_INTEGER *)&usec_time );
            hrtimer = true;
            return ( usec_time * 1000000 ) / freq;
    } else {
        /* Fallback on GetTickCount() which has a milisecond resolution
        * (actually, best case is about 5-10 ms resolution)
        * GetTickCount() only returns a DWORD thus will wrap after * about 49.7 days ! */
        hrtimer = false;
        return GetTickCount()*1000;     // this
        //return timeGetTime()*1000;    // or this
    }
}

