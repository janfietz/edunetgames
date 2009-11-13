#include "NetBoidFactory.h"
#include "NetBoidReplica.h"

//-----------------------------------------------------------------------------
OpenSteer::Boid* NetBoidReplicaFactory::CreateBoid( 
	OpenSteer::ProximityDatabase& pd )	
{	
	NetBoidReplica* pkNewReplica = new NetBoidReplica( pd );		
	this->m_pkReplicaManager->Reference(pkNewReplica);

	this->m_uidMap.Set( pkNewReplica->AccessVehicle()->getEntityId(), pkNewReplica );
	return pkNewReplica->AccessVehicle();
}

//-----------------------------------------------------------------------------
void NetBoidReplicaFactory::DestroyBoid( const OpenSteer::Boid* pkBoid)
{	
	const OpenSteer::InstanceTracker::Id uiEntityId = pkBoid->getEntityId();	
	if(true == this->m_uidMap.Has(uiEntityId))
	{
		RakNet::Replica3* pReplicaObject = this->m_uidMap.Get( uiEntityId);		
		this->m_pkReplicaManager->BroadcastDestruction( 
			pReplicaObject, UNASSIGNED_SYSTEM_ADDRESS);
		this->m_uidMap.Set(uiEntityId, NULL);
		delete pReplicaObject;
	}

}