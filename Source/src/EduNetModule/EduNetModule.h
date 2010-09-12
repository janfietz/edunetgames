#ifndef EDUNET_MODULE_H
#define EDUNET_MODULE_H
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


//-----------------------------------------------------------------------------
#include "EduNetCore/EduNetCore.h"
#include "EduNetDynamicLibrary.h"
#include <boost/shared_ptr.hpp>

//-----------------------------------------------------------------------------
// DLL SUPPORT
//-----------------------------------------------------------------------------
#if defined WIN32
	#define ET_DLL_EXPORT __declspec(dllexport)
// we do not want to support static linking
//	#define ET_DLL_IMPORT __declspec(dllimport)
#else
	#define ET_DLL_EXPORT
// we do not want to support static linking
//	#define ET_DLL_IMPORT
#endif

#define ET_MODULE_STR(n) #n

#define ET_STD_ENTRY_FUNC etModuleQueryEntry

#define ET_STD_ENTRY_FUNC_STR ET_MODULE_STR(ET_STD_ENTRY_FUNC)

//-----------------------------------------------------------------------------
namespace EduNet	{
	class ModuleEntry;
}

//-----------------------------------------------------------------------------
#define ET_DECLARE_MODULE_ENTRYFUNC_S(fnc)\
	EduNet::ModuleEntry* fnc( void )
//-----------------------------------------------------------------------------
#define ET_MODULE_ENTRYFUNC_EXPORT(fnc) \
	extern "C" ET_DLL_EXPORT \
	ET_DECLARE_MODULE_ENTRYFUNC_S(fnc)

//-----------------------------------------------------------------------------
#define ET_IMPLEMENT_MODULE_ENTRYFUNC(fnc, entryClass)\
	ET_MODULE_ENTRYFUNC_EXPORT(ET_STD_ENTRY_FUNC){ \
		static entryClass kEntry; \
		return &kEntry; }

typedef ET_DECLARE_MODULE_ENTRYFUNC_S(ModuleEntryFunc);

typedef struct OpenSteerUTData_t OpenSteerUTData;

class EduNetPluginFactory;

namespace EduNet	{

//-----------------------------------------------------------------------------
class ModuleEntry
{
public:
	virtual const char* getName( void ) const ET_ABSTRACT;
	virtual const char* getAbout( void ) const ET_ABSTRACT;

	virtual void setOpenSteerUTData( OpenSteerUTData* ) const ET_ABSTRACT;

	virtual EduNetPluginFactory* createPluginFactory( void ) const ET_ABSTRACT;
};

//-----------------------------------------------------------------------------
class RawModule
{
public:
	RawModule( void );
	virtual ~RawModule( void );

	bool load(const char* pszLibName);
	ModuleEntry* accessEntry( void ) const
	{
		return this->m_pEntry;
	};

private:

	ModuleEntryFunc* queryEntryFunction( void );
	bool queryEntry( void );

	EduNet::DynamicLibraryPtr m_spLib;
	ModuleEntry* m_pEntry;
};

typedef boost::shared_ptr<RawModule>  RawModulePtr;
typedef std::vector<RawModulePtr> RawModules;

}

#endif //EDUNET_MODULE_H