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

#include "MasterZonePlugin.h"
#include "OpenSteerUT/OpenSteerUT.h"

#include "EduNetGames/Pedestrians/NetPedestrianPlugins.h"


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

	void MasterZonePlugin::zoneCheck( const ZonePlugin* zone, SimpleNetworkVehicle* pVehicle )
	{
		// TODO:
		pVehicle->setIsZoneMember( zone->getZoneId(), zone->isVehicleInside( *pVehicle ) );
		pVehicle->setIsZoneBorderMember(zone->getZoneId(), zone->isVehicleInsideBorder( *pVehicle ) );
		
		// TODO:
	}
	void MasterZonePlugin::open( void )
	{		
		size_t pluginCount = this->getPluginCount();	
		for( size_t i = 0; i < pluginCount; ++i )
		{
			ZonePlugin* pkSubZone = dynamic_cast<ZonePlugin*>(this->getPlugin(i));
			if( NULL != pkSubZone )
			{
				size_t subZonePluginCount = pkSubZone->getPluginCount();	
				for( size_t i = 0; i < subZonePluginCount; ++i )
				{
					AbstractPlugin* pPlugin = pkSubZone->getPlugin(i);
					if( NULL != pPlugin )
					{
						std::string pluginName(pPlugin->pluginName());
						if (pluginName == "NetPedestrianPlugin")
						{
							AbstractEntityFactory* pFactory(NULL);
							if( true == this->m_bCreateContentZone[pkSubZone->getZoneId()] )
							{
								pFactory = this->getEntityFactory();			
							}
							NetPedestrianPlugin* pkContentPlugin = dynamic_cast<NetPedestrianPlugin*>(pPlugin);
							pkContentPlugin->setRenderPath( NULL != pFactory );
							pkContentPlugin->setEntityFactory( pFactory );
						}						
					}
				}
			}
		}
		BaseClass::open();
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

	OpenSteer::ZonePlugin* MasterZonePlugin::accessOwnSubZone() const
	{
		const size_t pluginCount = this->getPluginCount();
		for( size_t i = 0; i < pluginCount; ++i )
		{
			ZonePlugin* pSubZone = dynamic_cast<ZonePlugin*>(this->getPlugin(i));
			if( NULL != pSubZone )
			{
				if (pSubZone->getZoneId() == m_uiZoneId)
				{
					return pSubZone;
				}
			}
		}
		return NULL;
	}

	

	

}
