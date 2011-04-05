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

#include "NetSoccerModule.h"
#include "NetSoccerPlugins.h"
#include "OpenSteerUT/OpenSteerUT.h"

//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT + 200;
	kSettings.uiServerStartPort = SERVER_PORT + 200;
	kSettings.sessionPassword = "NetSoccer";
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
	NetSoccerPluginFactory::NetSoccerPluginFactory()
	{

	}
	//-------------------------------------------------------------------------
	NetSoccerPluginFactory::~NetSoccerPluginFactory()
	{

	}
	//-------------------------------------------------------------------------
	void NetSoccerPluginFactory::fillStringArrayWithPluginName( enStringArray_t& kNames ) const
	{
		kNames.push_back("NetSoccerRenderOfflinePlugin");
		kNames.push_back("RenderSoccerPeerPlugin");
		kNames.push_back("RenderSoccerClientPlugin");
		kNames.push_back("SoccerClientServerPlugin");
	}

	//-------------------------------------------------------------------------
	OpenSteer::AbstractPlugin* NetSoccerPluginFactory::createPluginByNameInternal(
		const char* pszName ) const
	{
		enString_t kName(pszName);
		if (0 == kName.compare("NetSoccerRenderOfflinePlugin"))
		{
			return ET_NEW NetSoccerRenderOfflinePlugin(false);
		}
		if (0 == kName.compare("RenderSoccerPeerPlugin"))
		{
			return ET_NEW RenderSoccerPeerPlugin(false);
		}
		if (0 == kName.compare("RenderSoccerClientPlugin"))
		{
			return ET_NEW RenderSoccerClientPlugin(false);
		}
		if (0 == kName.compare("SoccerClientServerPlugin"))
		{
			return ET_NEW SoccerClientServerPlugin(false);
		}
		return NULL;
	}

	//-------------------------------------------------------------------------
	const char* NetSoccerModule::getName( void ) const
	{
		return "netsoccer";
	}

	//-------------------------------------------------------------------------
	const char* NetSoccerModule::getAbout( void ) const
	{
		return "Provides plugins fo a distributed soccer game";
	}

	//-------------------------------------------------------------------------
	void NetSoccerModule::setOpenSteerUTData( OpenSteerUTData* data ) const
	{
		OpenSteerUTData::_SDMInitDLL( data );
	}

	//-------------------------------------------------------------------------
	PluginFactory* NetSoccerModule::createPluginFactory( void ) const
	{
		return ET_NEW NetSoccerPluginFactory();
	}

}

#include <mgf/memory/MemoryTracker.h>

// install debug memory tracking facility
mgf::MemoryDebug memoryDebug(true);

ET_IMPLEMENT_MODULE_ENTRYFUNC(EduNet::NetSoccerModule)
