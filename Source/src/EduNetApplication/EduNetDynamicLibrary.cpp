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

#include "EduNetDynamicLibrary.h"
#include "EduNetCommon/EduNetLog.h"
#include <string>
#ifdef WIN32
#else
#include <dlfcn.h>
#endif

using namespace EduNet;
//-----------------------------------------------------------------------------
EduNetDynamicLibrary::EduNetDynamicLibrary ( void ) :
        m_pLibHandle ( NULL )
{
}
//-----------------------------------------------------------------------------
EduNetDynamicLibrary::~EduNetDynamicLibrary ( void )
{
    this->UnLoadLibrary();
}
//-----------------------------------------------------------------------------
bool EduNetDynamicLibrary::LoadLibrary ( const char* pszLibName )
{
    if ( NULL != this->m_pLibHandle )
    {
        return false;
    }
#ifdef WIN32
    this->m_pLibHandle = LoadLibrary ( pszLibName );
#else
    std::string kName ( "./" );
    kName += pszLibName;
    std::ostringstream message;

    kName += ".so";
    this->m_pLibHandle = dlopen ( kName.c_str(), RTLD_LAZY );
#endif

    if ( NULL != this->m_pLibHandle )
    {
#ifdef ET_DEBUG
        std::ostringstream message;
        message << "Open dynamic library ";
        message << '"' << pszLibName << '"' << std::ends;
        Log::printMessage ( message );
#endif
    }
    else
    {
        std::ostringstream message;
        message << "Failed to open dynamic library ";
        message << '"' << pszLibName << '"' << std::ends;
        Log::printError ( message );
    }

    return NULL != this->m_pLibHandle;
}
//-----------------------------------------------------------------------------
bool EduNetDynamicLibrary::UnLoadLibrary ( void )
{
    bool bResult ( false );
    if ( NULL != this->m_pLibHandle )
    {
#ifdef WIN32
        bResult = 1 == FreeLibrary ( ( HINSTANCE ) this->m_pLibHandle );
#else
        bResult = 0 == dlclose ( this->m_pLibHandle );
#endif
        this->m_pLibHandle = NULL;
    }
    return bResult;
}


