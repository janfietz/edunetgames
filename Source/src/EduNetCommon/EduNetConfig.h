#ifndef __EDUNETCONFIG_H__
#define __EDUNETCONFIG_H__

// application name macro
#ifndef EDUNET_APPNAME
#define EDUNET_APPNAME EduNetOptions::getAppName()
#endif

// raknet
#ifndef EDUNET_HAVE_RAKNET
#define EDUNET_HAVE_RAKNET 1
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

#if EDUNET_HAVE_RAKNET
#pragma message( " - EDUNET_HAVE_RAKNET" )
#endif

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
//#include <windows.h>
#endif  // WIN32


#endif // __EDUNETCONFIG_H__
