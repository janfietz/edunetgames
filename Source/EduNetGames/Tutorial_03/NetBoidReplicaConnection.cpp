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
#include "NetBoidReplicaConnection.h"
#include "NetBoidReplica.h"
#include "NetBoidConditionReplica.h"
#include "BoidsPlugin.h"
#include "OpenSteerUT/AbstractVehicleGroup.h"

//-----------------------------------------------------------------------------
NetBoidReplicaConnection::NetBoidReplicaConnection(
	SystemAddress _systemAddress, RakNetGUID _guid,
		OpenSteer::BoidsPlugin* pBoidPlugin) :
		Connection_RM3(_systemAddress, _guid)
{
	m_pBoidPlugin = pBoidPlugin;
}

//-----------------------------------------------------------------------------
RakNet::Replica3* NetBoidReplicaConnection::AllocReplica(
	RakNet::BitStream *allocationId,
	RakNet::ReplicaManager3 *replicaManager3)
{
	static NetBoidReplica kReplica;
	static NetBoidConditionReplica kReplicaCondition;
	RakNet::RakString typeName;
	allocationId->Read(typeName);
	if (typeName==kReplica.GetName()){
		NetBoidReplica* pkNewReplica = ET_NEW NetBoidReplica( this->m_pBoidPlugin, true  );
		OpenSteer::AbstractVehicleGroup kVG( this->m_pBoidPlugin->allVehicles() );
		kVG.addVehicle( pkNewReplica->accessEntity() );
		return pkNewReplica; 
	}
	if (typeName==kReplicaCondition.GetName()){
		NetBoidConditionReplica* pkNewReplica = 
			ET_NEW NetBoidConditionReplica( this->m_pBoidPlugin  );
		return pkNewReplica; 
	}
	return 0;
}

//-----------------------------------------------------------------------------
OpenSteer::AbstractVehicle* NetBoidReplicaFactory::createVehicle( 
	OpenSteer::EntityClassId classId ) const
{
	NetBoidReplica* pkNewReplica = ET_NEW NetBoidReplica( 
		this->m_pkReplicaManager->accessPlugin(), false );		
	this->m_pkReplicaManager->Reference( pkNewReplica );

	OpenSteer::AbstractVehicle* pkVehicle = pkNewReplica->accessEntity();
	this->m_uidMap.Set( pkVehicle->getEntityId(), pkNewReplica );
	return pkNewReplica->accessEntity();
}

//-----------------------------------------------------------------------------
void NetBoidReplicaFactory::destroyVehicle( 
	OpenSteer::AbstractVehicle* pkVehicle ) const
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
//-----------------------------------------------------------------------------
void NetBoidReplicaManager::setPlugin(
	OpenSteer::BoidsPlugin* pPlugin)
{
	this->m_pBoidPlugin = pPlugin;
}
//-----------------------------------------------------------------------------
OpenSteer::BoidsPlugin* NetBoidReplicaManager::accessPlugin( void ) const
{
	return this->m_pBoidPlugin;
}