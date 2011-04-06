#pragma once
#ifndef __MGFWINDOWS_H__
#define __MGFWINDOWS_H__

#if defined(WIN32) || defined(_WIN32)

// # include <Psapi.h>
// # pragma comment( lib, "Psapi.lib" )

//-----------------------------------------------------------------------------
//! \brief windows include never ever include <windows.h> directly !!!
//-----------------------------------------------------------------------------

#  define __MGF_WINDOWS_SPEEDUPS__ 1
// some windows speedups
#  if __MGF_WINDOWS_SPEEDUPS__
#	  define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#	  define WIN32_EXTRA_LEAN
#	  define NOGDICAPMASKS
#	  define OEMRESOURCE
#	  define NOATOM
#    ifndef __MGF_WINDOWS_USECLIPBOARD__
#      define NOCLIPBOARD
#    endif
//#	  define NOCTLMGR // windows utility functions like DialogBox, 
#	  define NOMEMMGR
#	  define NOMETAFILE
#	  define NOOPENFILE
#	  define NOSERVICE
#	  define NOSOUND
#	  define NOCOMM
#	  define NOKANJI
#	  define NOHELP
#	  define NOPROFILER
#	  define NODEFERWINDOWPOS
#	  define NOMCX
#     define NONLS
#     define NOMB
#  endif // __MGF_WINDOWS_SPEEDUPS__

// Windows Header Files:
#  include <windows.h>

#  endif

#endif //  __MGFWINDOWS_H__