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

#include "PeerZonePlugin.h"
#include "OpenSteerUT/OpenSteerUT.h"
#include "OpenSteerUT/ZonePlugin.h"

#include "NetPedestrianPlugins.h"


//-----------------------------------------------------------------------------
namespace EduNet	{

	MasterZonePlugin::MasterZonePlugin ( bool bAddToRegistry, size_t zoneId ):
	BaseClass( bAddToRegistry ),
	m_uiZoneId(zoneId),
	m_pEntityFactory(NULL)
	{
		this->m_bCreateContentZone[0] = false;
		this->m_bCreateContentZone[1] = false;
		this->m_bCreateContentZone[2] = false;
		this->m_bCreateContentZone[3] = false;
		if( zoneId < 4 )
		{
			this->m_bCreateContentZone[zoneId] = true;
		}
		else
		{
			if( zoneId < 5 )
			{
				this->m_bCreateContentZone[0] = true;
				this->m_bCreateContentZone[1] = true;
				this->m_bCreateContentZone[2] = true;
				this->m_bCreateContentZone[3] = true;
			}
		}
	}

	void MasterZonePlugin::setZoneId( size_t zoneId )
	{
		this->m_uiZoneId = zoneId;
		this->m_bCreateContentZone[0] = false;
		this->m_bCreateContentZone[1] = false;
		this->m_bCreateContentZone[2] = false;
		this->m_bCreateContentZone[3] = false;
		if( zoneId < 4 )
		{
			this->m_bCreateContentZone[zoneId] = true;
		}
		else
		{
			if( zoneId < 5 )
			{
				this->m_bCreateContentZone[0] = true;
				this->m_bCreateContentZone[1] = true;
				this->m_bCreateContentZone[2] = true;
				this->m_bCreateContentZone[3] = true;
			}
		}
	}


	const char* MasterZonePlugin::name() const
	{
		if( this->m_uiZoneId < 4 )
		{
			if(this->m_bCreateContentZone[0])
				return "Zone-0";
			if(this->m_bCreateContentZone[1])
				return "Zone-1";
			if(this->m_bCreateContentZone[2])
				return "Zone-2";
			if(this->m_bCreateContentZone[3])
				return "Zone-3";
		}
		if( this->m_uiZoneId < 5 )
		{
			return "Zones";
		}
		else
		{
			return "EmptyZones";
		}
	}

	void MasterZonePlugin::zoneCheck( const ZonePlugin* zone, SimpleNetworkVehicle* vehicle )
	{
		// TODO:
		vehicle->setIsZoneMember( zone->getZoneId(), zone->isVehicleInside( *vehicle ) );
		// TODO:
	}

	//---------------------------------------------------------------------
	void MasterZonePlugin::update( const float currentTime, const float elapsedTime )
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
				if( NULL == contentPlugin )
				{

				}
				else
				{
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
				}
				++iter1;
			}

			++iter0;
		}

	}

	void MasterZonePlugin::onSubZoneAdded( ZonePlugin* pkSubZone )
	{
		AbstractEntityFactory* pFactory(NULL);
		if( true == this->m_bCreateContentZone[pkSubZone->getZoneId()] )
		{
			pFactory = this->getEntityFactory();			
		}
		NetPedestrianPlugin* pkContentPlugin = ET_NEW NetPedestrianPlugin( false, 0.225 );
		pkContentPlugin->setPathColor( pkSubZone->getZoneColor() );
		pkContentPlugin->setRenderPath( NULL != pFactory );
		pkContentPlugin->setEntityFactory( pFactory );
		pkSubZone->addPlugin( pkContentPlugin );
	};

	AbstractEntityFactory* MasterZonePlugin::getEntityFactory( void ) const
	{
		return this->m_pEntityFactory;
	}

	void MasterZonePlugin::setEntityFactory( AbstractEntityFactory* pFactory )
	{
		this->m_pEntityFactory = pFactory;
	}
//-----------------------------------------------------------------------------
	void MasterZonePlugin::addVehicle( OpenSteer::AbstractVehicle* pkVehicle )
	{
		// TODO WHAT
		// add the vehicle to the corresponding zone
	}
//-----------------------------------------------------------------------------
	void MasterZonePlugin::removeVehicle( OpenSteer::AbstractVehicle* pkVehicle )
	{
		// TODO WHAT
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
PeerZonePlugin::PeerZonePlugin( bool bAddToRegistry ):BaseClass( bAddToRegistry )
{

}

//-----------------------------------------------------------------------------
void PeerZonePlugin::setZoneId( size_t zoneId )
{
	this->m_kGamePlugin.setZoneId( zoneId );
}

//-----------------------------------------------------------------------------
void PeerZonePlugin::initializeReplication()
{	
	this->m_pkReplicaManager = ET_NEW AbstractEntityReplicaManager();
	this->setGamePluginReplicaManager( this->m_pkReplicaManager );
	this->m_pkReplicaManager->setPlugin( &this->m_kGamePlugin );
	
	// attach vehicle factory
	this->m_pkNetPedestrianFactory = ET_NEW AbstractEntityReplicaFactory( this->m_pkReplicaManager );	
	this->m_kGamePlugin.setEntityFactory( this->m_pkNetPedestrianFactory );
}
//-----------------------------------------------------------------------------
void PeerZonePlugin::shutdownReplication()
{	
	this->m_pNetInterface->DetachPlugin( this->m_pkReplicaManager );
	this->setGamePluginReplicaManager( NULL );
	ET_SAFE_DELETE(this->m_pkReplicaManager);
	
	// attach vehicle factory
	this->m_kGamePlugin.setEntityFactory( NULL );
	ET_SAFE_DELETE(this->m_pkNetPedestrianFactory);
	
}
//-----------------------------------------------------------------------------
const char* PeerZonePlugin::name() const
{
	if( this->m_kGamePlugin.getZoneId() < 4 )
	{
		if(this->m_kGamePlugin.createsContentZone(0))
			return "PeerZone-0";
		if(this->m_kGamePlugin.createsContentZone(1))
			return "PeerZone-1";
		if(this->m_kGamePlugin.createsContentZone(2))
			return "PeerZone-2";
		if(this->m_kGamePlugin.createsContentZone(3))
			return "PeerZone-3";
	}
	if( this->m_kGamePlugin.getZoneId() < 5 )
	{
		return "PeerZones";
	}
	else
	{
		return "PeerEmptyZones";
	}
}

//-----------------------------------------------------------------------------
void PeerZonePlugin::prepareOpen( void ) 
{ 
	BaseClass::prepareOpen();
	this->initializeReplication();
	this->m_kGamePlugin.prepareOpen();
}
//-----------------------------------------------------------------------------
void PeerZonePlugin::CreateContent( void )
{	
	this->m_pNetInterface->AttachPlugin( this->m_pkReplicaManager );
	BaseClass::CreateContent();
}
//-----------------------------------------------------------------------------
void PeerZonePlugin::DeleteContent( void )
{
	BaseClass::DeleteContent();
	this->shutdownReplication();
}
//-----------------------------------------------------------------------------
AbstractEntityReplica* PeerZonePlugin::allocEntityReplica(
	OpenSteer::AbstractPlugin* pPlugin,
	OpenSteer::EntityClassId classId,
	bool bIsRemoteObject,
	bool bClientReplica ) const
{
	bool btest;
	btest = true;
	return BaseClass::allocEntityReplica(pPlugin,
		classId,
		bIsRemoteObject, bClientReplica);
}
//-----------------------------------------------------------------------------
OpenSteer::AbstractEntityFactory* PeerZonePlugin::getGamePluginEntityFactory(
	void ) const
{
	return &this->m_kOfflinePedestrianFactory;
}