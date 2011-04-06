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

#include "NetCtfPlugins.h"

#include "OpenSteerUT/PluginArray.h"
#include "OpenSteerUT/OpenSteerUT.h"

#include "EduNetModule/EduNetPluginFactory.h"
#include "EduNetModule/EduNetModule.h"

//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT + 500;
	kSettings.uiServerStartPort = SERVER_PORT + 500;
	kSettings.sessionPassword = "CTF";
	kSettings.uiPortPongCount = 10;
}

//-----------------------------------------------------------------------------
void OpenSteer::handleGlobalDataInstanceFailure( void )
{
	// in case this happens the whole thing will simply not work
	assert( true == GlobalData::hasInstance() );
}

//-----------------------------------------------------------------------------
namespace EduNet	{

	//-------------------------------------------------------------------------
	class CtfModulePluginFactory : public PluginFactory
	{
		ET_DECLARE_BASE( PluginFactory )
	public:
		CtfModulePluginFactory();
	protected:
		~CtfModulePluginFactory();

		virtual void fillStringArrayWithPluginName( enStringArray_t& kNames ) const;
		virtual OpenSteer::AbstractPlugin* createPluginByNameInternal(
			const char* pszName ) const;
	};

	//-------------------------------------------------------------------------
	CtfModulePluginFactory::CtfModulePluginFactory()
	{
		NetCtfPlugin::allocateEntityFactory();

	}

	//-------------------------------------------------------------------------
	CtfModulePluginFactory::~CtfModulePluginFactory()
	{

		NetCtfPlugin::destroyEntityFactory();
	}

	//-------------------------------------------------------------------------
	void CtfModulePluginFactory::fillStringArrayWithPluginName( enStringArray_t& kNames ) const
	{
		kNames.push_back( "CtfRenderClientPlugin" );
		kNames.push_back( "CtfRenderPeerPlugin" );
		kNames.push_back( "CtfClientServerPlugin" );
	}

	//-------------------------------------------------------------------------
	OpenSteer::AbstractPlugin* CtfModulePluginFactory::createPluginByNameInternal(
		const char* pszName ) const
	{
		std::string kName(pszName);
		if( kName == "CtfRenderClientPlugin" )
		{
			OpenSteer::AbstractPlugin* pkPlugin = ET_NEW CtfRenderClientPlugin( false );
			return pkPlugin;  
		}
		else if( kName == "CtfRenderPeerPlugin" )
		{
			OpenSteer::AbstractPlugin* pkPlugin = ET_NEW CtfRenderPeerPlugin( false );
			return pkPlugin;  
		}
		else if( kName == "CtfClientServerPlugin" )
		{
			OpenSteer::AbstractPlugin* pkPlugin = ET_NEW CtfClientServerPlugin( false );
			return pkPlugin;  
		}
		return NULL;
	}

	//-------------------------------------------------------------------------
	class CtfModule : public EduNet::ModuleEntry
	{
		ET_DECLARE_BASE( EduNet::ModuleEntry )
	public:
		virtual const char* getName( void ) const;
		virtual const char* getAbout( void ) const;

		virtual void setOpenSteerUTData( OpenSteerUTData* ) const;

		virtual PluginFactory* createPluginFactory( void );
		virtual void destroyPluginFactory(  PluginFactory* pFactory );
	};

	//-------------------------------------------------------------------------
	const char* CtfModule::getName( void ) const
	{
		return "ModCtf";
	}

	//-------------------------------------------------------------------------
	const char* CtfModule::getAbout( void ) const
	{
		return "provides plugins to show player control concepts";
	}

	//-------------------------------------------------------------------------
	void CtfModule::setOpenSteerUTData( OpenSteerUTData* data ) const
	{
		OpenSteerUTData::_SDMInitDLL( data );
	}

	//-------------------------------------------------------------------------
	PluginFactory* CtfModule::createPluginFactory( void )
	{
		return ET_NEW CtfModulePluginFactory();
	}

	void CtfModule::destroyPluginFactory( PluginFactory* pFactory )
	{
		ET_SAFE_DELETE(pFactory);
	}
}


#include <mgf/memory/MemoryTracker.h>

// install debug memory tracking facility
mgf::MemoryDebug memoryDebug(true);


ET_IMPLEMENT_MODULE_ENTRYFUNC(EduNet::CtfModule)
