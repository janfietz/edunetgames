#ifndef __EDUNETCONFIG_H__
#define __EDUNETCONFIG_H__

//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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

// application name macro
#ifndef EDUNET_APPNAME
#define EDUNET_APPNAME EduNet::Options::getAppName()
#endif

// profiling
#ifndef WIN32
#if __APPLE__ && __MACH__
#define EDUNET_HAVE_PROFILE 0
#else
#define EDUNET_HAVE_PROFILE 0
#endif
#endif
#ifndef EDUNET_HAVE_PROFILE
#define EDUNET_HAVE_PROFILE 0
#endif


// opensteer
#ifndef EDUNET_HAVE_OPENSTEER
#define EDUNET_HAVE_OPENSTEER 1
#endif

// boost lib
#ifndef EDUNET_HAVE_BOOST
#define EDUNET_HAVE_BOOST 1
#endif

#if EDUNET_SHOW_CONFIG
#pragma message( "-------------------------------------------------------------------------------" )
#pragma message( " Build Configuration" )
#pragma message( "-------------------------------------------------------------------------------" )

#if EDUNET_HAVE_OPENSTEER
#pragma message( " - EDUNET_HAVE_OPENSTEER" )
#endif

#if EDUNET_HAVE_BOOST
#pragma message( " - EDUNET_HAVE_BOOST" )
#endif

#pragma message( "-------------------------------------------------------------------------------" )
#endif // EDUNET_SHOW_CONFIG


// windows

//-----------------------------------------------------------------------------
#ifdef WIN32
#if EDUNET_SHOW_CONFIG
#pragma message( " - windows build" )
#endif
#define __ET_WINDOWS_SPEEDUPS__ 1
// some windows speedups
#	if __ET_WINDOWS_SPEEDUPS__

#		define WIN32_LEAN_AND_MEAN
#		define WIN32_EXTRA_LEAN

#		define NOGDICAPMASKS
#		define OEMRESOURCE
#		define NOATOM
#ifndef __ET_WINDOWS_USECLIPBOARD__
#		define NOCLIPBOARD
#endif
//#		define NOCTLMGR // windows utility functions like DialogBox,
#		define NOMEMMGR
#		define NOMETAFILE
#		define NOOPENFILE
#		define NOSERVICE
#		define NOSOUND
#		define NOCOMM
#		define NOKANJI
#		define NOHELP
#		define NOPROFILER
#		define NODEFERWINDOWPOS
#		define NOMCX
// #define NONLS
// #define NOMB
# define WIN32_LEAN_AND_MEAN
# define WIN32_EXTRA_LEAN

#	endif // __ET_WINDOWS_SPEEDUPS__


# ifndef _WIN32_WINNT
#  define _WIN32_WINNT 0x0502 // Windows XP SP1
# endif
//# endif


#if _MSC_VER >= 1400

#endif


#ifdef ET_DEBUG
#include <crtdbg.h>
#endif

#endif  // WIN32


#endif // __EDUNETCONFIG_H__
