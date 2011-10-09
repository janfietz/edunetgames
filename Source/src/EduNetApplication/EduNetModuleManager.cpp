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
#include <boost/algorithm/string.hpp>
#include <boost/xpressive/xpressive.hpp>

#if BOOST_VERSION >= 104700
#define native_file_string string 
#endif

namespace bfs = boost::filesystem;
namespace bx = boost::xpressive;

// note: set to 1 to debug loadModulesFromDirectory
#define ET_DEBUG_LOADMODULES 0

namespace EduNet	{


//-----------------------------------------------------------------------------
ModuleManager::ModuleManager()
{
	char pszCurrentModuleFileName[EMaxPath];
#ifdef WIN32
	int bytes = ::GetModuleFileNameA( NULL, pszCurrentModuleFileName, sizeof(pszCurrentModuleFileName) );
#else
	char szTmp[32];
	sprintf(szTmp, "/proc/%d/exe", getpid());
	int bytes = readlink(szTmp, pszCurrentModuleFileName, sizeof(pszCurrentModuleFileName) );
#endif
	if(bytes > 0)
	{
		m_currentModuleFileName.assign(pszCurrentModuleFileName, bytes);
	}

	if( m_currentModuleFileName.empty() == false )
	{
		m_currentModulePath = m_currentModuleFileName;
		// determine the module path
		{
			size_t len = m_currentModuleFileName.length();
			for( size_t i = len-1; i >= 0; --i )
			{
				if( ( m_currentModuleFileName[i] == '\\' ) || ( m_currentModuleFileName[i] == '/' ) )
				{
					m_currentModulePath = m_currentModuleFileName.substr(0, i);
					break;
				}
			}
		}
		// determine the module type
		this->queryModuleRuntimeTypeFromFileName( m_currentModuleFileName.c_str(), this->m_moduleRuntimeType );
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

	bx::sregex re= bx::sregex::compile("_Debug");
	if(bx::regex_search( tempString, re ))
	{
		kModuleType.assign("debug");
	}
	else
	{
		kModuleType.assign("release");
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
const char* ModuleManager::getCurrentModulePath( void ) const
{
	return m_currentModulePath.c_str();
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
#if BOOST_VERSION >= 104700
		std::string fileName = iter->path().string();
#else
		std::string fileName = iter->filename();
#endif

		try
		{
			if (bfs::is_directory(*iter))
			{
				++dc;
				(*this) << fileName.c_str() << "[Directory]\n";
			} 
			else
			{
				++fc;
				const char* pszModuleFileName = fileName.c_str();
				this->addModuleFromFile( fileName.c_str() );
				(*this) << fileName.c_str() << "\n";
			}
		} 
		catch (const std::exception & ex)
		{
			(*this) << fileName.c_str() << ": " << ex.what() << "\n";
		}
		(*this) << fc << " " << dc << "\n";
	} //for
}

//-----------------------------------------------------------------------------
bool ModuleManager::addModuleFromFile(const char* pszFileName)
{
	bool bResult = false;
	// first check if the module type matches
	if( ( true == EduNet::DynamicLibrary::isDynamicLib( pszFileName ) ) )
	{
		enString_t moduleType;
		this->queryModuleRuntimeTypeFromFileName( pszFileName, moduleType );
		if( this->m_moduleRuntimeType.compare( moduleType ) == 0 )
		{
		
			bool bWantsToLoadModule = this->appWantsToLoadModule( pszFileName );
			if( ( true == bWantsToLoadModule ) )
			{
				RawModulePtr spNewLib(ET_NEW RawModule() );
				RawModule* pkNewmodule = spNewLib.get();

				bool bResult = pkNewmodule->load ( pszFileName );
				if ( true == bResult )
				{
					// pass over important application data to this module
					ModuleEntry* moduleEntry = pkNewmodule->accessEntry();
					moduleEntry->setOpenSteerUTData( OpenSteerUTData::g_openSteerUTDataPtr );
					this->m_modules.push_back ( spNewLib );
				}
			}
		
		}
		else
		{
	#ifdef ET_DEBUG
			std::ostringstream message;
			message << "File: ";
			message << pszFileName << " is not of type:" << m_moduleRuntimeType.c_str() << std::ends;
			Log::printMessage ( message );
	#endif
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
	boost::to_lower(moduleName);
	enStringArray_t::const_iterator iter = kNames.begin();
	enStringArray_t::const_iterator iterEnd = kNames.end();
	while( iter != iterEnd )
	{
		enString_t temp(*iter);
		boost::to_lower(temp);
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
	this->m_modules.clear();
}

//-----------------------------------------------------------------------------
void ModuleManager::unloadModule( const char* pszName )
{

}



}

