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

#include "AbstractVehicleReplicaConnection.h"
#include "AbstractVehicleReplica.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/VehicleClassIds.h"

//-----------------------------------------------------------------------------
// replica connection client
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RakNet::Replica3* AbstractVehicleReplicaConnection::AllocReplica(
	RakNet::BitStream *allocationId,
	RakNet::ReplicaManager3 *replicaManager3)
{
	RakNet::RakString typeName;
	allocationId->Read( typeName );
	OpenSteer::AbstractPlugin* pkPlugin = this->getPlugin();
	if( NULL != pkPlugin )
	{
		OpenSteer::EntityClassId classId = ET_CID_UNKNOWN;
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
		if( classId == ET_CID_UNKNOWN )
		{
			bool bTest = true;
			bTest = false;
			assert( classId != ET_CID_UNKNOWN );
		}
		else
		{
			AbstractVehicleReplica* pkNewReplica = new AbstractVehicleReplica( pkPlugin, classId, true  );
			OpenSteer::AbstractVehicleGroup kVG( pkPlugin->allVehicles() );
			kVG.addVehicle( pkNewReplica->AccessVehicle() );
			return pkNewReplica; 
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// replica factory peer
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* AbstractVehicleReplicaFactory::createVehicle( OpenSteer::EntityClassId classId, OpenSteer::ProximityDatabase* pkProximityDatabase ) const
{
	AbstractVehicleReplica* pkNewReplica = new AbstractVehicleReplica( this->m_pkReplicaManager->getPlugin(), classId, false );	
	if( NULL != pkNewReplica )
	{
		this->m_pkReplicaManager->Reference( pkNewReplica );
		OpenSteer::AbstractVehicle* pkVehicle = pkNewReplica->AccessVehicle();
		if( NULL != pkVehicle )
		{
			this->m_uidMap.Set( pkVehicle->getEntityId(), pkNewReplica );
		}
		return pkVehicle;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
void AbstractVehicleReplicaFactory::destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	const OpenSteer::InstanceTracker::Id uiEntityId = pkVehicle->getEntityId();	
	if(true == this->m_uidMap.Has( uiEntityId ))
	{
		RakNet::Replica3* pReplicaObject = this->m_uidMap.Get( uiEntityId);		
		this->m_pkReplicaManager->BroadcastDestruction( pReplicaObject, UNASSIGNED_SYSTEM_ADDRESS);
		this->m_uidMap.Set( uiEntityId, NULL );
		delete pReplicaObject;
	}
// do not call the base class in this case !!!
//	BaseClass::destroyVehicle( pkVehicle );
}


