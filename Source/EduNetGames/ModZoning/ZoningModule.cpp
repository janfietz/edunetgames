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
#include "EduNetConnect/NetworkPlugin.h"
#include "OpenSteerUT/OpenSteerUT.h"
#include "OpenSteerUT/ZonePlugin.h"

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
	}

	//-------------------------------------------------------------------------
	OpenSteer::AbstractPlugin* ZoningModulePluginFactory::createPluginByNameInternal(
		const char* pszName ) const
	{
		std::string kName(pszName);
		if( kName == "ZonePlugin" )
		{
			return ET_NEW ZonePlugin( false );  
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
	PluginFactory* ZoningModule::createPluginFactory( void ) const
	{
		return ET_NEW ZoningModulePluginFactory();
	}


}

ET_IMPLEMENT_MODULE_ENTRYFUNC(EduNet::ZoningModule)

