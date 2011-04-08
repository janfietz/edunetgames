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
#include "EduNetModule.h"

namespace EduNet	{

//-----------------------------------------------------------------------------
RawModule::RawModule( void ):
	m_pEntry(NULL),
	m_pLib(NULL)
{

}

//-----------------------------------------------------------------------------
RawModule::~RawModule( void )
{
	// TODO: latest point to unload modules again
	unload();
}

//-----------------------------------------------------------------------------
bool RawModule::load(const char* pszLibName)
{
	//EduNet::DynamicLibraryPtr spNewLib( ET_NEW EduNet::DynamicLibrary() );
	EduNet::DynamicLibrary* pkNewLib = ET_NEW EduNet::DynamicLibrary();
	bool bLoaded = pkNewLib->loadLib( pszLibName ) ;
	if( true == bLoaded )
	{
		this->m_pLib = pkNewLib;
		if( true == this->queryEntry() )
		{
			// log success			
		}
		else
		{
			// release lib again
			unload();
		}
	}else
	{
		ET_SAFE_DELETE(pkNewLib);
	}

	return (NULL!= this->m_pEntry);	
}

//-----------------------------------------------------------------------------
bool RawModule::unload()
{
	bool bReturn(false);
	this->m_pEntry = NULL;
	EduNet::DynamicLibrary* pkNewLib = this->m_pLib;
	if (NULL != pkNewLib)
	{
		bReturn = pkNewLib->unloadLib();
	}

	ET_SAFE_DELETE(m_pLib);
	
	return bReturn;
}


//-----------------------------------------------------------------------------
bool RawModule::queryEntry( void )
{	
	ModuleEntryFunc* pkEntryFunc = this->queryEntryFunction();
	if (NULL != pkEntryFunc)
	{
		// call the entry function
		this->m_pEntry = (*pkEntryFunc)();
	}
	return (NULL != this->m_pEntry);
}

//-----------------------------------------------------------------------------
ModuleEntryFunc* RawModule::queryEntryFunction( void )
{	
	ModuleEntryFunc* pkEntryFunc(NULL);
	EduNet::DynamicLibrary* pkLib = this->m_pLib;
	if( NULL != pkLib )
	{
		void* pkFunc = pkLib->findSymbol( "etModuleQueryEntry" );
		if (NULL != pkFunc)
		{
			pkEntryFunc = (ModuleEntryFunc*)pkFunc;
		}
	}
	return pkEntryFunc;
}

} 
