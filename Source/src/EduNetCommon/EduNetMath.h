#ifndef __EDUNETMATH_H__
#define __EDUNETMATH_H__

//-----------------------------------------------------------------------------
// Copyright (c) 2009, Jan Fietz, Cyrus Preuss
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include <math.h>
#include <assert.h>

#include "EduNetCore/EduNetMacros.h"

//-----------------------------------------------------------------------------
template <class T>
EF_FORCEINLINE
const T& etMin(const T& a, const T& b) 
{
	return a < b ? a : b ;
}

//-----------------------------------------------------------------------------
template <class T>
EF_FORCEINLINE
const T& etMax(const T& a, const T& b) 
{
	return  a > b ? a : b;
}

//-----------------------------------------------------------------------------
template <class T>
EF_FORCEINLINE
T etClamp(const T& fValue, const T& fMin, const T& fMax) {
	assert( fMin <= fMax );
	T fValueOut = etMax( fValue, fMin );
	fValueOut = etMin( fValueOut, fMax );
	return fValueOut;
}

//-----------------------------------------------------------------------------
template <class T>
EF_FORCEINLINE
T etClampSave(const T& fValue, const T& fMin, const T& fMax) {
	T _fMin = etMin( fMin, fMax );
	T _fMax = etMax( fMin, fMax );
	T fValueOut = etMax( fValue, _fMin );
	fValueOut = etMin( fValueOut, _fMax );
	return fValueOut;
}

//-----------------------------------------------------------------------------
template <class T>
EF_FORCEINLINE
T etSign(const T& fValue)
{
	return ( fValue > T( 0.0 ) ? T( +1.0 ) : ( fValue < T( 0.0 ) ? T( -1.0 ) : T( 0.0 ) ) );
}	

//-----------------------------------------------------------------------------
template <class T>
EF_FORCEINLINE
T etInterval(const T& fMin, const T& fMax)
{
	assert( fMin <= fMax );
	return ( fMax - fMin );
}	




#endif // __EDUNETMATH_H__