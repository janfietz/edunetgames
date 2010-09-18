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

#include "EduNetGames/Tutorial_04/NetPedestrianPlugins.h"

#if 0
AbstractPlugin* parent = dynamic_cast<AbstractPlugin*>(vehicle.getParentEntity());
if(NULL != parent)
{
	AbstractLocalSpace* parentPluginLocalSpace = dynamic_cast<AbstractLocalSpace*>(parent);
	Vec3 offset(parentPluginLocalSpace->position());
	AbstractPlugin* parentPlugin = parent->getParentPlugin();
	while( NULL != parentPlugin )
	{
		parentPluginLocalSpace = dynamic_cast<AbstractLocalSpace*>(parentPlugin);
		offset += parentPluginLocalSpace->position();
		parentPlugin = parentPlugin->getParentPlugin();
	}

	LocalSpaceData localCopy = vehicle.getLocalSpaceData();
	localCopy._position += offset;
	this->drawBasic2dCircularLocalSpace(
		localCopy,
		color, vehicle.radius() );
}
else
{
}
#endif

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

	class MasterZonePlugin : public OpenSteer::ZonePlugin
	{
		ET_DECLARE_BASE( OpenSteer::ZonePlugin )
	public:
		MasterZonePlugin ( bool bAddToRegistry = false ):
		BaseClass( bAddToRegistry )
		{

		}

		void zoneCheck( const ZonePlugin* zone, SimpleNetworkVehicle* vehicle )
		{
			vehicle->setIsZoneMember( zone->getZoneId(), zone->isVehicleInside( *vehicle ) );
		}

		//---------------------------------------------------------------------
		virtual void update( const float currentTime, const float elapsedTime )
		{
			BaseClass::update( currentTime, elapsedTime );
			
			// now check and update zone memberships
			size_t pluginCount = this->getPluginCount();

			typedef std::vector<ZonePlugin*> ZonePluginArray_t;
			ZonePluginArray_t subZones;
			for( size_t i = 0; i < pluginCount; ++i )
			{
				ZonePlugin* pkSubZone = dynamic_cast<ZonePlugin*>(this->getPlugin(i));
				if( NULL != pkSubZone )
				{
					subZones.push_back( pkSubZone );
				}
			}

			ZonePluginArray_t::iterator iterStart = subZones.begin();
			ZonePluginArray_t::const_iterator iterEnd = subZones.end();

			ZonePluginArray_t::const_iterator iter0 = iterStart;
			while( iter0 != iterEnd )
			{
				ZonePluginArray_t::iterator iter1 = iterStart;
				while( iter1 != iterEnd )
				{
					AbstractPlugin* contentPlugin = (*iter1)->getPlugin(0);
					osAVGroup vehicles = contentPlugin->allVehicles();
					osAVIterator vehicleIter = vehicles.begin();
					osAVIterator vehicleIterEnd = vehicles.end();
					while( vehicleIter != vehicleIterEnd )
					{
						SimpleNetworkVehicle* networkVehicle = dynamic_cast<SimpleNetworkVehicle*>(*vehicleIter);
						if( NULL != networkVehicle )
						{
							this->zoneCheck( *iter0, networkVehicle );
						}
						++vehicleIter;
					}
					++iter1;
				}

				++iter0;
			}

		}

		virtual void onSubZoneAdded( ZonePlugin* pkSubZone )
		{
//			if( 0 == pkSubZone->getZoneId() )
			{
				NetPedestrianPlugin* pkContentPlugin = ET_NEW NetPedestrianPlugin( false, 0.225 );
				pkContentPlugin->setPathColor( pkSubZone->getZoneColor() );
				pkSubZone->addPlugin( pkContentPlugin );
			}
		};



	};

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
		kNames.push_back( "OfflinePedestrianPlugin" );
	}

	//-------------------------------------------------------------------------
	OpenSteer::AbstractPlugin* ZoningModulePluginFactory::createPluginByNameInternal(
		const char* pszName ) const
	{
		std::string kName(pszName);
		if( kName == "ZonePlugin" )
		{
			ZonePlugin* pkZone = ET_NEW MasterZonePlugin( false );

			return pkZone;  
		}
		if( kName == "OfflinePedestrianPlugin" )
		{
			return ET_NEW OfflinePedestrianPlugin( false );  
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

