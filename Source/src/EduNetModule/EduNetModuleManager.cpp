//-----------------------------------------------------------------------------
// Copyright (c) Jan Fietz, Cyrus Preuss
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
// LOSS OF USE, DATA, OR PROFITS; OR BUSIN;ESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "EduNetModuleManager.h"
#include<boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;

// note: set to 1 to debug loadModulesFromDirectory
#define ET_DEBUG_LOADMODULES 0

//-----------------------------------------------------------------------------
const EduNetModuleManager& EduNetModuleManager::operator<<( const char* pszString ) const
{
#if ET_DEBUG_LOADMODULES
	std::cout << pszString;
#else
#endif
	return (*this);
}

//-----------------------------------------------------------------------------
const EduNetModuleManager& EduNetModuleManager::operator<<( unsigned long ul ) const
{
#if ET_DEBUG_LOADMODULES
	std::cout << ul;
#else
#endif
	return (*this);
}


//-----------------------------------------------------------------------------
void EduNetModuleManager::loadModulesFromDirectory(const char* pszDirectory)
{
	bfs::path p(pszDirectory);
	unsigned long fc = 0, dc = 0;
	if (!bfs::exists(p))
		(*this) << "\nFile Not Found:" << p.native_file_string().c_str() << "\n";
	else if (!bfs::is_directory(p))
		(*this) << "\nFound: " << p.native_file_string().c_str() << "\n";
	(*this) << "In directory:" << p.native_file_string().c_str() << "\n";
	bfs::directory_iterator iter(p), end_iter;
	for (; iter != end_iter; ++iter)
	{
		try
		{
			if (bfs::is_directory(*iter))
			{
				++dc;
				(*this) << iter->leaf().c_str() << "[Directory]\n";
			} 
			else
			{
				++fc;
				addModuleFromFile(iter->leaf().c_str());
				(*this) << iter->leaf().c_str() << "\n";
			}
		} 
		catch (const std::exception & ex)
		{
			(*this) << iter->leaf().c_str() << ": " << ex.what() << "\n";
		}
		(*this) << fc << " " << dc << "\n";
	} //for
}

//-----------------------------------------------------------------------------
bool EduNetModuleManager::addModuleFromFile(const char* pszFileName)
{
	EduNetRawModulePtr spNewLib(ET_NEW EduNetRawModule() );
	EduNetRawModule* pkNewmodule = spNewLib.get();

	bool bResult = pkNewmodule->load ( pszFileName );
	if ( true == bResult )
	{
		this->m_modules.push_back ( spNewLib );
	}
	return bResult;
}

//-----------------------------------------------------------------------------
void EduNetModuleManager::unloadAll( void )
{
	this->m_modules.clear();
}

//-----------------------------------------------------------------------------
void EduNetModuleManager::unloadModule( const char* pszName )
{

}

