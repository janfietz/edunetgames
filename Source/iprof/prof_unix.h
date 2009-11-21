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
		  __asm {
			  rdtsc;
			  mov    ebx, result
			  mov    [ebx], eax
			  mov    [ebx+4], edx
		  }
#else
      struct timespec t; 
      *result = 0;
      if( clock_gettime( CLOCK_MONOTONIC, &t) == 0 ) {
	  *result = (Prof_Int64)t.tv_sec * 1000000000 +  t.tv_nsec;
      } 
            
#endif //__APPLE__
      }

#endif
