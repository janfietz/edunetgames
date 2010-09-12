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
#include "EduNetPluginLoadPlugin.h"
#include "EduNetCommon/EduNetOptions.h"
#include "EduNetModule/EduNetPluginFactory.h"

namespace EduNet
{

//-----------------------------------------------------------------------------
void PluginLoadPlugin::loadModules( const char* pszPath )
{
	// load modules in working directory
	this->m_modules.loadModulesFromDirectory( pszPath );
}

//-----------------------------------------------------------------------------
void PluginLoadPlugin::unloadModules( void )
{
	this->removeAllPlugins();
	this->m_modules.unloadAll();
}

//-----------------------------------------------------------------------------
void PluginLoadPlugin::createPluginsFromModules ( void )
{
	const RawModules& kModules = this->m_modules.getModules();
	RawModules::const_iterator kIter = kModules.begin();
	RawModules::const_iterator kIterEnd = kModules.end();
	while ( kIterEnd != kIter )
	{
		RawModule* pkModule = ( *kIter ).get();
		this->createPluginsFromModule ( pkModule );
		++kIter;
	}
}

//-----------------------------------------------------------------------------
void PluginLoadPlugin::createPluginsFromModule (
	RawModule* pkModule )
{
	ModuleEntry* pkEntry = pkModule->accessEntry();
	bool bWantsToLoadPlugin = this->appWantsToLoadPlugin( pkEntry->getName() );
	if ( (NULL != pkEntry)  && (true == bWantsToLoadPlugin) )
	{
		EduNetPluginFactory* pkFactory = pkEntry->createPluginFactory();
		EduNetPluginFactoryPtr spFactory ( pkFactory );

		EdutNetStringList kList;
		pkFactory->getPluginNames ( kList );

		std::ostringstream message;
		message << "Plugins in loaded Module \"" << pkEntry->getName() << "\"\n";
		EdutNetStringList::iterator kNameIter = kList.begin();
		EdutNetStringList::iterator kNameIterEnd = kList.end();
		while ( kNameIterEnd != kNameIter )
		{
			const char* pszPluginName = ( *kNameIter ).c_str();
			AbstractPlugin* pkPlugin = createPluginFromFactoryByName(pkFactory, pszPluginName );
			if ( NULL != pkPlugin )
			{
				this->addPlugin ( pkPlugin );
				// little hack
				OpenSteer::Plugin::addToRegistry ( pkPlugin );
			}
			message << '"' << ( *kNameIter ).c_str() << '"' << "\n";
			++kNameIter;
		}
		message << std::ends;
		EduNet::Log::printMessage ( message );
	}
}

//-----------------------------------------------------------------------------
bool PluginLoadPlugin::appWantsToLoadPlugin (
										const char* pszPluginName )
{
//	const enStringArray_t& kNames = EduNetOptions::accessOptions().accessModuleNameList();
	const enStringArray_t kNames;

	// by default load all
	if (true == kNames.empty())
	{
		return true;
	}

	enString_t pluginName(pszPluginName);
	::tolower( pluginName );
	enStringArray_t::const_iterator iter = kNames.begin();
	enStringArray_t::const_iterator iterEnd = kNames.end();
	while( iter != iterEnd )
	{
		enString_t temp(*iter);
		::tolower( temp );
		if( pluginName.find( temp ) != enString_t::npos )
		{
			return true;
		}
		++iter;
	}
	return false;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin*  PluginLoadPlugin::createPluginFromFactoryByName(
	EduNetPluginFactory* pkFactory,
	const char* pszPluginName )
{
	return pkFactory->createPluginByName( pszPluginName );
}
//-----------------------------------------------------------------------------
OpenSteer::AbstractPlugin* PluginLoadPlugin::createPluginByName(
	const char* pszPluginName )
{
	RawModule* pkModule = this->findModuleForPlugin( pszPluginName );
	if (NULL != pkModule)
	{
		ModuleEntry* pkEntry = pkModule->accessEntry();
		if (NULL != pkEntry )
		{
			EduNetPluginFactory* pkFactory = pkEntry->createPluginFactory();
			EduNetPluginFactoryPtr spFactory ( pkFactory );
			return this->createPluginFromFactoryByName( pkFactory, pszPluginName );
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
RawModule* PluginLoadPlugin::findModuleForPlugin(
	const char* pszPluginName )
{
	const RawModules& kModules = this->m_modules.getModules();
	RawModules::const_iterator kIter = kModules.begin();
	RawModules::const_iterator kIterEnd = kModules.end();
	while ( kIterEnd != kIter )
	{
		RawModule* pkModule = ( *kIter ).get();
		ModuleEntry* pkEntry = pkModule->accessEntry();
		bool bWantsToLoadPlugin = appWantsToLoadPlugin( pkEntry->getName() );
		if ( (NULL != pkEntry)  && (true == bWantsToLoadPlugin) )
		{
			EduNetPluginFactory* pkFactory = pkEntry->createPluginFactory();
			EduNetPluginFactoryPtr spFactory ( pkFactory );

			EdutNetStringList kList;
			pkFactory->getPluginNames ( kList );

			// check if plugin name is inside of this array
			std::string kName(pszPluginName);
			EdutNetStringList::const_iterator kNameIter = 
				std::find(kList.begin(), kList.end(), kName);

			if( kNameIter != kList.end() )
			{
				return pkModule;
			}
		}
		++kIter;
	}
	return NULL;
}


}