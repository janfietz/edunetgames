#include "prof_time.h"

#include <sys/time.h> 

static int baseRealTimeSec = 0;
static int baseRealTimeUsec = 0;

double Prof_get_time(void)
{
#ifndef  __APPLE__
    struct timeval t;
#else
    _STRUCT_TIMEVAL t;
#endif
	
    if (gettimeofday (&t, 0) != 0) return 0.0;
	
    // ensure the base time is recorded once after launch
    if (baseRealTimeSec == 0)
    { 
        baseRealTimeSec = t.tv_sec;
        baseRealTimeUsec = t.tv_usec;
    }
	
    // real "wall clock" time since launch
    return (( t.tv_sec  - baseRealTimeSec) +
            ((t.tv_usec - baseRealTimeUsec) / 1000000.0f));

}
