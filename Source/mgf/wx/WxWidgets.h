#pragma once
#ifndef __MGF_WXWIDGETS_H__
#define __MGF_WXWIDGETS_H__

// note: we do not want to include the winsock api here
#ifdef _WIN32
#  define _WINSOCKAPI_
#  include <mgf/core/os/Windows.h>
#endif

#pragma warning( push )
#pragma warning( disable : 4996 ) // This function or variable may be unsafe. Consider using strcpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#  include <wx/wx.h>
#endif
#pragma warning( pop )

#pragma warning( disable : 4251 ) // xxx needs to have dll-interface to be used by clients of class 'xxx'

#endif //  __MGF_WXWIDGETS_H__