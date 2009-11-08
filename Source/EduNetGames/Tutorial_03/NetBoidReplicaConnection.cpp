#include "NetBoidReplicaConnection.h"

#include "NetBoidReplica.h"
#include "NetBoidConditionReplica.h"

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
	RakNet::RakString typeName;
	allocationId->Read(typeName);
	if (typeName=="NetBoidReplica"){
		NetBoidReplica* pkNewReplica = new NetBoidReplica( this->m_pBoidPlugin  );
		this->m_pBoidPlugin->AddBoidToFlock( pkNewReplica->AccessVehicle() );
		return pkNewReplica; 
	}
	if (typeName=="BoidCondition"){
		NetBoidConditionReplica* pkNewReplica = 
			new NetBoidConditionReplica( this->m_pBoidPlugin  );
		return pkNewReplica; 
	}
	return 0;
}