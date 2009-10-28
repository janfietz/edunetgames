#include "NetBoidPlugin.h"
#include "EduNet/network/OSReplicaTypes.h"
#include "OpenSteerExtras/AbstractVehicleGroup.h"

NetPeerBoidPlugin gNetPeerBoidPlugin;
NetClientBoidPlugin gNetClientBoidPlugin;

//-----------------------------------------------------------------------------
RakNet::Replica3* BoidReplicaConnection::AllocReplica(
	RakNet::BitStream *allocationId,
	RakNet::ReplicaManager3 *replicaManager3)
{
	RakNet::RakString typeName;
	allocationId->Read(typeName);
	if (typeName=="BoidReplica"){
		BoidReplica* pkNewReplica = new BoidReplica( this->m_pBoidPlugin  );
		OpenSteer::AbstractVehicleGroup kVG( this->m_pBoidPlugin->allVehicles() );
		kVG.addVehicle( pkNewReplica->AccessVehicle() );
//		this->m_pBoidPlugin->AddBoidToFlock( pkNewReplica->AccessVehicle() );
		return pkNewReplica; 
	}
	return 0;
}

//-----------------------------------------------------------------------------
OpenSteer::Boid* BoidReplicaFactory::CreateBoid( 
	OpenSteer::ProximityDatabase& pd )	
{	
	BoidReplica* pkNewReplica = new BoidReplica( pd );		
	this->m_pkReplicaManager->Reference(pkNewReplica);

	this->m_uidMap.Set((unsigned int)pkNewReplica->AccessVehicle(), pkNewReplica);
	return pkNewReplica->AccessVehicle();
}
//-----------------------------------------------------------------------------
void BoidReplicaFactory::DestroyBoid( const OpenSteer::Boid* pkBoid)
{	
	unsigned int uiBoidAddress = (unsigned int)pkBoid;	
	if(true == this->m_uidMap.Has(uiBoidAddress))
	{
		RakNet::Replica3* pReplicaObject = this->m_uidMap.Get( uiBoidAddress);		
		this->m_pkReplicaManager->BroadcastDestruction( pReplicaObject, UNASSIGNED_SYSTEM_ADDRESS);
		this->m_uidMap.Set(uiBoidAddress, NULL);
		delete pReplicaObject;
	}

}

//-----------------------------------------------------------------------------
NetPeerBoidPlugin::NetPeerBoidPlugin()
{
	this->m_kReplicaManager.SetPlugin(&this->m_kGamePlugIn);

	this->m_pkBoidFactory = new BoidReplicaFactory(&this->m_kReplicaManager);	
	this->m_kGamePlugIn.SetBoidFactory( this->m_pkBoidFactory );
}
//-----------------------------------------------------------------------------
void NetPeerBoidPlugin::StartNetworkSession( void )
{
	PeerPlugin<OpenSteer::BoidsPlugIn>::StartNetworkSession();
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
}

//-----------------------------------------------------------------------------
NetClientBoidPlugin::NetClientBoidPlugin()
{
	this->m_kReplicaManager.SetPlugin(&this->m_kGamePlugIn);

	this->m_pkBoidFactory = new BoidDummyFactory(&this->m_kReplicaManager);	
	this->m_kGamePlugIn.SetBoidFactory( this->m_pkBoidFactory );
}
//-----------------------------------------------------------------------------
void NetClientBoidPlugin::StartNetworkSession( void )
{
	ClientPlugin<OpenSteer::BoidsPlugIn>::StartNetworkSession();
	this->m_pNetInterface->AttachPlugin(&this->m_kReplicaManager);
}