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

#include "NetBoidModule.h"
#include "NetBoidPlugins.h"
#include "OpenSteerUT/OpenSteerUT.h"

//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT + 100;
	kSettings.uiServerStartPort = SERVER_PORT + 100;
	kSettings.sessionPassword = "NetBoidTutorials";
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
	NetBoidPluginFactory::NetBoidPluginFactory()
	{

	}
	//-------------------------------------------------------------------------
	NetBoidPluginFactory::~NetBoidPluginFactory()
	{

	}
	//-------------------------------------------------------------------------
	void NetBoidPluginFactory::fillStringArrayWithPluginName( enStringArray_t& kNames ) const
	{
		kNames.push_back("NetBoidRenderOfflinePlugin");
		kNames.push_back("NetBoidRenderPeerPlugin");
		kNames.push_back("NetBoidMultiplePeerPlugin");
		kNames.push_back("NetBoidServerPlugin");
		kNames.push_back("NetBoidClientPlugin");

	}

	//-------------------------------------------------------------------------
	OpenSteer::AbstractPlugin* NetBoidPluginFactory::createPluginByNameInternal(
		const char* pszName ) const
	{
		std::string kName(pszName);
		if (0 == kName.compare("NetBoidRenderOfflinePlugin"))
		{
			return ET_NEW NetBoidRenderOfflinePlugin(false);
		}
		if (0 == kName.compare("NetBoidRenderPeerPlugin"))
		{
			return ET_NEW NetBoidRenderPeerPlugin(false);
		}
		if (0 == kName.compare("NetBoidMultiplePeerPlugin"))
		{
			return ET_NEW NetBoidMultiplePeerPlugin(false);
		}
		if (0 == kName.compare("NetBoidServerPlugin"))
		{
			return ET_NEW NetBoidRenderServerPlugin(false);
		}
		if (0 == kName.compare("NetBoidClientPlugin"))
		{
			return ET_NEW NetBoidRenderClientPlugin(false);
		}
		return NULL;
	}

	//-------------------------------------------------------------------------
	const char* NetBoidModule::getName( void ) const
	{
		return "netboid";
	}

	//-------------------------------------------------------------------------
	const char* NetBoidModule::getAbout( void ) const
	{
		return "netboid";
	}

	//-------------------------------------------------------------------------
	void NetBoidModule::setOpenSteerUTData( OpenSteerUTData* data ) const
	{
		OpenSteerUTData::_SDMInitDLL( data );
	}

	//-------------------------------------------------------------------------
	PluginFactory* NetBoidModule::createPluginFactory( void )
	{
		return ET_NEW NetBoidPluginFactory();
	}

	//-----------------------------------------------------------------------------
	void NetBoidModule::destroyPluginFactory( PluginFactory* pFactory )
	{
		ET_SAFE_DELETE(pFactory);
	}
}

#include <mgf/memory/MemoryTracker.h>

// install debug memory tracking facility
mgf::MemoryDebug memoryDebug(true);

ET_IMPLEMENT_MODULE_ENTRYFUNC(EduNet::NetBoidModule)
