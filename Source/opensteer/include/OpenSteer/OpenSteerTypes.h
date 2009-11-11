#ifndef __OPENSTEERTYPES_H__
#define __OPENSTEERTYPES_H__

/*
 *  OpenSteerTypes.h
 *  OpenSteer
 *
 *  Created by Cyrus Preuss on 11/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __OSNATIVE_TYPES_H__
#define __OSNATIVE_TYPES_H__

#if (defined(__GNUC__)  || defined(__GCCXML__) || defined(__SNC__))
#include <stdint.h>
#endif

#if !defined(_STDINT_H) && !defined(_SN_STDINT_H) && !defined(_SYS_STDINT_H_)
typedef unsigned char         uint8_t;
typedef unsigned short        uint16_t;
typedef unsigned int          uint32_t;
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
#if defined(_MSC_VER) && _MSC_VER < 1300
typedef unsigned __int64    uint64_t;
typedef signed __int64   	int64_t;
#else
typedef unsigned long long int    uint64_t;
typedef signed long long   	int64_t;
#endif
#endif


#endif // __OSNATIVE_TYPES_H__



typedef float osScalar;

#endif //  __OPENSTEERTYPES_H__
