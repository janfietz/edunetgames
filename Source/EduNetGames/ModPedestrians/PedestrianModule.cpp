//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
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

#include "PedestrianModule.h"
#include "EduNetConnect/NetworkPlugin.h"
#include "OpenSteerUT/OpenSteerUT.h"
#include "EduNetGames/Pedestrians/NetPedestrianPlugins.h"

//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT + 303;
	kSettings.uiServerStartPort = SERVER_PORT + 303;
	kSettings.sessionPassword = "Zoning";
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
	PedestrianModulePluginFactory::PedestrianModulePluginFactory()
	{

	}

	//-------------------------------------------------------------------------
	PedestrianModulePluginFactory::~PedestrianModulePluginFactory()
	{

	}

	//-------------------------------------------------------------------------
	void PedestrianModulePluginFactory::fillStringArrayWithPluginName( enStringArray_t& kNames ) const
	{
		kNames.push_back( "PedestrianPlugin" );
		kNames.push_back( "PedestrianClientPlugin" );
		kNames.push_back( "PedestrianServerPlugin" );
		kNames.push_back( "PedestrianClientServerPlugin" );
	}

	//-------------------------------------------------------------------------
	OpenSteer::AbstractPlugin* PedestrianModulePluginFactory::createPluginByNameInternal(
		const char* pszName ) const
	{
		std::string kName(pszName);
		if( kName == "PedestrianPlugin" )
		{
			OpenSteer::AbstractPlugin* plugin( ET_NEW OfflinePedestrianPlugin() );
			return plugin;  
		}
		if( kName == "PedestrianClientPlugin" )
		{
			OpenSteer::AbstractPlugin* plugin( ET_NEW PedestrianRenderClientPlugin( true ) );
			return plugin;  
		}
		if( kName == "PedestrianServerPlugin" )
		{
			OpenSteer::AbstractPlugin* plugin( ET_NEW PedestrianRenderPeerPlugin( true ) );
			return plugin;  
		}
		if( kName == "PedestrianClientServerPlugin" )
		{
			OpenSteer::AbstractPlugin* plugin( ET_NEW PedestrianClientServerPlugin() );
			return plugin;  
		}
		return NULL;
	}

	//-------------------------------------------------------------------------
	const char* PedestrianModule::getName( void ) const
	{
		return "ModPedestrians";
	}

	//-------------------------------------------------------------------------
	const char* PedestrianModule::getAbout( void ) const
	{
		return "provides plugins to describe moving entity synchronization";
	}

	//-------------------------------------------------------------------------
	void PedestrianModule::setOpenSteerUTData( OpenSteerUTData* data ) const
	{
		OpenSteerUTData::_SDMInitDLL( data );
	}

	//-------------------------------------------------------------------------
	PluginFactory* PedestrianModule::createPluginFactory( void ) const
	{
		return ET_NEW PedestrianModulePluginFactory();
	}
}

ET_IMPLEMENT_MODULE_ENTRYFUNC(EduNet::PedestrianModule)

