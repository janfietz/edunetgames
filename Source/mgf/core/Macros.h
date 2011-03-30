#ifndef __MGFMACROS_H__
#define __MGFMACROS_H__

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
// * Neither the name of RockNet nor the names of its contributors
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

#if defined(_MSC_VER)
#ifdef _DEBUG
#define MGF_DEBUG
#endif
#else
#ifndef NDEBUG
#define MGF_DEBUG
#endif
#endif

//-----------------------------------------------------------------------------
#ifdef MGF_DEBUG
#  define MGF_NEW new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
#else
#  define MGF_NEW new
#endif
#define MGF_DELETE delete
//-----------------------------------------------------------------------------

// define __cdecl for non-Microsoft compilers
#if ( !defined(_MSC_VER) && !defined(__cdecl) )
#  define __cdecl
#endif

#define MGF_CRTDECL __cdecl

//-----------------------------------------------------------------------------
#define MGF_INLINE inline

#if defined(_MSC_VER)
#  define MGF_FORCEINLINE __forceinline
#else
#  define MGF_FORCEINLINE inline
#endif
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
#ifndef _MGF_HAVE_MS_EXTENSIONS
#if defined(_MSC_VER)
#    define _MGF_HAVE_MS_EXTENSIONS 1
#  else
#    define _MGF_HAVE_MS_EXTENSIONS 0
#  endif
#endif	//_MGF_HAVE_MS_EXTENSIONS

// The following macros are used together with virtual functions.
//	- MGF_OVERRIDE: means that the function MUST override a base class function
//	- MGF_SEALED: this function cannot be overridden in derived classes.
//	- MGF_ABSTRACT: this function is abstract and must be overridden in derived classes
//	- MGF_VIRTUAL_DECLARE: This virtual function is declared here the first time; but not abstract.
//						  Just to make it easier to see which functions are declared in a class.

#if _MGF_HAVE_MS_EXTENSIONS
#	define MGF_OVERRIDE override
#	define MGF_SEALED sealed
#	define MGF_ABSTRACT abstract
#else	//_MGF_HAVE_MS_EXTENSIONS
#	define MGF_OVERRIDE
#	define MGF_SEALED
#	define MGF_ABSTRACT = 0
#endif	//_MGF_HAVE_MS_EXTENSIONS


#define MGF_DECLARE_BASE( classname ) typedef classname BaseClass;

//-----------------------------------------------------------------------------
//! implement an empty private copy constructor and a private assignment
#define MGF_IMPLEMENT_CLASS_NO_COPY( classname ) private:\
	classname( const classname& );\
	classname& operator=( const classname& );

#include <assert.h>
#define MGF_ASSERT assert

//-----------------------------------------------------------------------------
//! implement a copy constructor and a assignment
#define MGF_IMPLEMENT_CLASS_COPY( classname ) \
	classname( const classname& kOther ){ this->CopyFrom( kOther ); }\
	classname& operator=( const classname& kOther ){ MGF_ASSERT( 0 != this ); this->CopyFrom( kOther ); return *this;}


#endif //  __MGFMACROS_H__
