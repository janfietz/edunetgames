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
#include "EduNetCommon/EduNetOptions.h"
#include "OpenSteerUT/OpenSteerUT.h"

#include <boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;

// note: set to 1 to debug loadModulesFromDirectory
#define ET_DEBUG_LOADMODULES 0

namespace EduNet	{


//-----------------------------------------------------------------------------
ModuleManager::ModuleManager()
{
#ifdef WIN32
	int bytes = ::GetModuleFileNameA( NULL, m_pszCurrentModuleFileName, sizeof(m_pszCurrentModuleFileName) );
	if(bytes == 0)
	{
		m_pszCurrentModuleFileName[0] = '\0';
	}
#else
	assert( true == false );
	// not tested ...
	char szTmp[32];
	sprintf(szTmp, "/proc/%d/exe", getpid());
	int bytes = MIN(readlink(szTmp, m_pszCurrentModuleFileName, len), len - 1);
	if(bytes >= 0)
		m_pszCurrentModuleFileName[bytes] = '\0';
	else
		m_pszCurrentModuleFileName[0] = '\0';
#endif
	if( m_pszCurrentModuleFileName[0] != 0 )
	{
		// determine the module path
		{
			strcpy_s( m_pszCurrentModulePath, EMaxPath, m_pszCurrentModuleFileName );
			size_t len = strlen( m_pszCurrentModulePath );
			for( size_t i = len; i >= 0; --i )
			{
				if( ( m_pszCurrentModulePath[i] != '\\' ) && ( m_pszCurrentModulePath[i] != '/' ) )
				{
					m_pszCurrentModulePath[i] = '\0';
				}
				else
				{
					break;
				}
			}
		}

		this->m_modules.reserve(256);

		// determine the module type
		this->queryModuleRuntimeTypeFromFileName( this->m_pszCurrentModuleFileName, this->m_moduleRuntimeType );
	}
}

//-----------------------------------------------------------------------------
ModuleManager::~ModuleManager()
{
	this->unloadAll();
}

//-----------------------------------------------------------------------------
void ModuleManager::queryModuleRuntimeTypeFromFileName( const char* pszFileName, enString_t& kModuleType )
{
	if( ( NULL == pszFileName ) || ( 0 == pszFileName[0] ) )
	{
		return;
	}
	// determine the module type
	enString_t tempString(pszFileName);
	size_t dotPos = tempString.rfind('.');
	size_t underscorePos = tempString.rfind('_');

	if( (dotPos != enString_t::npos) && (underscorePos != enString_t::npos) )
	{
		kModuleType = tempString.substr( underscorePos, dotPos - underscorePos );
		::tolower( kModuleType );
	}
}

//-----------------------------------------------------------------------------
const ModuleManager& ModuleManager::operator<<( const char* pszString ) const
{
#if ET_DEBUG_LOADMODULES
	std::cout << pszString;
#else
#endif
	return (*this);
}

//-----------------------------------------------------------------------------
const ModuleManager& ModuleManager::operator<<( unsigned long ul ) const
{
#if ET_DEBUG_LOADMODULES
	std::cout << ul;
#else
#endif
	return (*this);
}

//-----------------------------------------------------------------------------
const char* ModuleManager::getCurrentModuleFileName( void ) const
{
	return m_pszCurrentModuleFileName;
}

//-----------------------------------------------------------------------------
const char* ModuleManager::getCurrentModulePath( void ) const
{
	return m_pszCurrentModulePath;
}

//-----------------------------------------------------------------------------
void ModuleManager::loadModulesFromDirectory(const char* pszDirectory)
{
	bfs::path p(pszDirectory);
	unsigned long fc = 0, dc = 0;
	if (!bfs::exists(p))
	{
		(*this) << "\nFile Not Found:" << p.native_file_string().c_str() << "\n";
		return;
	}
	else if (!bfs::is_directory(p))
	{
		(*this) << "\nFound: " << p.native_file_string().c_str() << "\n";
		return;
	}
	(*this) << "In directory:" << p.native_file_string().c_str() << "\n";
	bfs::directory_iterator iter(p), end_iter;
	for (; iter != end_iter; ++iter)
	{
		try
		{
			if (bfs::is_directory(*iter))
			{
				++dc;
				(*this) << iter->filename().c_str() << "[Directory]\n";
			} 
			else
			{
				++fc;
				const char* pszModuleFileName = iter->filename().c_str();
				this->addModuleFromFile( iter->filename().c_str() );
				(*this) << iter->filename().c_str() << "\n";
			}
		} 
		catch (const std::exception & ex)
		{
			(*this) << iter->filename().c_str() << ": " << ex.what() << "\n";
		}
		(*this) << fc << " " << dc << "\n";
	} //for
}

//-----------------------------------------------------------------------------
bool ModuleManager::addModuleFromFile(const char* pszFileName)
{
	bool bResult = false;
	// first check if the module type matches
	enString_t moduleType;
	this->queryModuleRuntimeTypeFromFileName( pszFileName, moduleType );
	if( this->m_moduleRuntimeType.compare( moduleType ) == 0 )
	{
		if( ( true == EduNet::DynamicLibrary::isDynamicLib( pszFileName ) ) )
		{
			bool bWantsToLoadModule = this->appWantsToLoadModule( pszFileName );
			if( ( true == bWantsToLoadModule ) )
			{
				
				/*{
					RawModulePtr spNewLib(ET_NEW RawModule() );
					RawModule* pkNewmodule = spNewLib.get();
					bool bResult = pkNewmodule->load ( pszFileName );
					if ( true == bResult )
					{
						pkNewmodule->unload();
					}
				}*/

				RawModulePtr spNewModule(ET_NEW RawModule());
				RawModule* pkNewmodule = spNewModule.get();
				bool bResult = pkNewmodule->load ( pszFileName );
				if ( true == bResult )
				{
					// pass over important application data to this module
					ModuleEntry* moduleEntry = pkNewmodule->accessEntry();
					moduleEntry->setOpenSteerUTData( OpenSteerUTData::g_openSteerUTDataPtr );
					this->m_modules.push_back ( spNewModule );
				}
			}
		}
	}
	return bResult;
}

//-----------------------------------------------------------------------------
bool ModuleManager::appWantsToLoadModule (
	const char* pszModuleName )
{
	const enStringArray_t& kNames = EduNet::Options::accessOptions().accessModuleNameList();

	// by default load all
	if (true == kNames.empty())
	{
		return true;
	}

	enString_t moduleName(pszModuleName);
	::tolower( moduleName );
	enStringArray_t::const_iterator iter = kNames.begin();
	enStringArray_t::const_iterator iterEnd = kNames.end();
	while( iter != iterEnd )
	{
		enString_t temp(*iter);
		::tolower( temp );
		if( moduleName.find( temp ) != enString_t::npos )
		{
			return true;
		}
		++iter;
	}
	return false;
}

//-----------------------------------------------------------------------------
void ModuleManager::unloadAll( void )
{
	RawModules::const_iterator iter = this->m_modules.begin();
	RawModules::const_iterator iterEnd =  this->m_modules.end();
	while( iter != iterEnd  )
	{
		RawModule* pModule = iter->get();
		pModule->unload();

		++iter;
	}

	this->m_modules.clear();
}

//-----------------------------------------------------------------------------
void ModuleManager::unloadModule( const char* pszName )
{

}



}

