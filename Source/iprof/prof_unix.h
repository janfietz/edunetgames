#ifndef Prof_INC_PROF_UNIX_H
#define Prof_INC_PROF_UNIX_H

#if (defined(__GNUC__)  || defined(__GCCXML__) || defined(__SNC__) || defined(__APPLE__) )
#include <stdint.h>
#endif

#ifndef __APPLE__
#include <time.h>
#endif

typedef int64_t Prof_Int64;

#ifdef __cplusplus
  inline
#elif _MSC_VER >= 1200
  __forceinline
#else
  static
#endif
      void Prof_get_timestamp(Prof_Int64 *result)
      {
#ifdef __APPLE__	
		  *result = 0;
#else
	struct timespec t;
	*result = 0;
	if(0 == clock_gettime(CLOCK_MONOTONIC, &t))
	{
	  *result = t.tv_sec * 1000000000 + t.tv_nsec;
	}
#endif
      }

#endif
