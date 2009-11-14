#ifndef Prof_INC_PROF_UNIX_H
#define Prof_INC_PROF_UNIX_H

#if (defined(__GNUC__)  || defined(__GCCXML__) || defined(__SNC__) || defined(__APPLE__) )
#include <stdint.h>
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
         *result = 0;
      }

#endif
