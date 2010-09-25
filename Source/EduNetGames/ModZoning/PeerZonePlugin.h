#ifndef __PEERZONEPLUGIN_H__
#define __PEERZONEPLUGIN_H__
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

#include "OpenSteerUT/ZonePlugin.h"
#include "EduNetConnect/NetworkPlugin.h"
#include "EduNetConnect/PeerPlugin.h"
#include "EduNetConnect/AbstractEntityReplicaConnection.h"
#include "NetPedestrianFactory.h"

namespace OpenSteer	{
	class SimpleNetworkVehicle;
}

namespace EduNet	{

	class MasterZonePlugin : public OpenSteer::ZonePlugin
	{
		ET_DECLARE_BASE( OpenSteer::ZonePlugin )
	public:
		MasterZonePlugin ( bool bAddToRegistry = false, size_t zoneId = 4 );

		const char* name() const;

		void zoneCheck( const ZonePlugin* zone, SimpleNetworkVehicle* vehicle );

		//---------------------------------------------------------------------
		virtual void update( const float currentTime, const float elapsedTime );

		virtual void onSubZoneAdded( ZonePlugin* pkSubZone );

		bool createsContentZone( size_t uiZoneId ) const
		{
			if( uiZoneId < 4 )
			{
				return this->m_bCreateContentZone[uiZoneId];
			}
			return false;
		}

		virtual void setEntityFactory( AbstractEntityFactory* );

		virtual AbstractEntityFactory* getEntityFactory( void ) const;

		virtual void addVehicle (OpenSteer::AbstractVehicle* pkVehicle);
		virtual void removeVehicle (OpenSteer::AbstractVehicle* pkVehicle);

		bool m_bCreateContentZone[4];
		size_t m_uiZoneId;

		size_t getZoneId( void ) const{ return this->m_uiZoneId; };
		void setZoneId( size_t zoneId );
		AbstractEntityFactory* m_pEntityFactory;
	};

}


//-----------------------------------------------------------------------------
typedef PeerPlugin<EduNet::MasterZonePlugin> PeerZonePlugin_t;

//-----------------------------------------------------------------------------
class PeerZonePlugin : public PeerZonePlugin_t
{
	ET_DECLARE_BASE(PeerZonePlugin_t)
public:
	PeerZonePlugin( bool bAddToRegistry = false );

	virtual const char* name() const;

	virtual void prepareOpen( void ); 

	void setZoneId( size_t zoneId ); 
	
	virtual void CreateContent( void );
	virtual void DeleteContent( void );

	virtual AbstractEntityReplica* allocEntityReplica(
		OpenSteer::AbstractPlugin* pPlugin,
		OpenSteer::EntityClassId classId,
		bool bIsRemoteObject,  bool bClientReplica ) const;
	
	virtual OpenSteer::AbstractEntityFactory* getGamePluginEntityFactory( void ) const;
private:
	void initializeReplication();
	void shutdownReplication();
	AbstractEntityReplicaFactory* m_pkNetPedestrianFactory;
	AbstractEntityReplicaManager* m_pkReplicaManager;
	// factory to create pedestrian vehicles
	mutable NetPedestrianFactory m_kOfflinePedestrianFactory;
};


#endif //__PEERZONEPLUGIN_H__
