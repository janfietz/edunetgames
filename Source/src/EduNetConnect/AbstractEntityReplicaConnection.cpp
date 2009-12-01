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

#include "AbstractEntityReplicaConnection.h"
#include "AbstractEntityReplica.h"
#include "NetworkPlugin.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/VehicleClassIds.h"
#include "OpenSteer/Plugin.h"

//-----------------------------------------------------------------------------
// replica connection client
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RakNet::Replica3* AbstractEntityReplicaConnection::AllocReplica(
	RakNet::BitStream *allocationId, RakNet::ReplicaManager3 *replicaManager3)
{
	RakNet::RakString typeName;
	allocationId->Read( typeName );
	OpenSteer::EntityClassId classId = ET_CID_UNKNOWN;
	allocationId->Read( classId );
	OpenSteer::AbstractPlugin* pkPlugin = this->getPlugin();
	if( NULL != pkPlugin )
	{
		if( classId == ET_CID_UNKNOWN )
		{
			if( typeName == "NetCtfBaseVehicle" )
			{
				classId = ET_CID_CTF_BASE_VEHICLE;
			}
			else if( typeName == "NetCtfEnemyVehicle" )
			{
				classId = ET_CID_CTF_ENEMY_VEHICLE;
			}
			else if( typeName == "NetCtfSeekerVehicle" )
			{
				classId = ET_CID_CTF_SEEKER_VEHICLE;
			}
		}
		if( classId == OS_CID_SPHEREOBSTACLE )
		{
			bool bTest = true;
			bTest = false;
		}
		if( classId == ET_CID_UNKNOWN )
		{
			bool bTest = true;
			bTest = false;
			assert( classId != ET_CID_UNKNOWN );
		}
		else
		{
			OpenSteer::AbstractPlugin* pkParentPlugin = pkPlugin->getParentPlugin();
			if(NULL == pkParentPlugin)
			{
				pkParentPlugin = pkPlugin;
			}
			AbstractNetworkPlugin* pkNetworkPlugin = dynamic_cast<AbstractNetworkPlugin*>( pkParentPlugin );	

			AbstractEntityReplica* pkNewReplica(NULL);
			if(NULL != pkNetworkPlugin)
			{
				pkNewReplica = pkNetworkPlugin->allocEntityReplica( pkPlugin, classId, true , false);
			}else
			{
				pkNewReplica = ET_NEW AbstractEntityReplica( pkPlugin, classId, true  );
			}
			return pkNewReplica; 
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// replica factory peer
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
OpenSteer::AbstractEntity* AbstractEntityReplicaFactory::createEntity( 
	OpenSteer::EntityClassId classId ) const
{
	osAbstractPlugin* pkPlugin = this->m_pkReplicaManager->getPlugin();
	return this->createEntity( classId, pkPlugin );
	
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractEntity* AbstractEntityReplicaFactory::createEntity( 
	OpenSteer::EntityClassId classId,
	osAbstractPlugin* pkPlugin ) const
{
	osAbstractEntity* pkPluginEntity = OpenSteer::CastToAbstractEntity( pkPlugin );
	assert( NULL != pkPluginEntity );

	AbstractEntityReplica* pkNewReplica = this->createEntityReplica( 
		pkPlugin, classId, false, pkPluginEntity->isRemoteObject() );	
	if( NULL != pkNewReplica )
	{
		this->m_pkReplicaManager->Reference( pkNewReplica );
		OpenSteer::AbstractEntity* pkEntity = pkNewReplica->accessEntity();
		if( NULL != pkEntity )
		{
			this->m_uidMap.Set( pkEntity->getEntityId(), pkNewReplica );

			if( false == pkPluginEntity->isRemoteObject() )
			{
				NetworkID kNetWorkId = pkNewReplica->GetNetworkID();
				OpenSteer::NetworkId networkId = kNetWorkId.guid.g;
				pkEntity->setNetworkId( networkId );
			}
		}
		return pkEntity;
	}
	return NULL;
}
//-----------------------------------------------------------------------------
AbstractEntityReplica* AbstractEntityReplicaFactory::createEntityReplica( 
	OpenSteer::AbstractPlugin* pPlugin, 
	OpenSteer::EntityClassId classId, 
	bool bIsRemoteObject,  bool bClientReplica) const
{
	return ET_NEW AbstractEntityReplica( 
		pPlugin, classId, bIsRemoteObject, bClientReplica );	
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* AbstractEntityReplicaFactory::createVehicle( OpenSteer::EntityClassId classId ) const
{
	return dynamic_cast<OpenSteer::AbstractVehicle*>( this->createEntity( classId ) );
}

//-----------------------------------------------------------------------------
void AbstractEntityReplicaFactory::destroyEntity( OpenSteer::AbstractEntity* pkEntity ) const
{
	if( NULL == pkEntity )
	{
		return;
	}
	const OpenSteer::InstanceTracker::Id uiEntityId = pkEntity->getEntityId();	
	if(true == this->m_uidMap.Has( uiEntityId ))
	{
		RakNet::Replica3* pReplicaObject = this->m_uidMap.Get( uiEntityId );		
		this->m_pkReplicaManager->BroadcastDestruction( pReplicaObject, UNASSIGNED_SYSTEM_ADDRESS);
		this->m_uidMap.Set( uiEntityId, NULL );
		ET_DELETE pReplicaObject;
	}
	// do not call the base class in this case !!!
	//	BaseClass::destroyEntity( pkVehicle );
}

//-----------------------------------------------------------------------------
void AbstractEntityReplicaFactory::destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	this->destroyEntity( pkVehicle );
}

//-----------------------------------------------------------------------------
AbstractEntityReplica* AbstractEntityCCReplicaFactory::createEntityReplica( 
	OpenSteer::AbstractPlugin* pPlugin, 
	OpenSteer::EntityClassId classId, 
	bool bIsRemoteObject,  bool bClientReplica) const
{
	return ET_NEW AbstractEntityCCReplica( 
		pPlugin, classId, bIsRemoteObject, bClientReplica );	
}


