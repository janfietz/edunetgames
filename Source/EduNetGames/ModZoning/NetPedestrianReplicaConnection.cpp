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

#include "NetPedestrianReplicaConnection.h"
#include "NetPedestrianReplica.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"
#include "OpenSteerUT/VehicleClassIds.h"

//-----------------------------------------------------------------------------
// replica connection client
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RakNet::Replica3* NetPedestrianReplicaConnection::AllocReplica(
	RakNet::BitStream *allocationId,
	RakNet::ReplicaManager3 *replicaManager3)
{
	RakNet::RakString typeName;
	allocationId->Read( typeName );
	OpenSteer::EntityClassId kId;
	allocationId->Read( kId );
	OpenSteer::AbstractPlugin* pkPlugin = this->getPlugin();
	if( NULL != pkPlugin )
	{
		static NetPedestrianReplica kReplica;
		if ( typeName == kReplica.GetName() )
		{
			NetPedestrianReplica* pkNewReplica = ET_NEW NetPedestrianReplica( pkPlugin, true  );
			OpenSteer::AbstractVehicleGroup kVG( pkPlugin->allVehicles() );
			kVG.addVehicleToPlugin( pkNewReplica->accessEntity(), pkPlugin );
			return pkNewReplica; 
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// replica factory peer
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* NetPedestrianReplicaFactory::createVehicle( OpenSteer::EntityClassId classId ) const
{
	if( classId == ET_CID_NETPEDESTRIAN )
	{
		NetPedestrianReplica* pkNewReplica = ET_NEW NetPedestrianReplica( this->m_pkReplicaManager->getPlugin(), false );		
		this->m_pkReplicaManager->Reference( pkNewReplica );
		OpenSteer::AbstractVehicle* pkVehicle = pkNewReplica->accessEntity();
		this->m_uidMap.Set( pkVehicle->getEntityId(), pkNewReplica );
		NetworkID kNetWorkId = pkNewReplica->GetNetworkID();
		OpenSteer::NetworkId networkId = kNetWorkId.guid.g;
		pkVehicle->setNetworkId( networkId );
		return pkNewReplica->accessEntity();
	}
	return NULL;
}

//-----------------------------------------------------------------------------
bool NetPedestrianReplicaFactory::destroyVehicle( OpenSteer::AbstractVehicle* pkVehicle ) const
{
	const OpenSteer::InstanceTracker::Id uiEntityId = pkVehicle->getEntityId();	
	if(true == this->m_uidMap.Has( uiEntityId ))
	{
		RakNet::Replica3* pReplicaObject = this->m_uidMap.Get( uiEntityId);		
		this->m_pkReplicaManager->BroadcastDestruction( pReplicaObject, UNASSIGNED_SYSTEM_ADDRESS);
		this->m_uidMap.Set( uiEntityId, NULL );
		ET_DELETE pReplicaObject;
		return true;
	}
	return false;
// do not call the base class in this case !!!
//	BaseClass::destroyVehicle( pkVehicle );
}


