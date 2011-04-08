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

#include "ZoningModule.h"
#include "PeerZonePlugin.h"
#include "EduNetConnect/NetworkPlugin.h"
#include "OpenSteerUT/OpenSteerUT.h"

//-----------------------------------------------------------------------------
void EduNetConnect::queryConnectionsSettings( ConnectSettings& kSettings )
{
	kSettings.uiClientStartPort = CLIENT_PORT + 300;
	kSettings.uiServerStartPort = SERVER_PORT + 300;
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
	ZoningModulePluginFactory::ZoningModulePluginFactory()
	{

	}

	//-------------------------------------------------------------------------
	ZoningModulePluginFactory::~ZoningModulePluginFactory()
	{

	}

	//-------------------------------------------------------------------------
	void ZoningModulePluginFactory::fillStringArrayWithPluginName( enStringArray_t& kNames ) const
	{
		kNames.push_back( "ZonePlugin" );
		kNames.push_back( "EmptyZonePlugin" );
		kNames.push_back( "ZonePlugin0" );
		kNames.push_back( "ZonePlugin1" );
		kNames.push_back( "ZonePlugin2" );
		kNames.push_back( "ZonePlugin3" );
		kNames.push_back( "PeerZone0Master" );
		kNames.push_back( "PeerZone1Master" );
		kNames.push_back( "PeerZone2Master" );
		kNames.push_back( "PeerZone3Master" );
		kNames.push_back( "PeerZoneViewer" );
//		kNames.push_back( "OfflinePedestrianPlugin" );
	}

	//-------------------------------------------------------------------------
	OpenSteer::AbstractPlugin* ZoningModulePluginFactory::createPluginByNameInternal(
		const char* pszName ) const
	{
		std::string kName(pszName);
		if( kName == "ZonePlugin" )
		{
			OpenSteer::ZonePlugin* pkZone = ET_NEW MasterZonePlugin( false );

			return pkZone;  
		}
		if( kName == "EmptyZonePlugin" )
		{
			OpenSteer::ZonePlugin* pkZone = ET_NEW MasterZonePlugin( false, 5 );

			return pkZone;  
		}
		if( kName == "ZonePlugin0" )
		{
			OpenSteer::ZonePlugin* pkZone = ET_NEW MasterZonePlugin( false, 0 );

			return pkZone;  
		}
		if( kName == "ZonePlugin1" )
		{
			OpenSteer::ZonePlugin* pkZone = ET_NEW MasterZonePlugin( false, 1 );

			return pkZone;  
		}
		if( kName == "ZonePlugin2" )
		{
			OpenSteer::ZonePlugin* pkZone = ET_NEW MasterZonePlugin( false, 2 );

			return pkZone;  
		}
		if( kName == "ZonePlugin3" )
		{
			OpenSteer::ZonePlugin* pkZone = ET_NEW MasterZonePlugin( false, 3 );

			return pkZone;  
		}
		if( kName == "PeerZone0Master" )
		{
			PeerZonePlugin* pkZone = ET_NEW PeerZonePlugin( false );
			pkZone->setZoneId( 0 );

			return pkZone;  
		}
		if( kName == "PeerZone1Master" )
		{
			PeerZonePlugin* pkZone = ET_NEW PeerZonePlugin( false );
			pkZone->setZoneId( 1 );

			return pkZone;  
		}
		if( kName == "PeerZone2Master" )
		{
			PeerZonePlugin* pkZone = ET_NEW PeerZonePlugin( false );
			pkZone->setZoneId( 2 );

			return pkZone;  
		}
		if( kName == "PeerZone3Master" )
		{
			PeerZonePlugin* pkZone = ET_NEW PeerZonePlugin( false );
			pkZone->setZoneId( 3 );

			return pkZone;  
		}
		if( kName == "PeerZoneViewer" )
		{
			PeerZonePlugin* pkZone = ET_NEW PeerZonePlugin( false );
			pkZone->setZoneId( 5 );

			return pkZone;  
		}
		return NULL;
	}

	//-------------------------------------------------------------------------
	const char* ZoningModule::getName( void ) const
	{
		return "ModZones";
	}

	//-------------------------------------------------------------------------
	const char* ZoningModule::getAbout( void ) const
	{
		return "provides plugins to describe zoning concepts";
	}

	//-------------------------------------------------------------------------
	void ZoningModule::setOpenSteerUTData( OpenSteerUTData* data ) const
	{
		OpenSteerUTData::_SDMInitDLL( data );
	}

	//-------------------------------------------------------------------------
	PluginFactory* ZoningModule::createPluginFactory( void )
	{
		return ET_NEW ZoningModulePluginFactory();
	}

	//-----------------------------------------------------------------------------
	void ZoningModule::destroyPluginFactory( PluginFactory* pFactory )
	{
		ET_SAFE_DELETE(pFactory);
	}
}

//#include <mgf/memory/MemoryTracker.h>
//
//// install debug memory tracking facility
//mgf::MemoryDebug memoryDebug(true);

ET_IMPLEMENT_MODULE_ENTRYFUNC(EduNet::ZoningModule)

