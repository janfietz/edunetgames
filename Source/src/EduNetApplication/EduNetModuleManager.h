#ifndef EDUNETMODULEMANAGER_H_
#define EDUNETMODULEMANAGER_H_

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
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "EduNetModule/EduNetModule.h"

namespace EduNet {


class ModuleManager
{
public:
	ModuleManager();
	virtual ~ModuleManager();

	const char* getCurrentModuleFileName( void ) const;
	const char* getCurrentModulePath( void ) const;
	static void queryModuleRuntimeTypeFromFileName( const char* pszFileName, enString_t& kModuleType );

	void loadModulesFromDirectory ( const char* pszDirectory );
	void unloadAll( void );
	void unloadModule( const char* pszName );
	void createPluginsFromModules ( void );
	void createPluginsFromModule ( RawModule* pkModule );
	bool addModuleFromFile ( const char* pszFileName );
	bool appWantsToLoadModule( const char* pszkModuleName );

	const RawModules& getModules( void ) const
	{
		return this->m_modules;
	}


private:

	const ModuleManager& operator<<( const char* ) const;
	const ModuleManager& operator<<( unsigned long ) const;

	RawModules m_modules;
	
	enum { EMaxPath = 2048 };
	char m_pszCurrentModuleFileName[EMaxPath];
	char m_pszCurrentModulePath[EMaxPath];
	enString_t m_moduleRuntimeType;

};

}

#endif /* EDUNETMODULEMANAGER_H_ */
