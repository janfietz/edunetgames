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
	size_t uiBoidAddress = pkBoid->getEntityId();	
	if(true == this->m_uidMap.Has(uiBoidAddress))
	{
		RakNet::Replica3* pReplicaObject = this->m_uidMap.Get( uiBoidAddress);		
		this->m_pkReplicaManager->BroadcastDestruction( 
			pReplicaObject, UNASSIGNED_SYSTEM_ADDRESS);
		this->m_uidMap.Set(uiBoidAddress, NULL);
		delete pReplicaObject;
	}

}